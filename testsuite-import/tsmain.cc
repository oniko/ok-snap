#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <ostream>
#include <string>

#define private public
    #include "snapper/Lvm.h"
    #include "snapper/Snapper.h"
#undef private

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "testsuite-import/helpers.h"
#include "testsuite-import/general-fixtures.h"
#include "testsuite-import/lvmimportmetadata-fixtures.h"

using namespace snapper;
using namespace testsuiteimport::lvm;

BOOST_GLOBAL_FIXTURE( LvmGlobalConfig )

BOOST_AUTO_TEST_SUITE ( LVM_import_testsuite )

#include "testsuite-import/unit-LvmImportMetadata.h"

//#include "testsuite-import/unit-Lvm.h"

BOOST_AUTO_TEST_SUITE_END()

/*
using namespace boost::unit_test;

void test_case1() { BOOST_CHECK_EQUAL( 1, 1 ); }

bool
init_unit_test()
{
    //BOOST_GLOBAL_FIXTURE( MyConfig );
    

    test_suite* ts1 = new MyTestSuite( "test_suite1" );
    ts1->add( BOOST_TEST_CASE( &test_case1 ) );
    
    BOOST_CLASS_TEST_CASE

    framework::master_test_suite().add( ts1 );

    return true;
}

int
main( int argc, char* argv[] )
{
    return unit_test_main( &init_unit_test, argc, argv );
}
*/