#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <boost/test/unit_test.hpp>
#include <boost/scoped_ptr.hpp>

#include "testsuite-import/helpers.h"
#include "testsuite-import/lvm-test.h"
#include "testsuite-import/general-test.h"
#include "testsuite-import/lvm-fixtures.h"

namespace testsuiteimport { namespace lvm
{
    void
    LvmTestClass::tc_create_snapshot_environment()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FCreateSnapshotEnvironment());
	fixture->test_method();
    }


    void
    LvmTestClass::tc_create_snapshot_fail_on_environment()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FCreateSnapshotFailOnEnvironment());
	fixture->test_method();
    }


    void
    LvmTestClass::tc_remove_snapshot_environment()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FRemoveSnapshotEnvironment());
	fixture->test_method();
    }


    void
    LvmTestClass::tc_clone_snapshot()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FCloneSnapshot());
	fixture->test_method();
    }


    void
    LvmTestClass::tc_mount_snapshot_by_subvolume()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FMountSnapshotBySubvolume());
	fixture->test_method();
    }


    void LvmTestClass::tc_check_imported_snapshot()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FCheckImportedSnapshot());
	fixture->test_method();
    }


    void LvmTestClass::tc_lvm_delete_snapshot_by_vg_lv()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FDeleteSnapshotByVgLv());
	fixture->test_method();
    }


    void FCreateSnapshotEnvironment::test_method()
    {
	BOOST_REQUIRE_NO_THROW( f_lvm->createSnapshotEnvironment(f_valid_info_dir.f_num) );

	struct stat buff;

	BOOST_CHECK_EQUAL( fstatat(f_valid_info_dir.get_dirfd(), "snapshot", &buff, AT_SYMLINK_NOFOLLOW), 0 );
	BOOST_CHECK_NE( S_ISDIR(buff.st_mode), 0 );

	// Dir Exists
	BOOST_REQUIRE_NO_THROW( f_lvm->createSnapshotEnvironment(f_info_snapshot_dir_exists.f_num) );

	BOOST_CHECK_EQUAL( fstatat(f_info_snapshot_dir_exists.get_dirfd(), "snapshot", &buff, AT_SYMLINK_NOFOLLOW), 0 );
	BOOST_CHECK_NE( S_ISDIR(buff.st_mode), 0 );

	// file in place of Info dir
	BOOST_CHECK_THROW( f_lvm->createSnapshotEnvironment(f_invalid_snapshot_dir.f_num), snapper::CreateSnapshotEnvirnomentException );
    }


    void FCreateSnapshotFailOnEnvironment::test_method()
    {
	BOOST_REQUIRE_THROW( f_lvm->createSnapshot(f_invalid_snap_dir.f_num), snapper::CreateSnapshotFailedException );

	BOOST_CHECK_EQUAL( check_lv_exists(LvmGeneralFixture::f_conf_lvm_vg_name, f_lvm->snapshotLvName(f_invalid_snap_dir.f_num)), false );
    }


    void FRemoveSnapshotEnvironment::test_method()
    {
	BOOST_REQUIRE_NO_THROW( f_lvm->removeSnapshotEnvironment(f_snapshot_dir.f_num) );

	struct stat buf;
	BOOST_REQUIRE_NE( fstatat(f_snapshot_dir.get_dirfd(), "snapshot", &buf, AT_SYMLINK_NOFOLLOW), 0);
	BOOST_CHECK_EQUAL( errno, ENOENT );
    }


    void FCloneSnapshot::test_method()
    {
	// valid
	BOOST_REQUIRE_NO_THROW( f_lvm->cloneSnapshot(f_info_dir_1.f_num, f_valid_subvolume.vg_name, f_valid_subvolume.lv_name ) );

	BOOST_REQUIRE( check_lv_exists( f_valid_subvolume.vg_name, f_lvm->snapshotLvName(f_info_dir_1.f_num)) );
	BOOST_CHECK( check_is_thin( f_valid_subvolume.vg_name, f_lvm->snapshotLvName(f_info_dir_1.f_num)) );

	// missing
	BOOST_REQUIRE_THROW( f_lvm->cloneSnapshot(f_info_dir_2.f_num, f_missing_lv_vg, f_missing_lv_lv ), snapper::ImportSnapshotFailedException );
	BOOST_CHECK( !check_lv_exists( f_valid_subvolume.vg_name, f_lvm->snapshotLvName(f_info_dir_2.f_num)) );
    }


    void
    FMountSnapshotBySubvolume::test_method()
    {
	BOOST_REQUIRE_EQUAL( f_mount_volume.is_mounted(), false );
	BOOST_REQUIRE_NO_THROW( f_lvm->mountSnapshot(f_snapshot_dir_1.f_num, f_mount_volume.devicepath()) );
	BOOST_REQUIRE_EQUAL( f_mount_volume.is_mounted(), true );

	// test mounting already mounted snopshet
	BOOST_REQUIRE_NO_THROW( f_lvm->mountSnapshot(f_snapshot_dir_1.f_num, f_mount_volume.devicepath()) );
	BOOST_REQUIRE_EQUAL( check_is_mounted(f_mount_volume.vg_name, f_mount_volume.lv_name), true );

	BOOST_CHECK_THROW( f_lvm->mountSnapshot(f_snapshot_dir_2.f_num, f_missing_dev_path), snapper::MountSnapshotFailedException );
    }


    void
    FCheckImportedSnapshot::test_method()
    {
	BOOST_CHECK_EQUAL( f_lvm->checkImportedSnapshot(f_missing_vg_vg, f_missing_vg_lv, true), false );
	BOOST_CHECK_EQUAL( f_lvm->checkImportedSnapshot(f_missing_vg_vg, f_missing_vg_lv, false), false );

	BOOST_CHECK_EQUAL( f_lvm->checkImportedSnapshot(f_missing_lv_vg, f_missing_lv_lv, true), false );
	BOOST_CHECK_EQUAL( f_lvm->checkImportedSnapshot(f_missing_lv_vg, f_missing_lv_lv, false), false );

	BOOST_CHECK_EQUAL( f_lvm->checkImportedSnapshot(f_nonthin_subvolume.vg_name, f_nonthin_subvolume.lv_name, true), false );
	BOOST_CHECK_EQUAL( f_lvm->checkImportedSnapshot(f_nonthin_subvolume.vg_name, f_nonthin_subvolume.lv_name, false), false );

	// 2 expected failures
	BOOST_CHECK_EQUAL( f_lvm->checkImportedSnapshot(f_foreign_vg_subvolume.vg_name, f_foreign_vg_subvolume.lv_name, true), false );
	BOOST_CHECK_EQUAL( f_lvm->checkImportedSnapshot(f_foreign_vg_subvolume.vg_name, f_foreign_vg_subvolume.lv_name, false), false );

	BOOST_CHECK_EQUAL( f_lvm->checkImportedSnapshot(f_current_subvolume_vg_name, f_current_subvolume_lv_name, true), false );
	BOOST_CHECK_EQUAL( f_lvm->checkImportedSnapshot(f_current_subvolume_vg_name, f_current_subvolume_lv_name, false), false );

	BOOST_CHECK_EQUAL( f_lvm->checkImportedSnapshot(f_rw_wrong_fs_uuid_subvolume.vg_name, f_rw_wrong_fs_uuid_subvolume.lv_name, true), false );
	BOOST_CHECK_EQUAL( f_lvm->checkImportedSnapshot(f_rw_wrong_fs_uuid_subvolume.vg_name, f_rw_wrong_fs_uuid_subvolume.lv_name, false), false );

	BOOST_CHECK_EQUAL( f_lvm->checkImportedSnapshot(f_rw_subvolume.vg_name, f_rw_subvolume.lv_name, true), false );
	BOOST_CHECK_EQUAL( f_lvm->checkImportedSnapshot(f_rw_subvolume.vg_name, f_rw_subvolume.lv_name, false), true );

	BOOST_CHECK_EQUAL( f_lvm->checkImportedSnapshot(f_ro_wrong_fs_uuid_subvolume.vg_name, f_ro_wrong_fs_uuid_subvolume.lv_name, true), false );
	BOOST_CHECK_EQUAL( f_lvm->checkImportedSnapshot(f_ro_wrong_fs_uuid_subvolume.vg_name, f_ro_wrong_fs_uuid_subvolume.lv_name, false), false );

	BOOST_CHECK_EQUAL( f_lvm->checkImportedSnapshot(f_ro_subvolume.vg_name, f_ro_subvolume.lv_name, true), true );
	BOOST_CHECK_EQUAL( f_lvm->checkImportedSnapshot(f_ro_subvolume.vg_name, f_ro_subvolume.lv_name, false), true );
    }


    void FDeleteSnapshotByVgLv::test_method()
    {
	BOOST_CHECK_THROW( f_lvm->deleteSnapshot(f_missing_vg_vg, f_missing_vg_lv), snapper::DeleteSnapshotFailedException );
	BOOST_CHECK_THROW( f_lvm->deleteSnapshot(f_missing_lv_vg, f_missing_lv_lv), snapper::DeleteSnapshotFailedException );

	BOOST_REQUIRE_NO_THROW( f_lvm->deleteSnapshot(f_subvolume.vg_name, f_subvolume.lv_name) );
	BOOST_CHECK_EQUAL( check_lv_exists(f_subvolume.vg_name, f_subvolume.lv_name), false );
    }
}}
