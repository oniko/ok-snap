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

#include <boost/test/unit_test.hpp>

#include "testsuite-import/lvm-fixtures.h"
#include "testsuite-import/helpers.h"

namespace testsuiteimport { namespace lvm
{
    using std::cout;
    using std::cerr;

    const string LvmSubvolumeWrapper::lvm_fs_type = "LVM2";

    LvmSubvolumeWrapper::LvmSubvolumeWrapper(const string& vg_name, const string& lv_orig_name, const string& lv_name, bool ro)
	:  SubvolumeWrapper(), vg_name(vg_name), lv_name(lv_name), lv_orig_name(lv_orig_name)
    {
	lvcreate_thin_snapshot_wrapper(vg_name, lv_orig_name, lv_name, ro);	    
    }


    LvmSubvolumeWrapper::LvmSubvolumeWrapper(const string& vg_name, const string& lv_name)
	: SubvolumeWrapper()
    {
	lvcreate_non_thin_lv_wrapper(vg_name, lv_name);
    }


    LvmSubvolumeWrapper::~LvmSubvolumeWrapper()
    {
	if (umount2(devicepath().c_str(), MNT_DETACH))
	    std::cerr << "umount2( \"" << devicepath() << "\", MNT_DETACH) failed!" << std::endl;
	try
	{
	    lvremove_wrapper(vg_name, lv_name);
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


    void InfoDirectory::infodir_init()
    {
	std::cout << "CreateSnapshotEnvironment ctor" << std::endl;

	int dirfd = open(infos_dir.c_str(), O_RDONLY | O_NOATIME | O_CLOEXEC | O_NOFOLLOW);
	if (dirfd < 0)
	    BOOST_FAIL( "Couldn't open " << infos_dir << " directory" );

	std::ostringstream oss(std::ios_base::ate);
	oss << f_num;

	while (mkdirat(dirfd, oss.str().c_str(), 0755) && f_num < 100)
	{
	    f_num++;

	    oss.str();
	    oss.clear();
	    oss << f_num;

	    if (errno == EEXIST)
		continue;

	    BOOST_FAIL( "Couldn't create the info directory" );
	}

	if (f_num >= 100)
	    BOOST_FAIL( "Something went terribly wrong" );

	f_info_dir = infos_dir + "/" + oss.str();

	f_dirfd = openat(dirfd, oss.str().c_str(), O_RDONLY | O_NOATIME | O_CLOEXEC | O_NOFOLLOW);

	if (f_dirfd < 0)
	    BOOST_FAIL( "Can't open: " << oss );

	struct stat buff;

	if (fstat(f_dirfd, &buff) || !S_ISDIR(buff.st_mode))
	{
	    close(f_dirfd);
	    BOOST_FAIL( "Can't stat dir: " << f_info_dir << " or the d-entry is not a directory" );
	}
    }

    InfoDirectory::InfoDirectory(const string& infos_dir_loc)
	: f_num(0), f_info_dir(infos_dir_loc)
    {
	infodir_init();
    }

    InfoDirectory::InfoDirectory(const string& infos_dir_loc, unsigned int num)
	: f_num(num), f_info_dir(infos_dir_loc)
    {
	infodir_init();
    }

    InfoDirectory::~InfoDirectory()
    {
	deep_rmdirat(f_dirfd);
	close(f_dirfd);

	if (rmdir(f_info_dir.c_str()))
	{
	    perror("rmdir");
	    BOOST_TEST_MESSAGE( "Can't remove: " << f_info_dir );
	}
    }

    void InfoDirWithSnapshotDir::init()
    {
	if (mkdirat(f_dirfd, "snapshot", 0755))
	    BOOST_FAIL( "Can't create snapshot directory in test environment" );
    }

    InfoDirWithSnapshotDir::InfoDirWithSnapshotDir()
	: InfoDirectory()
    {
	init();
    }


    InfoDirWithSnapshotDir::InfoDirWithSnapshotDir(unsigned int num)
	: InfoDirectory(num)
    {
	init();
    }


    InfoDirWithInvalidSnapshotDir::InfoDirWithInvalidSnapshotDir()
	: InfoDirectory()
    {
	init();
    }


    InfoDirWithInvalidSnapshotDir::InfoDirWithInvalidSnapshotDir(unsigned int num)
	: InfoDirectory(num)
    {
	init();
    }


    void
    InfoDirWithInvalidSnapshotDir::init()
    {
	std::cout << "CreateSnapshotEnvironmentFailure ctor" << std::endl;

	int fd = openat(f_dirfd, "snapshot", O_WRONLY | O_CREAT | O_EXCL | O_CLOEXEC, 0755);
	if (fd < 0)
	    BOOST_FAIL( "Can't create 'snapshot' file");

	close(fd);
    }


    CreateSnapshotFailOnEnvironment::~CreateSnapshotFailOnEnvironment()
    {
	try
	{
	    lvremove_wrapper(LvmGeneralFixture::f_conf_lvm_vg_name, f_lvm->snapshotLvName(f_invalid_snap_dir.f_num));
	}
	catch (const LvmImportTestsuiteException &e) {}
    }


    LvmCloneSnapshot::LvmCloneSnapshot()
	: f_valid_subvolume(LvmGeneralFixture::f_conf_lvm_vg_name, LvmGeneralFixture::f_conf_lvm_origin_lv_name, "tc_clone_snapshot_01", false),
	f_missing_lv_vg(LvmGeneralFixture::f_conf_lvm_vg_name),
	f_missing_lv_lv("ThiSiMissiNgLv"), f_info_dir_1(1), f_info_dir_2(1)
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
	: f_snapshot_dir_1(1), f_snapshot_dir_2(1),
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
	f_ro_wrong_fs_uuid_subvolume(LvmGeneralFixture::f_conf_lvm_vg_name, LvmGeneralFixture::f_conf_lvm_origin_lv_name, "test_ro_snap_wrong_uuid_01"),
	f_ro_subvolume(LvmGeneralFixture::f_conf_lvm_vg_name, LvmGeneralFixture::f_conf_lvm_origin_lv_name, "test_ro_snap_01")
    {
	string::size_type open = f_lvm->fstype().find("(");
	string::size_type close = f_lvm->fstype().find(")");
	string fstype = f_lvm->fstype().substr(open + 1, close - open - 1);

	modify_fs_uuid(f_rw_wrong_fs_uuid_subvolume.vg_name, f_rw_wrong_fs_uuid_subvolume.lv_name, fstype);
	modify_fs_uuid(f_ro_wrong_fs_uuid_subvolume.vg_name, f_ro_wrong_fs_uuid_subvolume.lv_name, fstype);
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
