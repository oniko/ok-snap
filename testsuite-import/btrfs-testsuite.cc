#include "testsuite-import/btrfs-testsuite.h"
#include "testsuite-import/btrfsimportmetadata-test.h"

#include "snapper/Snapper.h"

namespace testsuiteimport { namespace btrfs
{
    using boost::unit_test::test_case;

    BtrfsImportTestsuite::BtrfsImportTestsuite()
	: test_suite("btrfs testsuite import")
    {
	snapper::setLogQuery((snapper::LogQuery) &b_false_log_query);
	snapper::Snapper::createConfig("testsuite-import-btrfs", "/testsuite-import-btrfs", "btrfs", "default");

	boost::shared_ptr<BtrfsImportMetadataTestClass> btrfsimportmetadata_test(new BtrfsImportMetadataTestClass());

	// void tc_import_compare_metadata();

	/*
	 * NOTE: depends on:
	 *			- ctor
	 * 			- Btrfs::checkImportedSnapshot()
	 */
	//void tc_import_check_imported_snapshot();
	/*
	 * NOTE: depends on:
	 * 			- ctor
	 * 			- Btrfs::deleteImportedSnapshot()
	 */
	//void tc_import_delete_snapshot();
	// NOTE: depends on:	- ctor
	//void tc_import_get_raw_metadata();
	// NOTE: depends on:	- ctor
	// void tc_import_get_snapshot_dir();

	test_case* tc_import_ctor = BOOST_CLASS_TEST_CASE( &BtrfsImportMetadataTestClass::tc_import_ctor, btrfsimportmetadata_test );
	test_case* tc_import_compare_metadata = BOOST_CLASS_TEST_CASE( &BtrfsImportMetadataTestClass::tc_import_compare_metadata, btrfsimportmetadata_test );
	test_case* tc_import_check_imported_snapshot = BOOST_CLASS_TEST_CASE( &BtrfsImportMetadataTestClass::tc_import_check_imported_snapshot, btrfsimportmetadata_test );
	test_case* tc_import_delete_snapshot = BOOST_CLASS_TEST_CASE( &BtrfsImportMetadataTestClass::tc_import_delete_snapshot, btrfsimportmetadata_test );
	test_case* tc_import_get_raw_metadata = BOOST_CLASS_TEST_CASE( &BtrfsImportMetadataTestClass::tc_import_get_raw_metadata, btrfsimportmetadata_test );
	test_case* tc_import_get_snapshot_dir = BOOST_CLASS_TEST_CASE( &BtrfsImportMetadataTestClass::tc_import_get_snapshot_dir, btrfsimportmetadata_test );

	tc_import_compare_metadata->depends_on(tc_import_ctor);
	// TODO: add dependecy for Btrfs::checkImportedSnapshot() test
	tc_import_check_imported_snapshot->depends_on(tc_import_ctor);
	// TODO: add dependecy for Btrfs::deleteImportedSnapshot() test
	tc_import_delete_snapshot->depends_on(tc_import_ctor);
	tc_import_get_raw_metadata->depends_on(tc_import_ctor);
	tc_import_get_snapshot_dir->depends_on(tc_import_ctor);

	add(tc_import_ctor);
	add(tc_import_compare_metadata);
	add(tc_import_check_imported_snapshot);
	add(tc_import_delete_snapshot);
	add(tc_import_get_raw_metadata);
	add(tc_import_get_snapshot_dir);
    }

    BtrfsImportTestsuite::~BtrfsImportTestsuite()
    {
	std::cout << "calling ts dtor" << std::endl;
	snapper::Snapper::deleteConfig("testsuite-import-btrfs");
	std::cout << "done" << std::endl;
    }


}}
