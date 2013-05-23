#ifndef UNIT_LVM_H
#define UNIT_LVM_H

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <ostream>
#include <string>

#include <boost/test/unit_test.hpp>

#include "testsuite-import/helpers.h"

/*
 * Lvm.cc should create directories where snapshots
 * will be mounted to
 */
BOOST_FIXTURE_TEST_CASE( tc_create_snapshot_environment, CreateSnapshotEnvironment )
{
    BOOST_CHECK_NO_THROW( lvm->createSnapshotEnvironment(num) );

    struct stat buff;

    BOOST_CHECK_EQUAL( fstatat(dirfd, "snapshot", &buff, AT_SYMLINK_NOFOLLOW), 0 );
    BOOST_CHECK_NE( S_ISDIR(buff.st_mode), 0 );
}

BOOST_FIXTURE_TEST_CASE( tc_create_snapshot_environment_dir_exists, CreateSnapshotEnvironmentDirExists )
{
    BOOST_CHECK_NO_THROW( lvm->createSnapshotEnvironment(num) );

    struct stat buff;

    BOOST_CHECK_EQUAL( fstatat(dirfd, "snapshot", &buff, AT_SYMLINK_NOFOLLOW), 0 );
    BOOST_CHECK_NE( S_ISDIR(buff.st_mode), 0 );
}

/*
 * should throw exception in case 'snapshot' dentry exists and it's actually not a directory
 */
BOOST_FIXTURE_TEST_CASE( tc_create_snapshot_environment_failure, CreateSnapshotEnvironmentFailure )
{
    BOOST_CHECK_THROW( lvm->createSnapshotEnvironment(num), snapper::CreateSnapshotFailedException );
}

BOOST_AUTO_TEST_CASE ( tc_remove_snapshot_environment )
{
    // nothing to test right now
    BOOST_CHECK_EQUAL( true, true );
}

BOOST_FIXTURE_TEST_CASE ( tc_clone_snapshot, CloneSnapshotValid )
{
    // TODO: create global config file in testsuite-import directory maybe
    BOOST_CHECK_NO_THROW( lvm->cloneSnapshot(num, f_vg_name, f_lv_name ) );

    BOOST_CHECK( check_lv_exists( f_vg_name, lvm->snapshotLvName(num)) );
    BOOST_CHECK( check_is_thin( f_vg_name, lvm->snapshotLvName(num)) );
}

BOOST_FIXTURE_TEST_CASE ( tc_clone_snapshot_missing_origin, CloneSnapshotMissingOrigin )
{
    BOOST_CHECK_THROW( lvm->cloneSnapshot(num, f_vg_name, f_lv_name ), snapper::ImportSnapshotFailedException );

    BOOST_CHECK_EQUAL( check_lv_exists( f_vg_name, lvm->snapshotLvName(num)), false );
}

BOOST_FIXTURE_TEST_CASE ( tc_mount_snapshot_by_device, MountSnapshotByDeviceValid )
{
    BOOST_CHECK_NO_THROW( lvm->mountSnapshot(num, dev_path) );

    BOOST_CHECK( check_is_mounted(f_vg_name, f_lv_name) );
}

BOOST_FIXTURE_TEST_CASE ( tc_mount_snapshot_by_device_already_mounted, MountSnapshotByDeviceAlreadyMounted )
{
    BOOST_CHECK_NO_THROW ( lvm->mountSnapshot(num, dev_path ) );

    BOOST_CHECK( check_is_mounted(f_vg_name, f_lv_name) );
}

BOOST_FIXTURE_TEST_CASE ( tc_mount_snapshot_by_device_missing_device, MountSnapshotByDeviceInvalidDevice )
{
    BOOST_CHECK_THROW ( lvm->mountSnapshot(num, missing_dev_path), snapper::MountSnapshotFailedException );
}

// NOTE: following tests are important

BOOST_FIXTURE_TEST_CASE ( tc_check_imported_snapshot, CheckImportedSnapshotValid )
{
    BOOST_CHECK( lvm->checkImportedSnapshot(f_vg_name, f_lv_name) );
}

/*
 * NOTE: For the time being, we don't want snapper to accept snapshots
 *	 originating in different VG or different pool than configured
 * 	 volume.
 */
BOOST_FIXTURE_TEST_CASE ( tc_check_imported_snapshot_wrong_vg, CheckImportedSnapshotWrongVg )
{
    BOOST_CHECK( !lvm->checkImportedSnapshot(f_vg_name, f_lv_name) );
}

// NOTE: We _can't_ allow snapper importing its own volume!!!
BOOST_FIXTURE_TEST_CASE ( tc_check_imported_snapshot_volume_import, CheckImportedSnapshotVolumeImport )
{
    BOOST_CHECK( !lvm->checkImportedSnapshot(f_vg_name, f_lv_name) );
}

/*
 * NOTE: this test assume snapper shouldn't allow importing snapshot
 * with different fs_uuid from original volume.
 *
 * NOTE: 1 expected failure. Possibly move this test to Snapshot unit
 */
BOOST_AUTO_TEST_CASE_EXPECTED_FAILURES ( tc_check_imported_snapshot_fs_uuid_mismatch, 1 )

BOOST_FIXTURE_TEST_CASE ( tc_check_imported_snapshot_fs_uuid_mismatch, CheckImportedSnapshotFsUuidMismatch )
{
    BOOST_CHECK( !lvm->checkImportedSnapshot(f_vg_name, f_lv_name) );
}

BOOST_FIXTURE_TEST_CASE ( tc_check_imported_snapshot_non_thin_lv, CheckImportedSnapshotNonThinLv )
{
    BOOST_CHECK( !lvm->checkImportedSnapshot(f_vg_name, f_lv_name) );
}

BOOST_FIXTURE_TEST_CASE ( tc_check_delete_snapshot_by_vg_lv, DeleteSnapshotByVgLv )
{
    BOOST_CHECK_NO_THROW( lvm->deleteSnapshot(f_vg_name, f_lv_name) );

    BOOST_CHECK( !check_lv_exists(f_vg_name, f_lv_name) );
}

BOOST_FIXTURE_TEST_CASE ( tc_check_delete_snapshot_by_vg_lv_missing, DeleteSnapshotByVgLvMissing )
{
    BOOST_CHECK_THROW( lvm->deleteSnapshot(f_vg_name, f_lv_name), snapper::DeleteSnapshotFailedException );
}

#endif
