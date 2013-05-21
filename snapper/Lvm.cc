/*
 * Copyright (c) [2011-2013] Novell, Inc.
 *
 * All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, contact Novell, Inc.
 *
 * To contact Novell about this file by physical or electronic mail, you may
 * find current contact information at www.novell.com.
 */


#include "config.h"

#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <asm/types.h>
#include <boost/algorithm/string.hpp>

#include "snapper/Log.h"
#include "snapper/Filesystem.h"
#include "snapper/Lvm.h"
#include "snapper/Snapper.h"
#include "snapper/SnapperTmpl.h"
#include "snapper/SystemCmd.h"
#include "snapper/SnapperDefines.h"
#include "snapper/Regex.h"
#include "snapper/LvmImportMetadata.h"
#include "snapper/AppUtil.h"


namespace snapper
{

    Filesystem*
    Lvm::create(const string& fstype, const string& subvolume)
    {
	Regex rx("^lvm\\(([_a-z0-9]+)\\)$");
	if (rx.match(fstype))
	    return new Lvm(subvolume, rx.cap(1));

	return NULL;
    }


    Lvm::Lvm(const string& subvolume, const string& mount_type)
	: Filesystem(subvolume), mount_type(mount_type),
	caps(LvmCapabilities::get_lvm_capabilities())
    {
	if (access(LVCREATEBIN, X_OK) != 0)
	{
	    throw ProgramNotInstalledException(LVCREATEBIN " not installed");
	}

	if (access(LVSBIN, X_OK) != 0)
	{
	    throw ProgramNotInstalledException(LVSBIN " not installed");
	}

	if (access(LVCHANGEBIN, X_OK) != 0)
	{
	    throw ProgramNotInstalledException(LVCHANGEBIN " not installed");
	}

	bool found = false;
	MtabData mtab_data;

	if (!getMtabData(subvolume, found, mtab_data))
	    throw InvalidConfigException();

	if (!found)
	{
	    y2err("filesystem not mounted");
	    throw InvalidConfigException();
	}

	if (!detectThinVolumeNames(mtab_data))
	    throw InvalidConfigException();

	mount_options = filter_mount_options(mtab_data.options);
	if (mount_type == "xfs")
	    mount_options.push_back("nouuid");

	fs_uuid = get_fs_uuid(mtab_data.device);
	if (fs_uuid.empty())
	    y2war("Empty fs uuid");
    }


    void
    Lvm::createConfig() const
    {
	SDir subvolume_dir = openSubvolumeDir();

	int r1 = subvolume_dir.mkdir(".snapshots", 0750);
	if (r1 != 0 && errno != EEXIST)
	{
	    y2err("mkdir failed errno:" << errno << " (" << strerror(errno) << ")");
	    throw CreateConfigFailedException("mkdir failed");
	}
    }


    void
    Lvm::deleteConfig() const
    {
	SDir subvolume_dir = openSubvolumeDir();

	int r1 = subvolume_dir.unlink(".snapshots", AT_REMOVEDIR);
	if (r1 != 0)
	{
	    y2err("rmdir failed errno:" << errno << " (" << strerror(errno) << ")");
	    throw DeleteConfigFailedException("rmdir failed");
	}
    }


    string
    Lvm::snapshotDir(unsigned int num) const
    {
	return (subvolume == "/" ? "" : subvolume) + "/.snapshots/" + decString(num) +
	    "/snapshot";
    }


    SDir
    Lvm::openInfosDir() const
    {
	SDir subvolume_dir = openSubvolumeDir();
	SDir infos_dir(subvolume_dir, ".snapshots");

	struct stat stat;
	if (infos_dir.stat(&stat) != 0)
	{
	    throw IOErrorException();
	}

	if (stat.st_uid != 0)
	{
	    y2err(".snapshots must have owner root");
	    throw IOErrorException();
	}

	if (stat.st_gid != 0 && stat.st_mode & S_IWGRP)
	{
	    y2err(".snapshots must have group root or must not be group-writable");
	    throw IOErrorException();
	}

	if (stat.st_mode & S_IWOTH)
	{
	    y2err(".snapshots must not be world-writable");
	    throw IOErrorException();
	}

	return infos_dir;
    }


    SDir
    Lvm::openSnapshotDir(unsigned int num) const
    {
	SDir info_dir = openInfoDir(num);
	SDir snapshot_dir(info_dir, "snapshot");

	return snapshot_dir;
    }


    string
    Lvm::snapshotLvName(unsigned int num) const
    {
	return lv_name + "-snapshot" + decString(num);
    }


    void
    Lvm::createSnapshot(unsigned int num) const
    {
	SystemCmd cmd(LVCREATEBIN " --permission r --snapshot --name " +
		      quote(snapshotLvName(num)) + " " + quote(vg_name + "/" + lv_name));
	if (cmd.retcode() != 0)
	    throw CreateSnapshotFailedException();

	try {
	    createSnapshotEnvironment(num);
	}
	catch(const CreateSnapshotFailedException &e)
	{
	    throw;
	}
    }


    void
    Lvm::deleteSnapshot(unsigned int num) const
    {
	deleteSnapshot(vg_name, snapshotLvName(num));
	removeSnapshotEnvironment(num);
    }


    bool
    Lvm::isSnapshotMounted(unsigned int num) const
    {
	bool mounted = false;
	MtabData mtab_data;

	if (!getMtabData(snapshotDir(num), mounted, mtab_data))
	    throw IsSnapshotMountedFailedException();

	return mounted;
    }


    void
    Lvm::mountSnapshot(unsigned int num) const
    {
	if (isSnapshotMounted(num))
	    return;

	try
	{
	    activateSnapshot(vg_name, snapshotLvName(num));
	}
	catch (const LvmActivationException& e)
	{
	    y2err("Couldn't mount snapshot " << num);
	    throw MountSnapshotFailedException();
	}

	SDir snapshot_dir = openSnapshotDir(num);

	if (!mount(getDevice(num), snapshot_dir, mount_type, mount_options))
	    throw MountSnapshotFailedException();
    }


    void
    Lvm::umountSnapshot(unsigned int num) const
    {
	if (!isSnapshotMounted(num))
	    return;

	SDir info_dir = openInfoDir(num);

	if (!umount(info_dir, "snapshot"))
	    throw UmountSnapshotFailedException();

	try
	{
	    deactivateSnapshot(vg_name, snapshotLvName(num));
	}
	catch (const LvmDeactivatationException& e)
	{
	    y2war("Snapshot #" << num << "deactivation failed");
	}
    }


    bool
    Lvm::checkSnapshot(unsigned int num) const
    {
	struct stat stat;
	int r1 = ::stat(getDevice(num).c_str(), &stat);

	return (r1 == 0 && S_ISBLK(stat.st_mode)) || detectInactiveSnapshot(vg_name, snapshotLvName(num));
    }


    bool
    Lvm::detectThinVolumeNames(const MtabData& mtab_data)
    {
	Regex rx("^/dev/mapper/(.+[^-])-([^-].+)$");
	if (!rx.match(mtab_data.device))
	{
	    y2err("could not detect lvm names from '" << mtab_data.device << "'");
	    return false;
	}

	vg_name = boost::replace_all_copy(rx.cap(1), "--", "-");
	lv_name = boost::replace_all_copy(rx.cap(2), "--", "-");

	return detectThinVolumeNames(vg_name, lv_name);
    }


    void Lvm::cloneSnapshot(unsigned int num, const string &vg_name, const string &lv_name) const
    {
	SystemCmd cmd(LVCREATEBIN " --permission r --snapshot --name " +
		      quote(snapshotLvName(num)) + " " + quote(vg_name + "/" + lv_name));

	if (cmd.retcode() != 0)
	    throw ImportSnapshotFailedException();

	try {
	    createSnapshotEnvironment(num);
	}
	catch(const CreateSnapshotFailedException &e)
	{
	    throw;
	}
    }


    void
    Lvm::deleteSnapshot(const string &vg_name, const string &lv_name) const
    {
	SystemCmd cmd(LVREMOVEBIN " --force " + quote(vg_name + "/" + lv_name));
	if (cmd.retcode() != 0)
	    throw DeleteSnapshotFailedException();
    }


    ImportMetadata* Lvm::createImportMetadata(const map<string,string> &raw_data) const
    {
	return new LvmImportMetadata(raw_data, this);
    }


    void Lvm::createSnapshotEnvironment(unsigned int num) const
    {
	SDir info_dir = openInfoDir(num);

	// TODO: think about flags!
	int fd = info_dir.open("snapshot", O_RDONLY | O_CLOEXEC | O_NOFOLLOW);
	if (fd < 0)
	{
	    if (info_dir.mkdir("snapshot", 0755))
	    {
		y2err("mkdir failed with errno:" << errno << " (" << strerror(errno) << ")");
		// TODO: SnapshotEnvironmentException...
		throw CreateSnapshotFailedException();
	    }
	}
	else
	{
	    struct stat buff;

	    if (fstat(fd, &buff) || !S_ISDIR(buff.st_mode))
	    {
		close(fd);

		y2err("stat failed or dentry 'snapshot' exists but it's not actually a directory!");
		// TODO: SnapshotEnvironmentException...
		throw CreateSnapshotFailedException();
	    }

	    close(fd);
	}
    }


    void Lvm::removeSnapshotEnvironment(unsigned int num) const
    {
	SDir info_dir = openInfoDir(num);
	/// hmm. try to test errors related to 'directory is mounted...'
	info_dir.unlink("snapshot", AT_REMOVEDIR);

	SDir infos_dir = openInfosDir();
	// directory '<num>' will be most propably not empty
	infos_dir.unlink(decString(num), AT_REMOVEDIR);
    }


    void
    Lvm::mountSnapshot(unsigned int num, const string &device_path) const
    {
	if (isSnapshotMounted(num))
	    return;

	SDir snapshot_dir = openSnapshotDir(num);

	if (!mount(device_path, snapshot_dir, mount_type, mount_options))
	    throw MountSnapshotFailedException();
    }


    bool
    Lvm::detectThinVolumeNames(const string& vg_name, const string& lv_name) const
    {
	SystemCmd cmd(LVSBIN " -o segtype --noheadings " + quote(vg_name + "/" + lv_name));

	if (cmd.retcode() != 0) {
	    y2err("could not detect segment type infromation from: " << vg_name << "/" << lv_name);
	    return false;
	}

	string segtype = boost::trim_copy(cmd.getLine(0));

	if (segtype.compare("thin")) {
	    y2err(vg_name << "/" << lv_name << " is not a LVM thin volume");
	    return false;
	}

	return true;
    }


    bool Lvm::checkImportedSnapshot(const string& vg_name, const string& lv_name) const
    {
	return (vg_name == this->vg_name) &&
	       (lv_name != this->lv_name) &&
	       detectThinVolumeNames(vg_name, lv_name);
    }


    string
    Lvm::getDevice(unsigned int num) const
    {
	return "/dev/mapper/" + boost::replace_all_copy(vg_name, "-", "--")
	       + "-" + boost::replace_all_copy(snapshotLvName(num), "-", "--");
    }


    void
    Lvm::activateSnapshot(const string& vg_name, const string& lv_name) const
    {
	SystemCmd cmd(LVCHANGEBIN + caps->get_ignoreactivationskip() + " -ay " + quote(vg_name + "/" + lv_name));
	if (cmd.retcode() != 0)
	{
	    y2err("Couldn't activate snapshot " << vg_name << "/" << lv_name);
	    throw LvmActivationException();
	}
    }


    void
    Lvm::deactivateSnapshot(const string& vg_name, const string& lv_name) const
    {
	SystemCmd cmd(LVCHANGEBIN " -an " + quote(vg_name + "/" + lv_name));
	if (cmd.retcode())
	    throw LvmDeactivatationException();
    }


    bool
    Lvm::detectInactiveSnapshot(const string& vg_name, const string& lv_name) const
    {
	SystemCmd cmd(LVSBIN " " + quote(vg_name + "/" + lv_name));
	return cmd.retcode() == 0;
    }


    LvmCapabilities::LvmCapabilities()
	: ignoreactivationskip(), time_support(false)
    {
	SystemCmd cmd(string(LVSBIN " --version"));

	if (cmd.retcode() != 0)
	{
	    y2war("Couldn't get LVM version info");
	}
	else
	{
	    Regex rx(".*LVM[[:space:]]+version:[[:space:]]+([0-9]+)\\.([0-9]+)\\.([0-9]+).*$");

	    if (!rx.match(cmd.getLine(0)))
	    {
		y2war("LVM version format didn't match");
	    }
	    else
	    {
		uint16_t maj, min, rev;

		rx.cap(1) >> maj;
		rx.cap(2) >> min;
		rx.cap(3) >> rev;

		lvm_version version(maj, min, rev);

		if (version >= lvm_version(2,2,99))
		{
		    ignoreactivationskip = " -K";
		}

		time_support = (version >= lvm_version(2,2,88));
	    }
	}
    }


    bool
    operator>=(const lvm_version& a, const lvm_version& b)
    {
	return a.version >= b.version;
    }


    LvmCapabilities*
    LvmCapabilities::get_lvm_capabilities()
    {
	/*
	 * NOTE: verify only one thread can access
	 * 	 this section at the same time!
	 */
	static LvmCapabilities caps;

	return &caps;
    }


    string
    LvmCapabilities::get_ignoreactivationskip() const
    {
	return ignoreactivationskip;
    }


    bool
    LvmCapabilities::get_time_support() const
    {
	return time_support;
    }

}
