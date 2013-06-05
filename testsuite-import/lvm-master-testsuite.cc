#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>

//#include "config.h"

#include "testsuite-import/lvm-master-testsuite.h"
#include "testsuite-import/lvmimportmetadata-test.h"

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

	boost::shared_ptr<LvmImportMetadataTestClass> instance(new LvmImportMetadataTestClass());

	// LvmImportMetadata unit
	test_case* tc_import_ctor_valid = BOOST_CLASS_TEST_CASE( &LvmImportMetadataTestClass::tc_import_ctor_valid, instance );
	add( tc_import_ctor_valid );
    }

    LvmImportTestsuite::~LvmImportTestsuite()
    {
	snapper::Snapper::deleteConfig("testsuite-import");
    }
}}
