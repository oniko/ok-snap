//#include <boost/test/unit_test.hpp>
//#include <boost/scoped_ptr.hpp>

#include "testsuite-import/lvmimportmetadata-test.h"
#include "testsuite-import/lvmimportmetadata-fixtures.h"

namespace testsuiteimport { namespace lvm
{
    void LvmImportMetadataTestClass::tc_import_ctor_valid()
    {
	GeneralFixture *fixture = new ValidMetadata;

	fixture->test_method();

	delete fixture;
    }
}}
