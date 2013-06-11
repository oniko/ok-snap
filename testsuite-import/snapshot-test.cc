#include <boost/scoped_ptr.hpp>
#include <boost/test/unit_test.hpp>

#include "testsuite-import/helpers.h"
#include "testsuite-import/snapshot-test.h"

namespace testsuiteimport { namespace lvm
{
    void SnapshotTestClass::tc_snapshot_simple_ctor()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FSimpleConstructorValid());
	fixture->test_method();
    }

    void SnapshotTestClass::tc_snapshot_import_ctor()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FImportConstructorValid());
	fixture->test_method();
    }

    void SnapshotTestClass::tc_snapshot_copy_ctor_no_import()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FCopyConstructorNoImport());
	fixture->test_method();
    }

    void SnapshotTestClass::tc_snapshot_copy_ctor_import()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FCopyConstructorImport());
	fixture->test_method();
    }

    void SnapshotTestClass::tc_delete_filesystem_snapshot_import_type_none()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FDeleteFilesystemSnapshotImportTypeNone());
	fixture->test_method();
    }

    void SnapshotTestClass::tc_delete_filesystem_snapshot_import_type_clone()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FDeleteFilesystemSnapshotImportTypeClone());
	fixture->test_method();
    }

    void SnapshotTestClass::tc_delete_filesystem_snapshot_import_type_adopt()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FDeleteFilesystemSnapshotImportTypeAdopt());
	fixture->test_method();
    }

    void SnapshotTestClass::tc_delete_filesystem_snapshot_import_type_ack()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FDeleteFilesystemSnapshotImportTypeAcknowledge());
	fixture->test_method();
    }

    void SnapshotTestClass::tc_delete_filesystem_snapshot_origin()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FDeleteFileSystemSnapshotOrigin());
	fixture->test_method();
    }

    void SnapshotTestClass::tc_mount_filesystem_snapshot_import_none_non_user_request()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FMountFileSystemSnapshotImportNone());
	fixture->test_method();
    }

    void SnapshotTestClass::tc_mount_filesystem_snapshot_import_none_user_request()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FMountFileSystemSnapshotImportNoneUserRequest());
	fixture->test_method();
    }

    void SnapshotTestClass::tc_mount_filesystem_snapshot_import_clone_non_user_request()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FMountFileSystemSnapshotImportClone());
	fixture->test_method();
    }

    void SnapshotTestClass::tc_mount_filesystem_snapshot_import_clone_user_request()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FMountFileSystemSnapshotImportCloneUserRequest());
	fixture->test_method();
    }

    void SnapshotTestClass::tc_mount_filesystem_snapshot_import_adopt_non_user_request()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FMountFileSystemSnapshotImportAdopt());
	fixture->test_method();
    }

    void SnapshotTestClass::tc_mount_filesystem_snapshot_import_adopt_user_request()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FMountFileSystemSnapshotImportAdoptUserRequest());
	fixture->test_method();
    }

    void SnapshotTestClass::tc_mount_filesystem_snapshot_import_ack_non_user_request()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FMountFileSystemSnapshotImportAck());
	fixture->test_method();
    }

    void SnapshotTestClass::tc_mount_filesystem_snapshot_import_ack_user_request()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FMountFileSystemSnapshotImportAckUserRequest());
	fixture->test_method();
    }

    void SnapshotTestClass::tc_umount_filesystem_snapshot_import_none_non_user_request()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FUmountFilesystemSnapshotImportNone());
	fixture->test_method();
    }

    void SnapshotTestClass::tc_umount_filesystem_snapshot_import_none_user_request()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FUmountFilesystemSnapshotImportNoneUserRequest());
	fixture->test_method();
    }

    void SnapshotTestClass::tc_umount_filesystem_snapshot_import_clone_non_user_request()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FUmountFilesystemSnapshotImportClone());
	fixture->test_method();
    }

    void SnapshotTestClass::tc_umount_filesystem_snapshot_import_clone_user_request()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FUmountFilesystemSnapshotImportCloneUserRequest());
	fixture->test_method();
    }

    void SnapshotTestClass::tc_umount_filesystem_snapshot_import_adopt_non_user_request()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FUmountFilesystemSnapshotImportAdopt());
	fixture->test_method();
    }

    void SnapshotTestClass::tc_umount_filesystem_snapshot_import_adopt_user_request()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FUmountFilesystemSnapshotImportAdoptUserRequest());
	fixture->test_method();
    }

    void SnapshotTestClass::tc_umount_filesystem_snapshot_import_ack_non_user_request()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FUmountFilesystemSnapshotImportAck());
	fixture->test_method();
    }

    void SnapshotTestClass::tc_umount_filesystem_snapshot_import_ack_user_request()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FUmountFilesystemSnapshotImportAckUserRequest());
	fixture->test_method();
    }

    void SnapshotTestClass::tc_umount_filesystem_snapshot_invalid()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FUmountFilesystemInvalid());
	fixture->test_method();
    }

    void SnapshotTestClass::tc_handle_umount_filesystem_snapshot_non_user_request()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FHandleUmountFilesystemSnapshot());
	fixture->test_method();
    }

    void SnapshotTestClass::tc_handle_umount_filesystem_snapshot_user_request()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FHandleUmountFilesystemSnapshotUserRequest());
	fixture->test_method();
    }

    void FSimpleConstructorValid::test_method()
    {
	boost::scoped_ptr<snapper::Snapshot> snapshot;

	BOOST_REQUIRE_NO_THROW( snapshot.reset(new snapper::Snapshot(f_dummy_snapper, f_type, f_num, f_date)) );

	BOOST_CHECK_EQUAL( snapshot->getType(), f_type );
	BOOST_CHECK_EQUAL( snapshot->getNum(), f_num );
	BOOST_CHECK_EQUAL( snapshot->getDate(), f_date );
	BOOST_CHECK_EQUAL( snapshot->getUid(), 0 );
	BOOST_CHECK_EQUAL( snapshot->getPreNum(), 0 );
	BOOST_CHECK_EQUAL( snapshot->getImportPolicy(), snapper::ImportPolicy::NONE );

	// check few private attributes (essential tests for further testing)
	BOOST_CHECK_EQUAL( snapshot->snapper, f_dummy_snapper );
	BOOST_CHECK( snapshot->p_idata == NULL );
    }

    void FImportConstructorValid::test_method()
    {
	boost::scoped_ptr<snapper::Snapshot> snapshot;

	BOOST_REQUIRE_NO_THROW( snapshot.reset(new snapper::Snapshot(f_dummy_snapper, f_type, f_num, f_date, f_import_policy, f_dummy_idata)) );

	BOOST_CHECK_EQUAL( snapshot->getType(), f_type );
	BOOST_CHECK_EQUAL( snapshot->getNum(), f_num );
	BOOST_CHECK_EQUAL( snapshot->getDate(), f_date );
	BOOST_CHECK_EQUAL( snapshot->getUid(), 0 );
	BOOST_CHECK_EQUAL( snapshot->getPreNum(), 0 );
	BOOST_CHECK_EQUAL( snapshot->getImportPolicy(), f_import_policy );

	// check few private attributes (essential tests for further testing)
	BOOST_CHECK_EQUAL( snapshot->snapper, f_dummy_snapper );
	BOOST_CHECK_EQUAL( snapshot->p_idata, f_dummy_idata );
    }

    void FCopyConstructorNoImport::test_method()
    {
	boost::scoped_ptr<snapper::Snapshot> copy;

	BOOST_REQUIRE_NO_THROW( copy.reset(new snapper::Snapshot(f_origin)) );

	BOOST_CHECK_EQUAL( copy->getType(), f_type );
	BOOST_CHECK_EQUAL( copy->getNum(), f_num );
	BOOST_CHECK_EQUAL( copy->getDate(), f_date );
	BOOST_CHECK_EQUAL( copy->getUid(), f_uid );
	BOOST_CHECK_EQUAL( copy->getPreNum(), f_pre_num );
	BOOST_CHECK_EQUAL( copy->getDescription(), f_description );
	BOOST_CHECK_EQUAL( copy->getCleanup(), f_cleanup );
 	BOOST_CHECK( copy->getUserdata() == f_userdata );

 	BOOST_CHECK_EQUAL( copy->getImportPolicy(), snapper::ImportPolicy::NONE );

	// check rest of private attributes
	BOOST_CHECK_EQUAL( copy->snapper, f_dummy_snapper );
	BOOST_CHECK_EQUAL( copy->info_modified, f_info_modified );
	BOOST_CHECK_EQUAL( copy->mount_checked, f_mount_checked );
	BOOST_CHECK_EQUAL( copy->mount_use_count, f_mount_use_count );
	BOOST_CHECK( copy->p_idata == NULL );
    }

    void FCopyConstructorImport::test_method()
    {
	boost::scoped_ptr<snapper::Snapshot> copy;

	BOOST_REQUIRE_NO_THROW( copy.reset(new snapper::Snapshot(*f_p_origin)) );
	//snapper::Snapshot copy(*f_p_origin);

	BOOST_CHECK_EQUAL( copy->getType(), f_type );
	BOOST_CHECK_EQUAL( copy->getNum(), f_num );
	BOOST_CHECK_EQUAL( copy->getDate(), f_date );
	BOOST_CHECK_EQUAL( copy->getUid(), f_uid );
	BOOST_CHECK_EQUAL( copy->getPreNum(), f_pre_num );
	BOOST_CHECK_EQUAL( copy->getDescription(), f_description );
	BOOST_CHECK_EQUAL( copy->getCleanup(), f_cleanup );
	BOOST_CHECK( copy->getUserdata() == f_userdata );

	BOOST_CHECK_EQUAL( copy->getImportPolicy(), f_import_policy );
	BOOST_CHECK_EQUAL( copy->snapper, f_dummy_snapper );
	BOOST_CHECK_EQUAL( copy->info_modified, f_info_modified );
	BOOST_CHECK_EQUAL( copy->mount_checked, f_mount_checked );
	BOOST_CHECK_EQUAL( copy->mount_use_count, f_mount_use_count );

	// TODO: think about removal. We don't want to test other class here
	// in case of extreme failure, this may fault assert, see ImportMetadata class
	BOOST_CHECK( copy->p_idata->isEqual(*f_p_idata) );
	// check if copy ctor has actually created a new copy on a new address
	BOOST_CHECK_NE( copy->p_idata, f_p_idata );
    }

    void FDeleteFilesystemSnapshotImportTypeNone::test_method()
    {
	BOOST_REQUIRE_NO_THROW( f_sh.deleteFilesystemSnapshot() );

	// TODO: Lvm dependant test here, move elsewhere
	BOOST_CHECK( !check_lv_exists(f_conf_vg_name, f_snapshot_lv_name) );
    }

    void FDeleteFilesystemSnapshotImportTypeClone::test_method()
    {
	BOOST_REQUIRE_NO_THROW( f_sh.deleteFilesystemSnapshot() );

	BOOST_CHECK( !check_lv_exists(f_conf_vg_name, f_snapshot_lv_name) );
    }

    void FDeleteFilesystemSnapshotImportTypeAdopt::test_method()
    {
	BOOST_REQUIRE_NO_THROW( f_sh.deleteFilesystemSnapshot() );

	BOOST_CHECK( !check_lv_exists(f_conf_vg_name, f_snapshot_lv_name) );
    }

    void FDeleteFilesystemSnapshotImportTypeAcknowledge::test_method()
    {
	BOOST_REQUIRE_NO_THROW( f_sh.deleteFilesystemSnapshot() );

	BOOST_CHECK( check_lv_exists(f_conf_vg_name, f_snapshot_lv_name) );
    }

    void FDeleteFileSystemSnapshotOrigin::test_method()
    {
	BOOST_CHECK_THROW( f_sh.deleteFilesystemSnapshot(), snapper::IllegalSnapshotException );

	BOOST_CHECK( check_lv_exists( f_conf_vg_name, f_conf_origin_name) );
    }

    void FMountFileSystemSnapshotImportNone::test_method()
    {
	unsigned int mount_count = f_sh.mount_use_count;

	BOOST_REQUIRE_NO_THROW( f_sh.mountFilesystemSnapshot(false) );

	BOOST_CHECK( check_is_mounted(f_conf_vg_name, f_snapshot_lv_name) );

	BOOST_CHECK_EQUAL( mount_count + 1, f_sh.mount_use_count );
    }

    void FMountFileSystemSnapshotImportNoneUserRequest::test_method()
    {
	unsigned int mount_count = f_sh.mount_use_count;

	BOOST_REQUIRE_NO_THROW( f_sh.mountFilesystemSnapshot(true) );

	BOOST_CHECK( check_is_mounted(f_conf_vg_name, f_snapshot_lv_name) );

	BOOST_REQUIRE_NO_THROW( f_sh.mountFilesystemSnapshot(true) );

	BOOST_CHECK_EQUAL( mount_count, f_sh.mount_use_count );
	BOOST_CHECK( f_sh.mount_user_request );
    }

    void FMountFileSystemSnapshotImportClone::test_method()
    {
	unsigned int mount_count = f_sh.mount_use_count;

	BOOST_REQUIRE_NO_THROW( f_sh.mountFilesystemSnapshot(false) );

	BOOST_CHECK( check_is_mounted(f_conf_vg_name, f_snapshot_lv_name) );
	BOOST_CHECK( !check_is_mounted(f_conf_vg_name, f_clone_origin_name) );

	BOOST_CHECK_EQUAL( mount_count + 1, f_sh.mount_use_count );
    }

    void FMountFileSystemSnapshotImportCloneUserRequest::test_method()
    {
	unsigned int mount_count = f_sh.mount_use_count;

	BOOST_REQUIRE_NO_THROW( f_sh.mountFilesystemSnapshot(true) );

	BOOST_CHECK( check_is_mounted(f_conf_vg_name, f_snapshot_lv_name) );
	BOOST_CHECK( !check_is_mounted(f_conf_vg_name, f_clone_origin_name) );

	BOOST_REQUIRE_NO_THROW( f_sh.mountFilesystemSnapshot(true) );

	BOOST_CHECK_EQUAL( mount_count, f_sh.mount_use_count );
	BOOST_CHECK( f_sh.mount_user_request );
    }

    void FMountFileSystemSnapshotImportAdopt::test_method()
    {
	unsigned int mount_count = f_sh.mount_use_count;

	BOOST_REQUIRE_NO_THROW( f_sh.mountFilesystemSnapshot(false) );

	BOOST_CHECK( check_is_mounted(f_conf_vg_name, f_snapshot_lv_name) );

	BOOST_CHECK_EQUAL( mount_count + 1, f_sh.mount_use_count );
    }

    void FMountFileSystemSnapshotImportAdoptUserRequest::test_method()
    {
	unsigned int mount_count = f_sh.mount_use_count;

	BOOST_REQUIRE_NO_THROW( f_sh.mountFilesystemSnapshot(true) );

	BOOST_CHECK( check_is_mounted(f_conf_vg_name, f_snapshot_lv_name) );

	BOOST_REQUIRE_NO_THROW( f_sh.mountFilesystemSnapshot(true) );

	BOOST_CHECK_EQUAL( mount_count, f_sh.mount_use_count );
	BOOST_CHECK( f_sh.mount_user_request );
    }

    void FMountFileSystemSnapshotImportAck::test_method()
    {
	unsigned int mount_count = f_sh.mount_use_count;

	BOOST_REQUIRE_NO_THROW( f_sh.mountFilesystemSnapshot(false) );

	BOOST_CHECK( check_is_mounted(f_conf_vg_name, f_snapshot_lv_name) );

	BOOST_CHECK_EQUAL( mount_count + 1, f_sh.mount_use_count );
    }

    void FMountFileSystemSnapshotImportAckUserRequest::test_method()
    {
	unsigned int mount_count = f_sh.mount_use_count;

	BOOST_REQUIRE_NO_THROW( f_sh.mountFilesystemSnapshot(true) );

	BOOST_CHECK( check_is_mounted(f_conf_vg_name, f_snapshot_lv_name) );

	BOOST_REQUIRE_NO_THROW( f_sh.mountFilesystemSnapshot(true) );

	BOOST_CHECK_EQUAL( mount_count, f_sh.mount_use_count );
	BOOST_CHECK( f_sh.mount_user_request );
    }

    void FUmountFilesystemSnapshotImportNone::test_method()
    {
	unsigned int mount_count = f_sh.mount_use_count;

	BOOST_REQUIRE_NO_THROW( f_sh.umountFilesystemSnapshot(false) );

	BOOST_CHECK_EQUAL( mount_count, f_sh.mount_use_count + 1);

	BOOST_CHECK( check_is_mounted(f_conf_vg_name, f_snapshot_lv_name) );
    }

    void FUmountFilesystemSnapshotImportNoneUserRequest::test_method()
    {
	unsigned int mount_count = f_sh.mount_use_count;

	BOOST_REQUIRE_NO_THROW( f_sh.umountFilesystemSnapshot(true) );

	BOOST_CHECK_EQUAL( mount_count, f_sh.mount_use_count );

	f_sh.mount_use_count = 0;

	BOOST_REQUIRE_NO_THROW( f_sh.umountFilesystemSnapshot(true) );

	BOOST_CHECK( !check_is_mounted(f_conf_vg_name, f_snapshot_lv_name) );
	BOOST_CHECK( !f_sh.mount_user_request );
    }

    void FUmountFilesystemSnapshotImportClone::test_method()
    {
	unsigned int mount_count = f_sh.mount_use_count;

	BOOST_REQUIRE_NO_THROW( f_sh.umountFilesystemSnapshot(false) );

	BOOST_CHECK_EQUAL( mount_count, f_sh.mount_use_count + 1);

	BOOST_CHECK( check_is_mounted(f_conf_vg_name, f_snapshot_lv_name) );
	BOOST_CHECK( check_is_mounted(f_conf_vg_name, f_clone_origin_name) );
    }

    void FUmountFilesystemSnapshotImportCloneUserRequest::test_method()
    {
	unsigned int mount_count = f_sh.mount_use_count;

	BOOST_REQUIRE_NO_THROW( f_sh.umountFilesystemSnapshot(true) );

	BOOST_CHECK_EQUAL( mount_count, f_sh.mount_use_count );
	BOOST_CHECK( check_is_mounted(f_conf_vg_name, f_snapshot_lv_name) );
	BOOST_CHECK( check_is_mounted(f_conf_vg_name, f_clone_origin_name) );

	mount_count = f_sh.mount_use_count = 0;

	BOOST_REQUIRE_NO_THROW( f_sh.umountFilesystemSnapshot(true) );

	BOOST_CHECK( !check_is_mounted(f_conf_vg_name, f_snapshot_lv_name) );
	// test if cloned origin is not affected by umount
	BOOST_CHECK( check_is_mounted(f_conf_vg_name, f_clone_origin_name) );
	BOOST_CHECK( !f_sh.mount_user_request );
    }

    void FUmountFilesystemSnapshotImportAdopt::test_method()
    {
	unsigned int mount_count = f_sh.mount_use_count;

	BOOST_REQUIRE_NO_THROW( f_sh.umountFilesystemSnapshot(false) );

	BOOST_CHECK_EQUAL( mount_count, f_sh.mount_use_count + 1);

	BOOST_CHECK( check_is_mounted(f_conf_vg_name, f_snapshot_lv_name) );
    }

    void FUmountFilesystemSnapshotImportAdoptUserRequest::test_method()
    {
	unsigned int mount_count = f_sh.mount_use_count;

	BOOST_REQUIRE_NO_THROW( f_sh.umountFilesystemSnapshot(true) );

	BOOST_CHECK_EQUAL( mount_count, f_sh.mount_use_count );

	f_sh.mount_use_count = 0;

	BOOST_REQUIRE_NO_THROW( f_sh.umountFilesystemSnapshot(true) );

	BOOST_CHECK( !check_is_mounted(f_conf_vg_name, f_snapshot_lv_name) );
	BOOST_CHECK( !f_sh.mount_user_request );
    }

    void FUmountFilesystemSnapshotImportAck::test_method()
    {
	unsigned int mount_count = f_sh.mount_use_count;

	BOOST_REQUIRE_NO_THROW( f_sh.umountFilesystemSnapshot(false) );

	BOOST_CHECK_EQUAL( mount_count, f_sh.mount_use_count + 1);

	BOOST_CHECK( check_is_mounted(f_conf_vg_name, f_snapshot_lv_name) );
    }

    void FUmountFilesystemSnapshotImportAckUserRequest::test_method()
    {
	unsigned int mount_count = f_sh.mount_use_count;

	BOOST_REQUIRE_NO_THROW( f_sh.umountFilesystemSnapshot(true) );

	BOOST_CHECK_EQUAL( mount_count, f_sh.mount_use_count );

	f_sh.mount_use_count = 0;

	BOOST_REQUIRE_NO_THROW( f_sh.umountFilesystemSnapshot(true) );

	BOOST_CHECK( !check_is_mounted(f_conf_vg_name, f_snapshot_lv_name) );
	BOOST_CHECK( !f_sh.mount_user_request );
    }

    void FUmountFilesystemInvalid::test_method()
    {
	BOOST_REQUIRE_THROW( f_sh.umountFilesystemSnapshot(true), snapper::IllegalSnapshotException );

	BOOST_CHECK( check_is_mounted(f_conf_vg_name, f_conf_origin_name ) );
    }

    void FHandleUmountFilesystemSnapshot::test_method()
    {
	f_sh.mount_user_request = false;
	f_sh.mount_checked = true;
	unsigned int mount_count = f_sh.mount_use_count = 1;

	BOOST_REQUIRE_NO_THROW( f_sh.handleUmountFilesystemSnapshot() );
	BOOST_REQUIRE( check_is_mounted(f_conf_vg_name, f_snapshot_lv_name) );
	BOOST_REQUIRE_EQUAL( mount_count, f_sh.mount_use_count );

	f_sh.mount_checked = false;

	BOOST_REQUIRE_NO_THROW( f_sh.handleUmountFilesystemSnapshot() );
	BOOST_REQUIRE( check_is_mounted(f_conf_vg_name, f_snapshot_lv_name) );
	BOOST_CHECK_EQUAL( mount_count, f_sh.mount_use_count );

	f_sh.mount_use_count = mount_count = 0;

	BOOST_REQUIRE_NO_THROW( f_sh.handleUmountFilesystemSnapshot() );
	BOOST_REQUIRE( check_is_mounted(f_conf_vg_name, f_snapshot_lv_name) );
	BOOST_REQUIRE_EQUAL( mount_count, f_sh.mount_use_count );

	f_sh.mount_checked = true;

	BOOST_REQUIRE_NO_THROW( f_sh.handleUmountFilesystemSnapshot() );
	BOOST_CHECK( !check_is_mounted(f_conf_vg_name, f_snapshot_lv_name) );
	BOOST_CHECK_EQUAL( mount_count, f_sh.mount_use_count );
    }

    void FHandleUmountFilesystemSnapshotUserRequest::test_method()
    {
	f_sh.mount_user_request = true;
	f_sh.mount_checked = true;
	unsigned int mount_count = f_sh.mount_use_count = 1;

	BOOST_REQUIRE_NO_THROW( f_sh.handleUmountFilesystemSnapshot() );
	BOOST_REQUIRE( check_is_mounted(f_conf_vg_name, f_snapshot_lv_name) );
	BOOST_REQUIRE_EQUAL( mount_count, f_sh.mount_use_count );

	f_sh.mount_checked = false;

	BOOST_REQUIRE_NO_THROW( f_sh.handleUmountFilesystemSnapshot() );
	BOOST_REQUIRE( check_is_mounted(f_conf_vg_name, f_snapshot_lv_name) );
	BOOST_REQUIRE_EQUAL( mount_count, f_sh.mount_use_count );

	f_sh.mount_use_count = mount_count = 0;

	BOOST_REQUIRE_NO_THROW( f_sh.handleUmountFilesystemSnapshot() );
	BOOST_REQUIRE( check_is_mounted(f_conf_vg_name, f_snapshot_lv_name) );
	BOOST_REQUIRE_EQUAL( mount_count, f_sh.mount_use_count );

	f_sh.mount_checked = true;

	BOOST_REQUIRE_NO_THROW( f_sh.handleUmountFilesystemSnapshot() );
	BOOST_CHECK( check_is_mounted(f_conf_vg_name, f_snapshot_lv_name) );
	BOOST_CHECK_EQUAL( mount_count, f_sh.mount_use_count );
    }
}}
