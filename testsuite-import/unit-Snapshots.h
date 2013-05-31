#include <boost/test/unit_test.hpp>

#include "testsuite-import/snapshots-fixtures.h"

BOOST_FIXTURE_TEST_CASE ( tc_import_helper_type_none, ImportHelperTypeNone )
{
    snapper::Snapshots::iterator it;
    BOOST_CHECK_THROW( it = f_shs.import_helper(f_sh), snapper::IllegalSnapshotException );
}

BOOST_FIXTURE_TEST_CASE ( tc_import_helper_type_clone, ImportHelperTypeClone )
{
    
}
