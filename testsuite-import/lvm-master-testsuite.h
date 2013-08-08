#ifndef LVM_MASTER_TESTSUITE_H
#define LVM_MASTER_TESTSUITE_H

#include <string>

#include <boost/test/unit_test.hpp>

#include "snapper/Logger.h"

#include "testsuite-import/lvm-snapshot-fixtures.h"

namespace testsuiteimport { namespace lvm {
    using std::string;

    //snapper::LogLevel, const string&, const char*, int, const char*, const string*
    
    // TODO: think about possibility of proper logging in testsuite.
    // Withou logging turned off, during testsuite teardown snapper destructor
    // ends with exception
    
    static bool true_log_query(snapper::LogLevel, const string& component,
			 const char* file, int line, const char* func,
			 const string* content)
    {
	return true;
    }


    struct LvmImportTestsuite : public boost::unit_test::test_suite
    {
	LvmImportTestsuite();
	~LvmImportTestsuite();

	static void myTestLogDo(::snapper::LogLevel level, const string& component, const char* file, int line,
				     const char* func, const string& text);
    };

}}
#endif
