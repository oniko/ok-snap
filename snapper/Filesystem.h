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


#ifndef SNAPPER_FILESYSTEM_H
#define SNAPPER_FILESYSTEM_H


#include <string>
#include <vector>
#include <map>

#include "snapper/Exception.h"
#include "snapper/FileUtils.h"
#include "snapper/Compare.h"


namespace snapper
{
    using std::string;
    using std::vector;
    using std::map;


    class MtabData;
    class ImportMetadata;

    struct CreateSnapshotEnvironmentException : public SnapperException
    {
	explicit CreateSnapshotFailedException() throw() {}
	virtual const char* what() const throw() { return "create snapshot environment failed"; }
    };

    struct RemoveSnapshotEnvironmentException : public SnapperException
    {
	explicit CreateSnapshotFailedException() throw() {}
	virtual const char* what() const throw() { return "remove snapshot environment failed"; }
    };

    class Filesystem
    {
    public:

	Filesystem(const string& subvolume) : subvolume(subvolume) {}
	virtual ~Filesystem() {}

	static Filesystem* create(const string& fstype, const string& subvolume);
	virtual ImportMetadata* createImportMetadata(const map<string,string> &raw_data) const = 0;

	virtual string fstype() const = 0;

	virtual void createConfig() const = 0;
	virtual void deleteConfig() const = 0;

	virtual string snapshotDir(unsigned int num) const = 0;

	virtual SDir openSubvolumeDir() const;
	virtual SDir openInfosDir() const = 0;
	virtual SDir openInfoDir(unsigned int num) const;
	virtual SDir openSnapshotDir(unsigned int num) const = 0;

	virtual void createSnapshotEnvironment(unsigned int num) const = 0;
	virtual void removeSnapshotEnvironment(unsigned int num) const = 0;

	virtual void createSnapshot(unsigned int num) const = 0;
	virtual void deleteSnapshot(unsigned int num) const = 0;

	virtual bool isSnapshotMounted(unsigned int num) const = 0;
	virtual void mountSnapshot(unsigned int num) const = 0;
	virtual void mountSnapshot(unsigned int num, const string &subvolume) const = 0;
	virtual void umountSnapshot(unsigned int num) const = 0;

	virtual bool checkSnapshot(unsigned int num) const = 0;

	virtual void cmpDirs(const SDir& dir1, const SDir& dir2, cmpdirs_cb_t cb) const;

    protected:

	const string subvolume;

	static vector<string> filter_mount_options(const vector<string>& options);

	static bool mount(const string& device, const SDir& dir, const string& mount_type,
			  const vector<string>& options);
	static bool umount(const SDir& dir, const string& mount_point);

    };

}


#endif
