#include <boost/test/unit_test.hpp>
#include <boost/scoped_ptr.hpp>

#include "testsuite-import/helpers.h"
#include "testsuite-import/lvm-test.h"
#include "testsuite-import/general-test.h"
#include "testsuite-import/lvm-fixtures.h"

namespace testsuiteimport { namespace lvm
{
    void LvmTestClass::tc_create_snapshot_environment()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FCreateSnapshotEnvironment());
	fixture->test_method();
    }

    void LvmTestClass::tc_create_snapshot_environment_dir_exists()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FCreateSnapshotEnvironmentDirExists());
	fixture->test_method();
    }

    void FCreateSnapshotEnvironment::test_method()
    {
	BOOST_REQUIRE_NO_THROW( f_lvm->createSnapshotEnvironment(f_num) );

	struct stat buff;

	BOOST_CHECK_EQUAL( fstatat(f_dirfd, "snapshot", &buff, AT_SYMLINK_NOFOLLOW), 0 );
	BOOST_CHECK_NE( S_ISDIR(buff.st_mode), 0 );
    }

    void FCreateSnapshotEnvironmentDirExists::test_method()
    {
	BOOST_REQUIRE_NO_THROW( f_lvm->createSnapshotEnvironment(f_num) );

	struct stat buff;

	BOOST_CHECK_EQUAL( fstatat(f_dirfd, "snapshot", &buff, AT_SYMLINK_NOFOLLOW), 0 );
	BOOST_CHECK_NE( S_ISDIR(buff.st_mode), 0 );
    }

    void FCreateSnapshotEnvironmentFailure::test_method()
    {
	BOOST_CHECK_THROW( f_lvm->createSnapshotEnvironment(f_num), snapper::CreateSnapshotFailedException );
    }

    void FCreateSnapshotFailOnEnvironment::test_method()
    {
	BOOST_REQUIRE_THROW( f_lvm->createSnapshot(f_num), snapper::CreateSnapshotFailedException );

	BOOST_CHECK( !check_lv_exists(f_conf_vg_name, f_lvm->snapshotLvName(f_num)) );
    }

    void FCloneSnapshotValid::test_method()
    {
	BOOST_REQUIRE_NO_THROW( f_lvm->cloneSnapshot(f_num, f_vg_name, f_lv_name ) );

	BOOST_REQUIRE( check_lv_exists( f_vg_name, lvm->snapshotLvName(f_num)) );
	BOOST_CHECK( check_is_thin( f_vg_name, lvm->snapshotLvName(f_num)) );
    }
    
    void FCloneSnapshotMissingOrigin::test_method()
    {
	BOOST_REQUIRE_THROW( f_lvm->cloneSnapshot(f_num, f_vg_name, f_lv_name ), snapper::ImportSnapshotFailedException );

	BOOST_CHECK( !check_lv_exists( f_vg_name, f_lvm->snapshotLvName(f_num)) );
    }
    
    void FMountSnapshotByDeviceValid::test_method()
    {
	BOOST_REQUIRE_NO_THROW( f_lvm->mountSnapshot(f_num, f_dev_path) );

	BOOST_CHECK( check_is_mounted(f_vg_name, f_lv_name) );
    }

    FMountSnapshotByDeviceAlreadyMounted::FMountSnapshotByDeviceAlreadyMounted()
	: MountSnapshotByDeviceValid()
    {
	std::cout << "MountSnapshotByDeviceAlreadyMounted ctor" << std::endl;

	int ret = mount(f_dev_path.c_str(), f_mountpoint.c_str(),
			f_lvm->mount_type.c_str(),
			MS_NOATIME | MS_NODEV | MS_NOEXEC | MS_RDONLY,
			NULL);

	if (ret)
	{
	    perror("mount");
	    BOOST_FAIL( "Can't mount filesystem for testing purposes: \"" <<
			dev_path << "\" -> \"" << mountpoint << "\"");
	}
    }
    
    void FMountSnapshotByDeviceAlreadyMounted::test_method()
    {
	BOOST_REQUIRE_NO_THROW ( f_lvm->mountSnapshot(f_num, f_dev_path ) );

	BOOST_CHECK( check_is_mounted(f_vg_name, f_lv_name) );
    }

    FMountSnapshotByDeviceInvalidDevice::FMountSnapshotByDeviceInvalidDevice()
	: CreateSnapshotEnvironmentDirExists()
    {
	std::cout << "MountSnapshotByDeviceInvalidDevice ctor" << std::endl;

	f_missing_dev_path = "/dev/mapper/this_device_do_not_exists";
    }

    void FMountSnapshotByDeviceInvalidDevice::test_method()
    {
	BOOST_CHECK_THROW ( f_lvm->mountSnapshot(f_num, f_missing_dev_path), snapper::MountSnapshotFailedException );
    }

    FCheckImportedSnapshotValid::FCheckImportedSnapshotValid()
	: LvmGeneralFixture(), f_vg_name("vg_test"), f_lv_name("lv_test_snapshot_01"),
	  f_origin_name("lv_test_thin_1")
    {
	std::cout << "CheckImportedSnapshotValid ctor" << std::endl;

	// let boost handle the exception
	lvcreate_thin_snapshot_wrapper( f_vg_name, f_origin_name, f_lv_name );
    }

    FCheckImportedSnapshotValid::~FCheckImportedSnapshotValid()
    {
	try {
	    lvremove_wrapper(f_vg_name, f_lv_name );
	}
	catch (const LvmImportTestsuiteException &e)
	{
	    std::cerr << "lvremove_wrapper( " << f_vg_name << ", " << f_lv_name << " ) failed" << std::endl;
	}
    }

    void FCheckImportedSnapshotValid::test_method()
    {
	BOOST_CHECK( f_lvm->checkImportedSnapshot(f_vg_name, f_lv_name) );
    }

    FCheckImportedSnapshotWrongVg::FCheckImportedSnapshotWrongVg()
	: LvmGeneralFixture(), f_vg_name("vg_test_2"), f_lv_name("lv_test_thin_2")
    {
	std::cout << "CheckImportedSnapshotWrongVg ctor" << std::endl;
    }

    void FCheckImportedSnapshotWrongVg::test_method()
    {
	BOOST_CHECK( !f_lvm->checkImportedSnapshot(f_vg_name, f_lv_name) );
    }

    void FCheckImportedSnapshotVolumeImport::test_method()
    {
	BOOST_CHECK( !lvm->checkImportedSnapshot(f_vg_name, f_lv_name) );
    }

    
    
}}
