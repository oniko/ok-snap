#include <boost/test/unit_test.hpp>
#include <boost/scoped_ptr.hpp>

#include "testsuite-import/snapshot-fixtures.h"
#include "testsuite-import/helpers.h"

// FIXME: Move this test case outside Lvm dependant unit tests
BOOST_FIXTURE_TEST_CASE ( tc_snapshot_simple_ctor, SimpleConstructorValid )
{
    snapper::Snapshot snapshot(f_dummy_snapper, f_type, f_num, f_date);

    BOOST_CHECK_EQUAL( snapshot.getType(), f_type );
    BOOST_CHECK_EQUAL( snapshot.getNum(), f_num );
    BOOST_CHECK_EQUAL( snapshot.getDate(), f_date );
    BOOST_CHECK_EQUAL( snapshot.getUid(), 0 );
    BOOST_CHECK_EQUAL( snapshot.getPreNum(), 0 );
    BOOST_CHECK_EQUAL( snapshot.getImportPolicy(), snapper::ImportPolicy::NONE );

    // check few private attributes (essential tests for further testing)
    BOOST_CHECK_EQUAL( snapshot.snapper, f_snapper );
    BOOST_CHECK_EQUAL( snapshot.p_idata, NULL );
}

// FIXME: Move this test case outside Lvm dependant unit tests
BOOST_FIXTURE_TEST_CASE ( tc_snapshot_import_ctor, ImportConstructorValid )
{
    snapper::Snapshot snapshot(f_dummy_snapper, f_type, f_num, f_date, f_import_policy, f_p_idata);

    BOOST_CHECK_EQUAL( snapshot.getType(), f_type );
    BOOST_CHECK_EQUAL( snapshot.getNum(), f_num );
    BOOST_CHECK_EQUAL( snapshot.getDate(), f_date );
    BOOST_CHECK_EQUAL( snapshot.getUid(), 0 );
    BOOST_CHECK_EQUAL( snapshot.getPreNum(), 0 );
    BOOST_CHECK_EQUAL( snapshot.getImportPolicy(), f_import_policy );

    // check few private attributes (essential tests for further testing)
    BOOST_CHECK_EQUAL( snapshot.snapper, f_snapper );
    BOOST_CHECK_EQUAL( snapshot.p_idata, f_p_idata );

    snapshot.p_idata = NULL;
}

// FIXME: Move this test case outside Lvm dependant unit tests
// TODO: depends on 'tc_snapshot_simple_constructor'
BOOST_FIXTURE_TEST_CASE ( tc_snapshot_copy_ctor_no_import, CopyConstructorNoImport )
{
    snapper::Snapshot copy(f_origin);

    BOOST_CHECK_EQUAL( copy.getType(), f_type );
    BOOST_CHECK_EQUAL( copy.getNum(), f_num );
    BOOST_CHECK_EQUAL( copy.getDate(), f_date );
    BOOST_CHECK_EQUAL( copy.getUid(), f_uid );
    BOOST_CHECK_EQUAL( copy.getPreNum(), f_pre_num );
    BOOST_CHECK_EQUAL( copy.getDescription(), f_description );
    BOOST_CHECK_EQUAL( copy.getCleanup(), f_cleanup );
    BOOST_CHECK_EQUAL( copy.getUserdata(), f_userdata );

    BOOST_CHECK_EQUAL( copy.getImportPolicy(), snapper::ImportPolicy::NONE );

    // check rest of private attributes
    BOOST_CHECK_EQUAL( copy.snapper, f_dummy_snapper );
    BOOST_CHECK_EQUAL( copy.info_modified, f_info_modified );
    BOOST_CHECK_EQUAL( copy.mount_checked, f_mount_checked );
    BOOST_CHECK_EQUAL( copy.mount_use_count, f_mount_use_count );
    BOOST_CHECK_EQUAL( copy.p_idata, NULL );
}

// TODO: depends on LvmImportMetadata ctor, Snapshot import ctor, LvmImportMetadata isEqualImpl
BOOST_FIXTURE_TEST_CASE ( tc_snapshot_copy_ctor_import, CopyConstructorImport )
{
    snapper::Snapshot copy(*f_p_origin);

    BOOST_CHECK_EQUAL( copy.getType(), f_type );
    BOOST_CHECK_EQUAL( copy.getNum(), f_num );
    BOOST_CHECK_EQUAL( copy.getDate(), f_date );
    BOOST_CHECK_EQUAL( copy.getUid(), f_uid );
    BOOST_CHECK_EQUAL( copy.getPreNum(), f_pre_num );
    BOOST_CHECK_EQUAL( copy.getDescription(), f_description );
    BOOST_CHECK_EQUAL( copy.getCleanup(), f_cleanup );
    BOOST_CHECK_EQUAL( copy.getUserdata(), f_userdata );

    BOOST_CHECK_EQUAL( copy.getImportPolicy(), f_import_policy );
    BOOST_CHECK_EQUAL( copy.snapper, f_snapper );
    BOOST_CHECK_EQUAL( copy.info_modified, f_info_modified );
    BOOST_CHECK_EQUAL( copy.mount_checked, f_mount_checked );
    BOOST_CHECK_EQUAL( copy.mount_use_count, f_mount_use_count );

    // in case of extreme failure, this may fault assert, see ImportMetadata class
    BOOST_CHECK( copy.p_idata->isEqual(f_p_idata) );
    // check if copy ctor has actually created a new copy on a new address
    BOOST_CHECK_NE( copy.p_idata, f_p_idata );
}

// TODO: depends on Snapshot ctor no import
BOOST_FIXTURE_TEST_CASE ( tc_delete_filesystem_snapshot_import_type_none, DeleteFilesystemSnapshotImportTypeNone )
{
    BOOST_CHECK_NO_THROW( f_sh->deleteFilesystemSnapshot() );

    BOOST_CHECK( !check_lv_exists(f_conf_vg_name, f_snapshot_name) );
}

BOOST_FIXTURE_TEST_CASE ( tc_delete_filesystem_snapshot_import_type_clone, DeleteFilesystemSnapshotImportTypeClone )
{
    BOOST_CHECK_NO_THROW( f_sh->deleteFilesystemSnapshot() );

    BOOST_CHECK( !check_lv_exists(f_conf_vg_name, f_snapshot_name) );
}

BOOST_FIXTURE_TEST_CASE ( tc_delete_filesystem_snapshot_import_type_adopt, DeleteFilesystemSnapshotImportTypeAdopt )
{
    BOOST_CHECK_NO_THROW( f_sh->deleteFilesystemSnapshot() );

    BOOST_CHECK( !check_lv_exists(f_conf_vg_name, f_snapshot_name) );
}

BOOST_FIXTURE_TEST_CASE ( tc_delete_filesystem_snapshot_import_type_ack, DeleteFilesystemSnapshotImportTypeAcknowledge )
{
    BOOST_CHECK_NO_THROW( f_sh->deleteFilesystemSnapshot() );

    BOOST_CHECK( check_lv_exists(f_conf_vg_name, f_snapshot_name) );
}

BOOST_FIXTURE_TEST_CASE ( tc_delete_filesystem_snapshot_origin, DeleteFileSystemSnapshotOrigin )
{
    BOOST_CHECK_THROW( f_sh.deleteFilesystemSnapshot(), snapper::IllegalSnapshotException );

    BOOST_CHECK( check_lv_exists( f_conf_vg_name, f_conf_origin_name) );
}

BOOST_FIXTURE_TEST_CASE ( tc_mount_filesystem_snapshot_import_none_non_user_request, MountFileSystemSnapshotImportNone )
{
    unsigned int mount_count = f_sh.mount_use_count;

    BOOST_CHECK_NO_THROW( f_sh.mountFilesystemSnapshot(false) );

    BOOST_CHECK ( check_is_mounted(f_vg_name, f_snapshot_lv_name) );

    BOOST_CHECK_EQUAL ( mount_count + 1, f_sh.mount_use_count );
}

BOOST_FIXTURE_TEST_CASE ( tc_mount_filesystem_snapshot_import_none_user_request, MountFileSystemSnapshotImportNone )
{
    unsigned int mount_count = f_sh.mount_use_count;

    BOOST_CHECK_NO_THROW( f_sh.mountFilesystemSnapshot(true) );

    BOOST_CHECK ( check_is_mounted(f_vg_name, f_snapshot_lv_name) );

    BOOST_CHECK_NO_THROW( f_sh.mountFilesystemSnapshot(true) );

    BOOST_CHECK_EQUAL ( mount_count, f_sh.mount_use_count );
    BOOST_CHECK ( f_sh.mount_user_request );
}

BOOST_FIXTURE_TEST_CASE ( tc_mount_filesystem_snapshot_import_clone_non_user_request, MountFileSystemSnapshotImportClone )
{
    unsigned int mount_count = f_sh.mount_use_count;

    BOOST_CHECK_NO_THROW( f_sh.mountFilesystemSnapshot(false) );

    BOOST_CHECK ( check_is_mounted(f_vg_name, f_snapshot_lv_name) );
    BOOST_CHECK ( !check_is_mounted(f_vg_name, f_clone_origin_name) );

    BOOST_CHECK_EQUAL ( mount_count + 1, f_sh.mount_use_count );
}

BOOST_FIXTURE_TEST_CASE ( tc_mount_filesystem_snapshot_import_clone_user_request, MountFileSystemSnapshotImportClone )
{
    unsigned int mount_count = f_sh.mount_use_count;

    BOOST_CHECK_NO_THROW( f_sh.mountFilesystemSnapshot(true) );

    BOOST_CHECK ( check_is_mounted(f_vg_name, f_snapshot_lv_name) );
    BOOST_CHECK ( !check_is_mounted(f_vg_name, f_clone_origin_name) );

    BOOST_CHECK_NO_THROW( f_sh.mountFilesystemSnapshot(true) );

    BOOST_CHECK_EQUAL ( mount_count, f_sh.mount_use_count );
    BOOST_CHECK ( f_sh.mount_user_request );
}

BOOST_FIXTURE_TEST_CASE ( tc_mount_filesystem_snapshot_import_adopt_non_user_request, MountFileSystemSnapshotImportAdoptOrAck )
{
    unsigned int mount_count = f_sh.mount_use_count;

    BOOST_CHECK_NO_THROW( f_sh.mountFilesystemSnapshot(false) );

    BOOST_CHECK ( check_is_mounted(f_vg_name, f_snapshot_lv_name) );

    BOOST_CHECK_EQUAL ( mount_count + 1, f_sh.mount_use_count );
}

BOOST_FIXTURE_TEST_CASE ( tc_mount_filesystem_snapshot_import_adopt_user_request, MountFileSystemSnapshotImportAdoptOrAck )
{
    unsigned int mount_count = f_sh.mount_use_count;

    BOOST_CHECK_NO_THROW( f_sh.mountFilesystemSnapshot(true) );

    BOOST_CHECK ( check_is_mounted(f_vg_name, f_snapshot_lv_name) );

    BOOST_CHECK_NO_THROW( f_sh.mountFilesystemSnapshot(true) );

    BOOST_CHECK_EQUAL ( mount_count, f_sh.mount_use_count );
    BOOST_CHECK ( f_sh.mount_user_request );
}

BOOST_FIXTURE_TEST_CASE ( tc_mount_filesystem_snapshot_import_ack_non_user_request, MountFileSystemSnapshotImportAdoptOrAck )
{
    unsigned int mount_count = f_sh.mount_use_count;

    BOOST_CHECK_NO_THROW( f_sh.mountFilesystemSnapshot(false) );

    BOOST_CHECK ( check_is_mounted(f_vg_name, f_snapshot_lv_name) );

    BOOST_CHECK_EQUAL ( mount_count + 1, f_sh.mount_use_count );
}

BOOST_FIXTURE_TEST_CASE ( tc_mount_filesystem_snapshot_import_ack_user_request, MountFileSystemSnapshotImportAdoptOrAck )
{
    unsigned int mount_count = f_sh.mount_use_count;

    BOOST_CHECK_NO_THROW( f_sh.mountFilesystemSnapshot(true) );

    BOOST_CHECK ( check_is_mounted(f_vg_name, f_snapshot_lv_name) );

    BOOST_CHECK_NO_THROW( f_sh.mountFilesystemSnapshot(true) );

    BOOST_CHECK_EQUAL ( mount_count, f_sh.mount_use_count );
    BOOST_CHECK ( f_sh.mount_user_request );
}

BOOST_FIXTURE_TEST_CASE ( tc_umount_filesystem_snapshot_import_none_non_user_request, UmountFilesystemSnapshotImportNone )
{
    unsigned int mount_count = f_sh.mount_count = 1;

    BOOST_CHECK_NO_THROW( f_sh.umountFilesystemSnapshot(false) );

    BOOST_CHECK_EQUAL( mount_count - 1, f_sh.mount_count );

    BOOST_CHECK( check_is_mounted(f_vg_name, f_snapshot_lv_name) );
}

BOOST_FIXTURE_TEST_CASE ( tc_umount_filesystem_snapshot_import_none_non_user_request, UmountFilesystemSnapshotImportNone )
{
    unsigned int mount_count = f_sh.mount_count = 1;

    BOOST_CHECK_NO_THROW( f_sh.umountFilesystemSnapshot(true) );

    BOOST_CHECK_EQUAL( mount_count, f_sh.mount_count );

    mount_count = f_sh.mount_count = 0;

    BOOST_CHECK( !check_is_mounted(f_vg_name, f_snapshot_lv_name) );
    BOOST_CHECK( !f_sh.mount_user_request );
}

