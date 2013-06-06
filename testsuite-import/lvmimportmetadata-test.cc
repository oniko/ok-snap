#include <boost/scoped_ptr.hpp>

#include "testsuite-import/general-test.h"
#include "testsuite-import/lvmimportmetadata-test.h"
#include "testsuite-import/lvmimportmetadata-fixtures.h"

namespace testsuiteimport { namespace lvm
{
    void LvmImportMetadataTestClass::tc_import_ctor()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FLvmImportConstructor());
	fixture->test_method();
    }

    void LvmImportMetadataTestClass::tc_import_copy_ctor()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FCopyConstructor());
	fixture->test_method();
    }

    void LvmImportMetadataTestClass::tc_import_clone_method()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FCloneMethod());
	fixture->test_method();
    }

    void LvmImportMetadataTestClass::tc_import_equal_method_true()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FEqualMethodTrue());
	fixture->test_method();
    }

    void LvmImportMetadataTestClass::tc_import_equal_method_false()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FEqualMethodFalse());
	fixture->test_method();
    }
}}
