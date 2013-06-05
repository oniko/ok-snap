#include <string>

#include <boost/test/unit_test.hpp>

#include "snapper/Logger.h"

namespace testsuiteimport { namespace lvm {
    using std::string;

    //snapper::LogLevel, const string&, const char*, int, const char*, const string*
    
    // TODO: setup proper snapper logging in testsuite later
    static bool false_log_query(snapper::LogLevel, const string& component,
			 const char* file, int line, const char* func,
			 const string* content)
    {
	return false;
    }

    struct LvmImportTestsuite : public ::boost::unit_test::test_suite
    {
	LvmImportTestsuite();
	~LvmImportTestsuite();

	// Testsuite logger class here
    };

}}
