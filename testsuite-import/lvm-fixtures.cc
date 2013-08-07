#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stddef.h>

#include <iostream>
#include <sstream>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/test/unit_test.hpp>

#include "testsuite-import/helpers.h"
#include "testsuite-import/lvm-fixtures.h"
#include "testsuite-import/helpers.h"

namespace testsuiteimport { namespace lvm
{
    using std::cout;
    using std::cerr;
    using std::string;
    using std::vector;

    const string LvmSubvolumeWrapper::lvm_fs_type = "LVM2";


    LvmSubvolumeWrapper::LvmSubvolumeWrapper(const string& vg_name, const string& lv_orig_name, const string& lv_name, bool ro)
	:  SubvolumeWrapper(NULL), vg_name(vg_name), lv_name(lv_name), lv_orig_name(lv_orig_name)
    {
	lvcreate_thin_snapshot_wrapper(vg_name, lv_orig_name, lv_name, ro);
	std::cout << "created LVM subvolume: " << devicepath() << std::endl;
    }


    LvmSubvolumeWrapper::LvmSubvolumeWrapper(const string& vg_name, const string& lv_name)
	: SubvolumeWrapper(NULL), vg_name(vg_name), lv_name(lv_name)
    {
	lvcreate_non_thin_lv_wrapper(vg_name, lv_name);
	std::cout << "created LVM subvolume: " << devicepath() << std::endl;
    }


    LvmSubvolumeWrapper::LvmSubvolumeWrapper(const string& vg_name, const string& lv_orig_name, unsigned int num, bool ro)
	: SubvolumeWrapper(new InfoDirWithSnapshotDir(LvmGeneralFixture::f_conf_lvm_snapshots_prefix, num)),
	vg_name(vg_name),	
	lv_name(LvmGeneralFixture::f_conf_lvm_origin_lv_name + get_lv_snapshot_suffix(p_info_dir->f_num)),
	lv_orig_name(lv_orig_name)
    {
	lvcreate_thin_snapshot_wrapper(vg_name, lv_orig_name, lv_name, ro);
	std::cout << "created LVM subvolume: " << devicepath() << std::endl;
    }


    LvmSubvolumeWrapper::LvmSubvolumeWrapper(const string& vg_name, const string& lv_orig_name, const string& lv_name, unsigned int num, bool ro)
	: SubvolumeWrapper(new InfoDirWithSnapshotDir(LvmGeneralFixture::f_conf_lvm_snapshots_prefix, num)),
	vg_name(vg_name),	
	lv_name(lv_name),
	lv_orig_name(lv_orig_name)
    {
	lvcreate_thin_snapshot_wrapper(vg_name, lv_orig_name, lv_name, ro);
	std::cout << "created LVM subvolume: " << devicepath() << std::endl;
    }


    LvmSubvolumeWrapper::~LvmSubvolumeWrapper()
    {
	try
	{
	    umount();
	}
	catch (const ImportTestsuiteException &e) {}
	try
	{
	    lvremove_wrapper(vg_name, lv_name);
	    std::cout << "delete LVM subvolume: " << devicepath() << ": success" << std::endl;
	}
	catch (const ImportTestsuiteException &e) {}
    }


    bool LvmSubvolumeWrapper::exists() const
    {
	return check_lv_exists(vg_name, lv_name);
    }


    bool LvmSubvolumeWrapper::is_mounted() const
    {
	return check_is_mounted(vg_name, lv_name);
    }


    void LvmSubvolumeWrapper::umount() const
    {
	string tmp = "-n -o TARGET " + devicepath();

	vector<string> args;
	boost::split(args, tmp, boost::is_any_of(" \t\n"), boost::token_compress_on);

	try
	{
	    SimpleSystemCmd cmd("/usr/bin/findmnt", args);

	    if (cmd.retcode())
	    {
		cerr << "findmnt " << devicepath() << " failed with ret_code: " << cmd.retcode() << std::endl;
		for (vector<string>::const_iterator cit = cmd.stderr_cbegin(); cit != cmd.stderr_cend(); cit++)
		    std::cerr << "findmnt stderr: " << *cit << std::endl;
		for (vector<string>::const_iterator cit = cmd.stdout_cbegin(); cit != cmd.stdout_cend(); cit++)
		    std::cerr << "findmnt stdout: " << *cit << std::endl;
		throw LvmImportTestsuiteException();
	    }

	    for (vector<string>::const_iterator cit = cmd.stdout_cbegin(); cit != cmd.stdout_cend(); cit++)
	    {
		if (umount2(cit->c_str(), MNT_DETACH))
		    std::cout << "umount2(" << *cit << ", MNT_DETACH) failed" << std::endl;
	    }
	}
	catch (const SimpleSystemCmdException &e)
	{
	    std::cerr << "SimpleSystemCmd(\"/usr/sbin/findmnt\") failed" << std::endl;
	    throw ImportTestsuiteException();
	}
    }


    string
    LvmSubvolumeWrapper::get_lv_snapshot_suffix(unsigned int num)
    {
	std::ostringstream oss;
	oss << "-snapshot" << num;

	return oss.str();
    }


    CreateSnapshotEnvironment::CreateSnapshotEnvironment()
	: f_valid_info_dir(LvmGeneralFixture::f_conf_lvm_snapshots_prefix),
	f_info_snapshot_dir_exists(LvmGeneralFixture::f_conf_lvm_snapshots_prefix),
	f_invalid_snapshot_dir(LvmGeneralFixture::f_conf_lvm_snapshots_prefix)
    {
    }


    CreateSnapshotFailOnEnvironment::CreateSnapshotFailOnEnvironment()
	: f_invalid_snap_dir(LvmGeneralFixture::f_conf_lvm_snapshots_prefix, 1)
    {
    }


    CreateSnapshotFailOnEnvironment::~CreateSnapshotFailOnEnvironment()
    {
	try
	{
	    lvremove_wrapper(LvmGeneralFixture::f_conf_lvm_vg_name, f_lvm->snapshotLvName(f_invalid_snap_dir.f_num));
	}
	catch (const LvmImportTestsuiteException &e) {}
    }


    RemoveSnapshotEnvironment::RemoveSnapshotEnvironment()
	: f_snapshot_dir(LvmGeneralFixture::f_conf_lvm_snapshots_prefix)
    {
    }


    LvmCloneSnapshot::LvmCloneSnapshot()
	: f_valid_subvolume(LvmGeneralFixture::f_conf_lvm_vg_name, LvmGeneralFixture::f_conf_lvm_origin_lv_name, "tc_clone_snapshot_01", false),
	f_missing_lv_vg(LvmGeneralFixture::f_conf_lvm_vg_name),
	f_missing_lv_lv("ThiSiMissiNgLv"),
	f_info_dir_1(LvmGeneralFixture::f_conf_lvm_snapshots_prefix, 1),
	f_info_dir_2(LvmGeneralFixture::f_conf_lvm_snapshots_prefix, 1)
    {
    }


    LvmCloneSnapshot::~LvmCloneSnapshot()
    {
	try
	{
	    // we expect Lvm will clone f_valid_subvolume into snapshot #f_num
	    lvremove_wrapper(f_valid_subvolume.vg_name, f_lvm->snapshotLvName(f_info_dir_1.f_num) );
	}
	catch (const ImportTestsuiteException &e)
	{
	    std::cerr << "there is no cloned snapshot "
		      << f_valid_subvolume.vg_name << "/"
		      << f_lvm->snapshotLvName(f_info_dir_1.f_num) << std::endl;
	}
    }


    LvmMountSnapshotBySubvolume::LvmMountSnapshotBySubvolume()
	: f_snapshot_dir_1(LvmGeneralFixture::f_conf_lvm_snapshots_prefix, 1),
	f_snapshot_dir_2(LvmGeneralFixture::f_conf_lvm_snapshots_prefix, 1),
	f_mount_volume(LvmGeneralFixture::f_conf_lvm_vg_name, LvmGeneralFixture::f_conf_lvm_origin_lv_name, "tc_mount_snapshot_by_device_01"),
	f_missing_dev_path("/dev/mapper/missing_vg-missing_lv")
    {
    }


    LvmCheckImportedSnapshot::LvmCheckImportedSnapshot()
	: LvmGeneralFixture(), f_missing_vg_vg("ThiSiSMisSingVg"),
	f_missing_vg_lv(LvmGeneralFixture::f_conf_lvm_origin_lv_name),
	f_missing_lv_vg(LvmGeneralFixture::f_conf_lvm_vg_name),
	f_missing_lv_lv("ThisIsMissingLogVol"),
	f_nonthin_subvolume(LvmGeneralFixture::f_conf_lvm_vg_name, "test_non_thin_volume_01"),
	f_foreign_vg_subvolume(LvmGeneralFixture::f_conf_lvm_foreign_vg_name, "test_lv_volume_01"),
	f_current_subvolume_vg_name(LvmGeneralFixture::f_conf_lvm_vg_name),
	f_current_subvolume_lv_name(LvmGeneralFixture::f_conf_lvm_origin_lv_name),
	f_rw_wrong_fs_uuid_subvolume(LvmGeneralFixture::f_conf_lvm_vg_name, LvmGeneralFixture::f_conf_lvm_origin_lv_name, "test_rw_snap_wrong_fs_uuid_01", false),
	f_rw_subvolume(LvmGeneralFixture::f_conf_lvm_vg_name, LvmGeneralFixture::f_conf_lvm_origin_lv_name, "test_rw_snap_01", false),
	f_ro_wrong_fs_uuid_subvolume(LvmGeneralFixture::f_conf_lvm_vg_name, LvmGeneralFixture::f_conf_lvm_origin_lv_name, "test_ro_snap_wrong_uuid_01", false),
	f_ro_subvolume(LvmGeneralFixture::f_conf_lvm_vg_name, LvmGeneralFixture::f_conf_lvm_origin_lv_name, "test_ro_snap_01")
    {
	string::size_type open = f_lvm->fstype().find("(");
	string::size_type close = f_lvm->fstype().find(")");
	string fstype = f_lvm->fstype().substr(open + 1, close - open - 1);

	modify_fs_uuid(f_rw_wrong_fs_uuid_subvolume.vg_name, f_rw_wrong_fs_uuid_subvolume.lv_name, fstype);
	modify_fs_uuid(f_ro_wrong_fs_uuid_subvolume.vg_name, f_ro_wrong_fs_uuid_subvolume.lv_name, fstype);

	change_permission(f_ro_wrong_fs_uuid_subvolume.vg_name, f_ro_wrong_fs_uuid_subvolume.lv_name, true);

	std::cout << "LvmCheckImportedSnapshot() ctor done" << std::endl;
    }


    DeleteSnapshotByVgLv::DeleteSnapshotByVgLv()
	: LvmGeneralFixture(),
	f_subvolume(LvmGeneralFixture::f_conf_lvm_vg_name, LvmGeneralFixture::f_conf_lvm_origin_lv_name, "test_delete_subvolume_by_vg_lv"),
	f_missing_vg_vg("iHavENoBodY"), f_missing_vg_lv(LvmGeneralFixture::f_conf_lvm_origin_lv_name),
	f_missing_lv_vg(LvmGeneralFixture::f_conf_lvm_vg_name), f_missing_lv_lv("ThiSiSmiSSingLv")
    {
    }


    DeleteSnapshotByVgLv::~DeleteSnapshotByVgLv()
    {
	try
	{
	    lvremove_wrapper(f_subvolume.vg_name, f_subvolume.lv_name);
	}
	// in case of failure, remove volume that should be deleted by Lvm unit
	catch (const LvmImportTestsuiteException& e) {}
    }

}}
