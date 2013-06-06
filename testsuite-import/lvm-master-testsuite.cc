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

	test_case* tc_create_snapshot_environment = BOOST_CLASS_TEST_CASE( &LvmTestClass::tc_create_snapshot_environment, lvm_test );
	test_case* tc_create_snapshot_environment_dir_exists = BOOST_CLASS_TEST_CASE( &LvmTestClass::tc_create_snapshot_environment_dir_exists, lvm_test );

	add(tc_create_snapshot_environment);
	add(tc_create_snapshot_environment_dir_exists);
    }

    LvmImportTestsuite::~LvmImportTestsuite()
    {
	snapper::Snapper::deleteConfig("testsuite-import");
    }
}}
