#include <boost/shared_ptr.hpp>

#include "testsuite-import/lvm-master-testsuite.h"
#include "testsuite-import/lvmimportmetadata-test.h"
#include "testsuite-import/lvm-test.h"
#include "testsuite-import/snapshot-test.h"
#include "testsuite-import/import-helper-testsuite.h"
#include "testsuite-import/lvm-ihelper-fixtures.h"

#include "snapper/Snapper.h"
#include "snapper/Logger.h"

#include <boost/test/unit_test.hpp>

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
	test_case* tc_import_compare = BOOST_CLASS_TEST_CASE( &LvmImportMetadataTestClass::tc_import_compare_metadata, lvmimportmetadata_test );
	test_case* tc_import_check = BOOST_CLASS_TEST_CASE( &LvmImportMetadataTestClass::tc_import_check_imported_snapshot, lvmimportmetadata_test );
	test_case* tc_import_clone = BOOST_CLASS_TEST_CASE( &LvmImportMetadataTestClass::tc_clone_imported_snapshot, lvmimportmetadata_test );
	test_case* tc_import_delete = BOOST_CLASS_TEST_CASE( &LvmImportMetadataTestClass::tc_delete_imported_snapshot, lvmimportmetadata_test );
	test_case* tc_import_raw_metadata = BOOST_CLASS_TEST_CASE( &LvmImportMetadataTestClass::tc_import_raw_metadata, lvmimportmetadata_test );
	test_case* tc_import_snapshot_dir = BOOST_CLASS_TEST_CASE( &LvmImportMetadataTestClass::tc_import_get_snapshot_dir, lvmimportmetadata_test );


	// dependencies
	tc_import_compare->depends_on(tc_import_ctor);
	tc_import_check->depends_on(tc_import_ctor);
	//tc_import_check->depends_on(lvm->check...);
	tc_import_clone->depends_on(tc_import_ctor);
	//tc_import_clone->depends_on(lvm->clone...);
	tc_import_delete->depends_on(tc_import_ctor);
	//tc_import_delete->depends_on(lvm->delete...);
	tc_import_raw_metadata->depends_on(tc_import_ctor);
	tc_import_snapshot_dir->depends_on(tc_import_ctor);
	//tc_import_snapshot_dir->depends_on(lvm->snapshotDir (missing test));


	add(tc_import_ctor);
	add(tc_import_compare);
	add(tc_import_check);
	add(tc_import_clone);
	add(tc_import_delete);
	add(tc_import_raw_metadata);
	add(tc_import_snapshot_dir);


	boost::shared_ptr<LvmTestClass> lvm_test(new LvmTestClass());

	// Lvm unit
	test_case* tc_create_snapshot_environment = BOOST_CLASS_TEST_CASE( &LvmTestClass::tc_create_snapshot_environment, lvm_test );
	test_case* tc_create_snapshot_fail_on_environment = BOOST_CLASS_TEST_CASE( &LvmTestClass::tc_create_snapshot_fail_on_environment, lvm_test );
	test_case* tc_clone_snapshot = BOOST_CLASS_TEST_CASE( &LvmTestClass::tc_clone_snapshot, lvm_test );
	test_case* tc_mount_snapshot_by_subvolume = BOOST_CLASS_TEST_CASE( &LvmTestClass::tc_mount_snapshot_by_subvolume, lvm_test );
	test_case* tc_check_imported_snapshot = BOOST_CLASS_TEST_CASE( &LvmTestClass::tc_check_imported_snapshot, lvm_test );
	test_case* tc_lvm_delete_snapshot_by_vg_lv = BOOST_CLASS_TEST_CASE( &LvmTestClass::tc_lvm_delete_snapshot_by_vg_lv, lvm_test );

	tc_create_snapshot_fail_on_environment->depends_on(tc_create_snapshot_environment);
	//tc_check_imported_snapshot->depends_on(lvm->is_sobvolume_ro);

	add(tc_create_snapshot_environment);
	add(tc_create_snapshot_fail_on_environment);
	add(tc_clone_snapshot);
	add(tc_mount_snapshot_by_subvolume);
	add(tc_check_imported_snapshot);
	add(tc_lvm_delete_snapshot_by_vg_lv);

	// Snapshot unit
	boost::shared_ptr<SnapshotTestClass> snapshot_test(new SnapshotTestClass(new LvmSnapshotFixtures()));

	test_case* tc_snapshot_ctor = BOOST_CLASS_TEST_CASE( &SnapshotTestClass::tc_snapshot_ctor, snapshot_test);
	test_case* tc_snapshot_get_import_policy = BOOST_CLASS_TEST_CASE( &SnapshotTestClass::tc_snapshot_get_import_policy, snapshot_test);
	test_case* tc_snapshot_get_snapshot_dir = BOOST_CLASS_TEST_CASE( &SnapshotTestClass::tc_snapshot_get_snapshot_dir, snapshot_test);
	test_case* tc_snapshot_mount_filesystem = BOOST_CLASS_TEST_CASE( &SnapshotTestClass::tc_snapshot_mount_filesystem, snapshot_test);

	tc_snapshot_ctor->depends_on(tc_import_ctor);

	tc_snapshot_get_import_policy->depends_on(tc_snapshot_ctor);

	tc_snapshot_get_snapshot_dir->depends_on(tc_import_snapshot_dir);

	tc_snapshot_mount_filesystem->depends_on(tc_snapshot_ctor);
	tc_snapshot_mount_filesystem->depends_on(tc_mount_snapshot_by_subvolume);

	add(tc_snapshot_ctor);
	add(tc_snapshot_get_import_policy);
	add(tc_snapshot_get_snapshot_dir);
	add(tc_snapshot_mount_filesystem);

	test_suite* ts_import_helper_testsuite = new ImportCheckerTestSuite("lvm", NULL);

	// TODO: add lot of dependencies here
	ts_import_helper_testsuite->depends_on(tc_clone_snapshot);

	add(ts_import_helper_testsuite);
    }


    LvmImportTestsuite::~LvmImportTestsuite()
    {
	snapper::Snapper::deleteConfig("testsuite-import");
    }
}}
