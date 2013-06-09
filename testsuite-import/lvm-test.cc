#include <fcntl.h>
//#include <sys/types.h>
#include <sys/stat.h>
//#include <unistd.h>

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

    void LvmTestClass::tc_create_snapshot_environment_failure()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FCreateSnapshotEnvironmentFailure());
	fixture->test_method();
    }

    void LvmTestClass::tc_create_snapshot_fail_on_environment()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FCreateSnapshotFailOnEnvironment());
	fixture->test_method();
    }

    void LvmTestClass::tc_clone_snapshot()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FCloneSnapshotValid());
	fixture->test_method();
    }

    void LvmTestClass::tc_clone_snapshot_missing_origin()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FCloneSnapshotMissingOrigin());
	fixture->test_method();
    }

    void LvmTestClass::tc_mount_snapshot_by_device()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FMountSnapshotByDeviceValid());
	fixture->test_method();
    }

    void LvmTestClass::tc_mount_snapshot_by_device_already_mounted()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FMountSnapshotByDeviceAlreadyMounted());
	fixture->test_method();
    }

    void LvmTestClass::tc_mount_snapshot_by_device_missing_device()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FMountSnapshotByDeviceInvalidDevice());
	fixture->test_method();
    }

    void LvmTestClass::tc_check_imported_snapshot()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FCheckImportedSnapshotValid());
	fixture->test_method();
    }

    void LvmTestClass::tc_check_imported_snapshot_wrong_vg()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FCheckImportedSnapshotWrongVg());
	fixture->test_method();
    }

    void LvmTestClass::tc_check_imported_snapshot_volume_import()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FCheckImportedSnapshotVolumeImport());
	fixture->test_method();
    }

    void LvmTestClass::tc_check_imported_snapshot_fs_uuid_mismatch()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FCheckImportedSnapshotFsUuidMismatch());
	fixture->test_method();
    }

    void LvmTestClass::tc_check_imported_snapshot_non_thin_lv()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FCheckImportedSnapshotNonThinLv());
	fixture->test_method();
    }

    void LvmTestClass::tc_check_delete_snapshot_by_vg_lv()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FDeleteSnapshotByVgLv());
	fixture->test_method();
    }

    void LvmTestClass::tc_check_delete_snapshot_by_vg_lv_missing()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FDeleteSnapshotByVgLvMissing());
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

	BOOST_REQUIRE( check_lv_exists( f_vg_name, f_lvm->snapshotLvName(f_num)) );
	BOOST_CHECK( check_is_thin( f_vg_name, f_lvm->snapshotLvName(f_num)) );
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

    void FMountSnapshotByDeviceAlreadyMounted::test_method()
    {
	BOOST_REQUIRE_NO_THROW ( f_lvm->mountSnapshot(f_num, f_dev_path ) );

	BOOST_CHECK( check_is_mounted(f_vg_name, f_lv_name) );
    }

    void FMountSnapshotByDeviceInvalidDevice::test_method()
    {
	BOOST_CHECK_THROW ( f_lvm->mountSnapshot(f_num, f_missing_dev_path), snapper::MountSnapshotFailedException );
    }

    void FCheckImportedSnapshotValid::test_method()
    {
	BOOST_CHECK( f_lvm->checkImportedSnapshot(f_vg_name, f_lv_name) );
    }

    void FCheckImportedSnapshotFsUuidMismatch::test_method()
    {
	BOOST_CHECK( !f_lvm->checkImportedSnapshot(f_vg_name, f_lv_name) );
    }

    void FCheckImportedSnapshotNonThinLv::test_method()
    {
	BOOST_CHECK( !f_lvm->checkImportedSnapshot(f_vg_name, f_lv_name) );
    }

    void FCheckImportedSnapshotWrongVg::test_method()
    {
	BOOST_CHECK( !f_lvm->checkImportedSnapshot(f_vg_name, f_lv_name) );
    }

    void FCheckImportedSnapshotVolumeImport::test_method()
    {
	BOOST_CHECK( !f_lvm->checkImportedSnapshot(f_vg_name, f_lv_name) );
    }

    void FDeleteSnapshotByVgLv::test_method()
    {
	BOOST_REQUIRE_NO_THROW( f_lvm->deleteSnapshot(f_vg_name, f_lv_name) );

	BOOST_CHECK( !check_lv_exists(f_vg_name, f_lv_name) );
    }

    void FDeleteSnapshotByVgLvMissing::test_method()
    {
	BOOST_CHECK_THROW( f_lvm->deleteSnapshot(f_vg_name, f_lv_name), snapper::DeleteSnapshotFailedException );
    }
}}
