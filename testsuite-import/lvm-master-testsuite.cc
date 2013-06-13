#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>

//#include "config.h"

#include "testsuite-import/lvm-master-testsuite.h"
#include "testsuite-import/lvmimportmetadata-test.h"
#include "testsuite-import/lvm-test.h"
#include "testsuite-import/snapshot-test.h"

//#define private public
//#define protected public
    #include "snapper/Snapper.h"
//#undef private
//#undef protected

#include "snapper/Logger.h"

namespace testsuiteimport { namespace lvm
{
    using boost::unit_test::test_case;

    LvmImportTestsuite::LvmImportTestsuite()
	: test_suite("LVM Testsuite")
    {
	snapper::setLogQuery((snapper::LogQuery) &false_log_query);
	snapper::Snapper::createConfig("testsuite-import", "/testsuite-import", "lvm(ext4)", "default");

	boost::shared_ptr<LvmImportMetadataTestClass> lvmimportmetadata_test(new LvmImportMetadataTestClass());

	// LvmImportMetadata unit
	test_case* tc_import_ctor = BOOST_CLASS_TEST_CASE( &LvmImportMetadataTestClass::tc_import_ctor, lvmimportmetadata_test );
	//test_case* tc_import_copy_ctor = BOOST_CLASS_TEST_CASE( &LvmImportMetadataTestClass::tc_import_copy_ctor, lvmimportmetadata_test );
	//test_case* tc_import_clone_method = BOOST_CLASS_TEST_CASE( &LvmImportMetadataTestClass::tc_import_clone_method, lvmimportmetadata_test );
	test_case* tc_import_equal_method_true = BOOST_CLASS_TEST_CASE( &LvmImportMetadataTestClass::tc_import_equal_method_true, lvmimportmetadata_test );
	test_case* tc_import_equal_method_false = BOOST_CLASS_TEST_CASE( &LvmImportMetadataTestClass::tc_import_equal_method_false, lvmimportmetadata_test );

	//tc_import_copy_ctor->depends_on(tc_import_ctor);
	//tc_import_clone_method->depends_on(tc_import_copy_ctor);
	//tc_import_equal_method_true->depends_on(tc_import_copy_ctor);
	//tc_import_equal_method_false->depends_on(tc_import_copy_ctor);

	add(tc_import_ctor);
	//add(tc_import_copy_ctor);
	//add(tc_import_clone_method);
	add(tc_import_equal_method_true);
	add(tc_import_equal_method_false);

	boost::shared_ptr<LvmTestClass> lvm_test(new LvmTestClass());

	// Lvm unit
	test_case* tc_create_snapshot_environment = BOOST_CLASS_TEST_CASE( &LvmTestClass::tc_create_snapshot_environment, lvm_test );
	test_case* tc_create_snapshot_environment_dir_exists = BOOST_CLASS_TEST_CASE( &LvmTestClass::tc_create_snapshot_environment_dir_exists, lvm_test );
	test_case* tc_create_snapshot_environment_failure = BOOST_CLASS_TEST_CASE( &LvmTestClass::tc_create_snapshot_environment_failure, lvm_test );
	test_case* tc_create_snapshot_fail_on_environment = BOOST_CLASS_TEST_CASE( &LvmTestClass::tc_create_snapshot_fail_on_environment, lvm_test );
	test_case* tc_clone_snapshot = BOOST_CLASS_TEST_CASE( &LvmTestClass::tc_clone_snapshot, lvm_test );
	test_case* tc_clone_snapshot_missing_origin = BOOST_CLASS_TEST_CASE( &LvmTestClass::tc_clone_snapshot_missing_origin, lvm_test );
	test_case* tc_mount_snapshot_by_device = BOOST_CLASS_TEST_CASE( &LvmTestClass::tc_mount_snapshot_by_device, lvm_test );
	test_case* tc_mount_snapshot_by_device_already_mounted = BOOST_CLASS_TEST_CASE( &LvmTestClass::tc_mount_snapshot_by_device_already_mounted, lvm_test );
	test_case* tc_mount_snapshot_by_device_missing_device = BOOST_CLASS_TEST_CASE( &LvmTestClass::tc_mount_snapshot_by_device_missing_device, lvm_test );
	test_case* tc_check_imported_snapshot = BOOST_CLASS_TEST_CASE( &LvmTestClass::tc_check_imported_snapshot, lvm_test );
	test_case* tc_check_imported_snapshot_wrong_vg = BOOST_CLASS_TEST_CASE( &LvmTestClass::tc_check_imported_snapshot_wrong_vg, lvm_test );
	test_case* tc_check_imported_snapshot_volume_import = BOOST_CLASS_TEST_CASE( &LvmTestClass::tc_check_imported_snapshot_volume_import, lvm_test );
	test_case* tc_check_imported_snapshot_fs_uuid_mismatch = BOOST_CLASS_TEST_CASE( &LvmTestClass::tc_check_imported_snapshot_fs_uuid_mismatch, lvm_test );
	test_case* tc_check_imported_snapshot_non_thin_lv = BOOST_CLASS_TEST_CASE( &LvmTestClass::tc_check_imported_snapshot_non_thin_lv, lvm_test );
	test_case* tc_lvm_delete_snapshot_by_vg_lv = BOOST_CLASS_TEST_CASE( &LvmTestClass::tc_lvm_delete_snapshot_by_vg_lv, lvm_test );
	test_case* tc_lvm_delete_snapshot_by_vg_lv_missing = BOOST_CLASS_TEST_CASE( &LvmTestClass::tc_lvm_delete_snapshot_by_vg_lv_missing, lvm_test );

	add(tc_create_snapshot_environment);
	add(tc_create_snapshot_environment_dir_exists);
	add(tc_create_snapshot_environment_failure);
	add(tc_create_snapshot_fail_on_environment);
	add(tc_clone_snapshot);
	add(tc_clone_snapshot_missing_origin);
	add(tc_mount_snapshot_by_device);
	add(tc_mount_snapshot_by_device_already_mounted);
	add(tc_mount_snapshot_by_device_missing_device);
	add(tc_check_imported_snapshot);
	add(tc_check_imported_snapshot_wrong_vg);
	add(tc_check_imported_snapshot_volume_import);
	add(tc_check_imported_snapshot_fs_uuid_mismatch, 1);
	add(tc_check_imported_snapshot_non_thin_lv);
	add(tc_lvm_delete_snapshot_by_vg_lv);
	add(tc_lvm_delete_snapshot_by_vg_lv_missing);

	// Snapshot unit
	// TODO: think about splitting down fs dependant tests
	boost::shared_ptr<SnapshotTestClass> snapshot_test(new SnapshotTestClass());

	test_case* tc_snapshot_simple_ctor = BOOST_CLASS_TEST_CASE( &SnapshotTestClass::tc_snapshot_simple_ctor, snapshot_test);
	test_case* tc_snapshot_import_ctor = BOOST_CLASS_TEST_CASE( &SnapshotTestClass::tc_snapshot_import_ctor, snapshot_test);
	//test_case* tc_snapshot_copy_ctor_no_import = BOOST_CLASS_TEST_CASE( &SnapshotTestClass::tc_snapshot_copy_ctor_no_import, snapshot_test);
	//test_case* tc_snapshot_copy_ctor_import = BOOST_CLASS_TEST_CASE( &SnapshotTestClass::tc_snapshot_copy_ctor_import, snapshot_test);
	test_case* tc_delete_filesystem_snapshot_import_type_none = BOOST_CLASS_TEST_CASE( &SnapshotTestClass::tc_delete_filesystem_snapshot_import_type_none, snapshot_test);
	test_case* tc_delete_filesystem_snapshot_import_type_clone = BOOST_CLASS_TEST_CASE( &SnapshotTestClass::tc_delete_filesystem_snapshot_import_type_clone, snapshot_test);
	test_case* tc_delete_filesystem_snapshot_import_type_adopt = BOOST_CLASS_TEST_CASE( &SnapshotTestClass::tc_delete_filesystem_snapshot_import_type_adopt, snapshot_test);
	test_case* tc_delete_filesystem_snapshot_import_type_ack = BOOST_CLASS_TEST_CASE( &SnapshotTestClass::tc_delete_filesystem_snapshot_import_type_ack, snapshot_test);
	test_case* tc_delete_filesystem_snapshot_origin = BOOST_CLASS_TEST_CASE( &SnapshotTestClass::tc_delete_filesystem_snapshot_origin, snapshot_test);
	test_case* tc_mount_filesystem_snapshot_import_none_non_user_request = BOOST_CLASS_TEST_CASE( &SnapshotTestClass::tc_mount_filesystem_snapshot_import_none_non_user_request, snapshot_test);
	test_case* tc_mount_filesystem_snapshot_import_none_user_request = BOOST_CLASS_TEST_CASE( &SnapshotTestClass::tc_mount_filesystem_snapshot_import_none_user_request, snapshot_test);
	test_case* tc_mount_filesystem_snapshot_import_clone_non_user_request = BOOST_CLASS_TEST_CASE( &SnapshotTestClass::tc_mount_filesystem_snapshot_import_clone_non_user_request, snapshot_test);
	test_case* tc_mount_filesystem_snapshot_import_clone_user_request = BOOST_CLASS_TEST_CASE( &SnapshotTestClass::tc_mount_filesystem_snapshot_import_clone_user_request, snapshot_test);
	test_case* tc_mount_filesystem_snapshot_import_adopt_non_user_request = BOOST_CLASS_TEST_CASE( &SnapshotTestClass::tc_mount_filesystem_snapshot_import_adopt_non_user_request, snapshot_test);
	test_case* tc_mount_filesystem_snapshot_import_adopt_user_request = BOOST_CLASS_TEST_CASE( &SnapshotTestClass::tc_mount_filesystem_snapshot_import_adopt_user_request, snapshot_test);
	test_case* tc_mount_filesystem_snapshot_import_ack_non_user_request = BOOST_CLASS_TEST_CASE( &SnapshotTestClass::tc_mount_filesystem_snapshot_import_ack_non_user_request, snapshot_test);
	test_case* tc_mount_filesystem_snapshot_import_ack_user_request = BOOST_CLASS_TEST_CASE( &SnapshotTestClass::tc_mount_filesystem_snapshot_import_ack_user_request, snapshot_test);
	test_case* tc_umount_filesystem_snapshot_import_none_non_user_request = BOOST_CLASS_TEST_CASE( &SnapshotTestClass::tc_umount_filesystem_snapshot_import_none_non_user_request, snapshot_test);
	test_case* tc_umount_filesystem_snapshot_import_none_user_request = BOOST_CLASS_TEST_CASE( &SnapshotTestClass::tc_umount_filesystem_snapshot_import_none_user_request, snapshot_test);
	test_case* tc_umount_filesystem_snapshot_import_clone_non_user_request = BOOST_CLASS_TEST_CASE( &SnapshotTestClass::tc_umount_filesystem_snapshot_import_clone_non_user_request, snapshot_test);
	test_case* tc_umount_filesystem_snapshot_import_clone_user_request = BOOST_CLASS_TEST_CASE( &SnapshotTestClass::tc_umount_filesystem_snapshot_import_clone_user_request, snapshot_test);
	test_case* tc_umount_filesystem_snapshot_import_adopt_non_user_request = BOOST_CLASS_TEST_CASE( &SnapshotTestClass::tc_umount_filesystem_snapshot_import_adopt_non_user_request, snapshot_test);
	test_case* tc_umount_filesystem_snapshot_import_adopt_user_request = BOOST_CLASS_TEST_CASE( &SnapshotTestClass::tc_umount_filesystem_snapshot_import_adopt_user_request, snapshot_test);
	test_case* tc_umount_filesystem_snapshot_import_ack_non_user_request = BOOST_CLASS_TEST_CASE( &SnapshotTestClass::tc_umount_filesystem_snapshot_import_ack_non_user_request, snapshot_test);
	test_case* tc_umount_filesystem_snapshot_import_ack_user_request = BOOST_CLASS_TEST_CASE( &SnapshotTestClass::tc_umount_filesystem_snapshot_import_ack_user_request, snapshot_test);
	test_case* tc_umount_filesystem_snapshot_invalid = BOOST_CLASS_TEST_CASE( &SnapshotTestClass::tc_umount_filesystem_snapshot_invalid, snapshot_test);
	test_case* tc_handle_umount_filesystem_snapshot_non_user_request = BOOST_CLASS_TEST_CASE( &SnapshotTestClass::tc_handle_umount_filesystem_snapshot_non_user_request, snapshot_test);
	test_case* tc_handle_umount_filesystem_snapshot_user_request = BOOST_CLASS_TEST_CASE( &SnapshotTestClass::tc_handle_umount_filesystem_snapshot_user_request, snapshot_test);

//	tc_snapshot_copy_ctor_no_import->depends_on(tc_snapshot_simple_ctor);
//	tc_snapshot_copy_ctor_import->depends_on(tc_snapshot_import_ctor);
	tc_delete_filesystem_snapshot_import_type_none->depends_on(tc_snapshot_simple_ctor);

	tc_delete_filesystem_snapshot_import_type_clone->depends_on(tc_snapshot_import_ctor);

	tc_delete_filesystem_snapshot_import_type_adopt->depends_on(tc_import_ctor);
	tc_delete_filesystem_snapshot_import_type_adopt->depends_on(tc_lvm_delete_snapshot_by_vg_lv);

	tc_delete_filesystem_snapshot_import_type_ack->depends_on(tc_snapshot_import_ctor);

	add(tc_snapshot_simple_ctor);
	add(tc_snapshot_import_ctor);
//	add(tc_snapshot_copy_ctor_no_import);
//	add(tc_snapshot_copy_ctor_import);
	add(tc_delete_filesystem_snapshot_import_type_none);
	add(tc_delete_filesystem_snapshot_import_type_clone);
	add(tc_delete_filesystem_snapshot_import_type_adopt);
	add(tc_delete_filesystem_snapshot_import_type_ack);
	add(tc_delete_filesystem_snapshot_origin);
	add(tc_mount_filesystem_snapshot_import_none_non_user_request);
	add(tc_mount_filesystem_snapshot_import_none_user_request);
	add(tc_mount_filesystem_snapshot_import_clone_non_user_request);
	add(tc_mount_filesystem_snapshot_import_clone_user_request);
	add(tc_mount_filesystem_snapshot_import_adopt_non_user_request);
	add(tc_mount_filesystem_snapshot_import_adopt_user_request);
	add(tc_mount_filesystem_snapshot_import_ack_non_user_request);
	add(tc_mount_filesystem_snapshot_import_ack_user_request);
	add(tc_umount_filesystem_snapshot_import_none_non_user_request);
	add(tc_umount_filesystem_snapshot_import_none_user_request);
	add(tc_umount_filesystem_snapshot_import_clone_non_user_request);
	add(tc_umount_filesystem_snapshot_import_clone_user_request);
	add(tc_umount_filesystem_snapshot_import_adopt_non_user_request);
	add(tc_umount_filesystem_snapshot_import_adopt_user_request);
	add(tc_umount_filesystem_snapshot_import_ack_non_user_request);
	add(tc_umount_filesystem_snapshot_import_ack_user_request);
	add(tc_umount_filesystem_snapshot_invalid);
	add(tc_handle_umount_filesystem_snapshot_non_user_request);
	add(tc_handle_umount_filesystem_snapshot_user_request);
	
    }

    LvmImportTestsuite::~LvmImportTestsuite()
    {
	snapper::Snapper::deleteConfig("testsuite-import");
    }
}}
