#include <boost/test/unit_test.hpp>

#include "testsuite-import/general-fixtures.h"


namespace testsuiteimport { namespace lvm {
    using boost::unit_test::test_suite;

    struct LvmImportTestsuite : public test_suite
    {
	LvmImportTestsuite();
	~LvmImportTestsuite();
    };
}}
