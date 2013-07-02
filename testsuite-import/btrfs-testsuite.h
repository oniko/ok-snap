#ifndef BTRFS_TESTSUITE_H
#define BTRFS_TESTSUITE_H

#include <string>

#include <boost/test/unit_test.hpp>

#include "snapper/Logger.h"

namespace testsuiteimport { namespace btrfs {
    using std::string;

    static bool b_false_log_query(snapper::LogLevel, const string& component,
			 const char* file, int line, const char* func,
			 const string* content)
    {
	return false;
    }

    struct BtrfsImportTestsuite : public boost::unit_test::test_suite
    {
	BtrfsImportTestsuite();
	~BtrfsImportTestsuite();

	// Testsuite logger class here
    };

}}
#endif // BTRFS_TESTSUITE_H
