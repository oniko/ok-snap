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

    void CreateSnapshotEnvironment::init()
    {
	std::cout << "CreateSnapshotEnvironment ctor" << std::endl;

	std::ostringstream oss;
	oss << f_conf_lvm_snapshots_prefix << f_num;

	while (mkdir(oss.str().c_str(), 0755) && f_num < 100)
	{
	    f_num++;

	    // TODO: WTF?
	    oss.str(f_conf_lvm_snapshots_prefix);
	    oss.clear();
	    oss << f_conf_lvm_snapshots_prefix << f_num;

	    if (errno == EEXIST)
		continue;

	    BOOST_FAIL( "Couldn't create the info directory" );
	}

	if (f_num >= 100)
	    BOOST_FAIL( "Something went terribly wrong" );

	f_snapshot_dir = oss.str();

	f_dirfd = open(oss.str().c_str(), O_RDONLY | O_CLOEXEC | O_NOFOLLOW);

	if (f_dirfd < 0)
	    BOOST_FAIL( "Can't open: " << oss );

	struct stat buff;

	if (fstat(f_dirfd, &buff) || !S_ISDIR(buff.st_mode))
	{
	    close(f_dirfd);
	    BOOST_FAIL( "Can't stat dir: /testsuite-import/.snapshots/" << f_num << " or the d-entry is not a directory" );
	}
    }

    CreateSnapshotEnvironment::CreateSnapshotEnvironment()
	: LvmGeneralFixture(), f_num(0)
    {
	init();
    }

    CreateSnapshotEnvironment::CreateSnapshotEnvironment(unsigned int num)
	: LvmGeneralFixture(), f_num(num)
    {
	init();
    }

    CreateSnapshotEnvironment::~CreateSnapshotEnvironment()
    {
	deep_rmdirat(f_dirfd);
	close(f_dirfd);

	if (rmdir(f_snapshot_dir.c_str()))
	{
	    perror("rmdir");
	    BOOST_TEST_MESSAGE( "Can't remove: " << f_snapshot_dir );
	}
    }

    void CreateSnapshotEnvironmentDirExists::init()
    {
	if (mkdirat(f_dirfd, "snapshot", 0755))
	    BOOST_FAIL( "Can't create snapshot directory in test environment" );
    }

    CreateSnapshotEnvironmentDirExists::CreateSnapshotEnvironmentDirExists()
	: CreateSnapshotEnvironment()
    {
	init();
    }

    CreateSnapshotEnvironmentDirExists::CreateSnapshotEnvironmentDirExists(unsigned int num)
	: CreateSnapshotEnvironment(num)
    {
	init();
    }

    CreateSnapshotEnvironmentFailure::CreateSnapshotEnvironmentFailure()
	: CreateSnapshotEnvironment()
    {
	std::cout << "CreateSnapshotEnvironmentFailure ctor" << std::endl;

	int fd = openat(f_dirfd, "snapshot", O_WRONLY | O_CREAT | O_EXCL | O_CLOEXEC, 0755);
	if (fd < 0)
	    BOOST_FAIL( "Can't create 'snapshot' file");

	close(fd);
    }


    CloneSnapshotValid::CloneSnapshotValid()
	: CreateSnapshotEnvironment(),
	f_vg_name(LvmGeneralFixture::f_conf_lvm_vg_name),
	f_lv_name("lv_test_snapshot_01"),
	f_origin_name("lv_test_thin_1")  
    {
	std::cout << "CloneSnapshotValid ctor" << std::endl;

	// let boost handle the exception
	lvcreate_thin_snapshot_wrapper( f_vg_name, f_origin_name, f_lv_name );
    }

// TEST_VG_NAME=vg_test
// TEST_POOL_NAME=lv_test_pool
// TEST_THIN_LV_NAME=lv_test_thin
// TEST_VOLUME=/testsuite-import
// TEST_SNAP_01=lv_test_snapshot_01

    CloneSnapshotValid::~CloneSnapshotValid()
    {
	try {
	    lvremove_wrapper(f_vg_name, f_lv_name );
	}
	catch (const LvmImportTestsuiteException &e)
	{
	    std::cerr << "lvremove_wrapper( " << f_vg_name << ", " << f_lv_name << " ) failed" << std::endl;
	}

	try {
	    lvremove_wrapper(f_vg_name, f_lvm->snapshotLvName(f_num));
	}
	catch (const LvmImportTestsuiteException &e)
	{
	    std::cerr << "lvremove_wrapper( " << f_vg_name << ", " << f_lvm->snapshotLvName(f_num) << " ) failed" << std::endl;
	}
    }

    

    CloneSnapshotMissingOrigin::CloneSnapshotMissingOrigin()
	: f_vg_name("vg_test"), f_lv_name("lv_missing_snapshot_name")
    {
	std::cout << "CloneSnapshotMissingOrigin ctor" << std::endl;
    }

   

    MountSnapshotByDeviceValid::MountSnapshotByDeviceValid()
	: CreateSnapshotEnvironmentDirExists(), f_vg_name("vg_test"),
	  f_lv_name("lv_test_snapshot_01"), f_origin_name("lv_test_thin_1")
    {
	std::cout << "MountSnapshotByDeviceValid ctor" << std::endl;

	// let boost handle the exception
	lvcreate_thin_snapshot_wrapper( f_vg_name, f_origin_name, f_lv_name );

	std::ostringstream oss;
	oss << "/testsuite-import/.snapshots/" << f_num << "/snapshot";

	f_dev_path = "/dev/mapper/" + f_vg_name + "-" + f_lv_name;
	f_mountpoint = oss.str();
    }

    MountSnapshotByDeviceValid::~MountSnapshotByDeviceValid()
    {
	if (umount2(f_mountpoint.c_str(), MNT_DETACH))
	    std::cerr << "umount2( \"" << f_mountpoint << "\", MNT_DETACH) failed!" << std::endl;

	try
	{
	    lvremove_wrapper(f_vg_name, f_lv_name );
	}
	catch (const LvmImportTestsuiteException &e)
	{
	    std::cerr << "lvremove_wrapper( " << f_vg_name << ", " << f_lv_name << " ) failed" << std::endl;
	}
    }

    MountSnapshotByDeviceAlreadyMounted::MountSnapshotByDeviceAlreadyMounted()
	: MountSnapshotByDeviceValid()
    {
	std::cout << "MountSnapshotByDeviceAlreadyMounted ctor" << std::endl;
	
	string::size_type open = f_lvm->fstype().find("(");
	string::size_type close = f_lvm->fstype().find(")");
	string fstype = f_lvm->fstype().substr(open + 1, close - open - 1);

	int ret = mount(f_dev_path.c_str(), f_mountpoint.c_str(),
			fstype.c_str(),
			MS_NOATIME | MS_NODEV | MS_NOEXEC | MS_RDONLY,
			NULL);

	if (ret)
	{
	    perror("mount");
	    BOOST_FAIL( "Can't mount filesystem for testing purposes: \"" <<
			f_dev_path << "\" -> \"" << f_mountpoint << "\"");
	}
    }

    MountSnapshotByDeviceInvalidDevice::MountSnapshotByDeviceInvalidDevice()
	: CreateSnapshotEnvironmentDirExists()
    {
	std::cout << "MountSnapshotByDeviceInvalidDevice ctor" << std::endl;

	f_missing_dev_path = "/dev/mapper/this_device_do_not_exists";
    }

    CheckImportedSnapshotValid::CheckImportedSnapshotValid()
	: LvmGeneralFixture(), f_vg_name("vg_test"), f_lv_name("lv_test_snapshot_01"),
	  f_origin_name("lv_test_thin_1")
    {
	std::cout << "CheckImportedSnapshotValid ctor" << std::endl;

	// let boost handle the exception
	lvcreate_thin_snapshot_wrapper( f_vg_name, f_origin_name, f_lv_name );
    }

    CheckImportedSnapshotValid::~CheckImportedSnapshotValid()
    {
	try {
	    lvremove_wrapper(f_vg_name, f_lv_name );
	}
	catch (const LvmImportTestsuiteException &e)
	{
	    std::cerr << "lvremove_wrapper( " << f_vg_name << ", " << f_lv_name << " ) failed" << std::endl;
	}
    }

    CheckImportedSnapshotWrongVg::CheckImportedSnapshotWrongVg()
	: LvmGeneralFixture(), f_vg_name("vg_test_2"), f_lv_name("lv_test_thin_2")
    {
	std::cout << "CheckImportedSnapshotWrongVg ctor" << std::endl;
    }

    CheckImportedSnapshotVolumeImport::CheckImportedSnapshotVolumeImport()
	: LvmGeneralFixture(), f_vg_name(f_lvm->getVgName()), f_lv_name(f_lvm->getLvName())
    {
	std::cout << "CheckImportedSnapshotVolumeImport ctor" << std::endl;
    }

    CheckImportedSnapshotFsUuidMismatch::CheckImportedSnapshotFsUuidMismatch()
	: LvmGeneralFixture(), f_vg_name("vg_test"), f_lv_name("lv_test_snapshot_01"),
	  f_origin_name("lv_test_thin_1")
    {
	std::cout << "CheckImportedSnapshotFsUuidMismatch ctor" << std::endl;

	lvcreate_thin_snapshot_wrapper( f_vg_name, f_origin_name, f_lv_name, false);

	string::size_type open = f_lvm->fstype().find("(");
	string::size_type close = f_lvm->fstype().find(")");
	string fstype = f_lvm->fstype().substr(open + 1, close - open - 1);

	modify_fs_uuid(f_vg_name, f_lv_name, fstype);
    }

    CheckImportedSnapshotFsUuidMismatch::~CheckImportedSnapshotFsUuidMismatch()
    {
	lvremove_wrapper(f_vg_name, f_lv_name);
    }

    CheckImportedSnapshotNonThinLv::CheckImportedSnapshotNonThinLv()
	: LvmGeneralFixture(), f_vg_name("vg_test"), f_lv_name("lv_non_thin")
    {
	lvcreate_non_thin_lv_wrapper(f_vg_name, f_lv_name);
    }

    CheckImportedSnapshotNonThinLv::~CheckImportedSnapshotNonThinLv()
    {
	lvremove_wrapper(f_vg_name, f_lv_name);
    }

    DeleteSnapshotByVgLv::DeleteSnapshotByVgLv()
	: LvmGeneralFixture(), f_vg_name("vg_test"), f_lv_name("lv_test_volume"),
	f_origin_name("lv_test_thin_1")
    {
	lvcreate_thin_snapshot_wrapper(f_vg_name, f_origin_name, f_lv_name);
    }

    DeleteSnapshotByVgLv::~DeleteSnapshotByVgLv()
    {
	try
	{
	    lvremove_wrapper(f_vg_name, f_lv_name);
	}
	// in case of failure, remove volume that should be deleted by Lvm class
	catch (const LvmImportTestsuiteException& e) {}
    }

    DeleteSnapshotByVgLvMissing::DeleteSnapshotByVgLvMissing()
	: LvmGeneralFixture(), f_vg_name("vg_blahblah"), f_lv_name("lv_blahblah")
    {
    }

}}
