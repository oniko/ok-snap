#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>

//#include "config.h"

#include "testsuite-import/lvm-master-testsuite.h"
#include "testsuite-import/lvmimportmetadata-test.h"
#include "testsuite-import/lvm-test.h"

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
	test_case* tc_import_copy_ctor = BOOST_CLASS_TEST_CASE( &LvmImportMetadataTestClass::tc_import_copy_ctor, lvmimportmetadata_test );
	test_case* tc_import_clone_method = BOOST_CLASS_TEST_CASE( &LvmImportMetadataTestClass::tc_import_clone_method, lvmimportmetadata_test );
	test_case* tc_import_equal_method_true = BOOST_CLASS_TEST_CASE( &LvmImportMetadataTestClass::tc_import_equal_method_true, lvmimportmetadata_test );
	test_case* tc_import_equal_method_false = BOOST_CLASS_TEST_CASE( &LvmImportMetadataTestClass::tc_import_equal_method_false, lvmimportmetadata_test );

	tc_import_copy_ctor->depends_on(tc_import_ctor);
	tc_import_clone_method->depends_on(tc_import_copy_ctor);
	tc_import_equal_method_true->depends_on(tc_import_copy_ctor);
	tc_import_equal_method_false->depends_on(tc_import_copy_ctor);

	add(tc_import_ctor);
	add(tc_import_copy_ctor);
	add(tc_import_clone_method);
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
	test_case* tc_check_delete_snapshot_by_vg_lv = BOOST_CLASS_TEST_CASE( &LvmTestClass::tc_check_delete_snapshot_by_vg_lv, lvm_test );
	test_case* tc_check_delete_snapshot_by_vg_lv_missing = BOOST_CLASS_TEST_CASE( &LvmTestClass::tc_check_delete_snapshot_by_vg_lv_missing, lvm_test );

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
	add(tc_check_delete_snapshot_by_vg_lv);
	add(tc_check_delete_snapshot_by_vg_lv_missing);

	
    }

    LvmImportTestsuite::~LvmImportTestsuite()
    {
	snapper::Snapper::deleteConfig("testsuite-import");
    }
}}
