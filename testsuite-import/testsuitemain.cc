#include <boost/test/unit_test.hpp>

#include "testsuite-import/lvm-master-testsuite.h"

using namespace boost::unit_test;

using testsuiteimport::lvm::LvmImportTestsuite;

bool init_unit_test()
{
    try
    {
	framework::master_test_suite().add(new LvmImportTestsuite());
    }
    catch (...)
    {
	return false;
    }

    return true;
}

int main(int argc, char **argv)
{
    return unit_test_main( &init_unit_test, argc, argv);
}

