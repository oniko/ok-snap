#include <boost/test/unit_test.hpp>

#include "testsuite-import/snapshot-fixtures.h"

// FIXME: Move this test case outside Lvm dependant unit tests
BOOST_FIXTURE_TEST_CASE ( tc_snapshot_simple_constructor, SimpleConstructorValid )
{
    snapper::Snapshot snapshot(f_snapper, f_type, f_num, f_date);

    BOOST_CHECK_EQUAL( snapshot.getType(), f_type );
    BOOST_CHECK_EQUAL( snapshot.getNum(), f_num );
    BOOST_CHECK_EQUAL( snapshot.getDate(), f_date );
    BOOST_CHECK_EQUAL( snapshot.getUid(), 0 );
    BOOST_CHECK_EQUAL( snapshot.getPreNum(), 0 );
    BOOST_CHECK_EQUAL( snapshot.getImportPolicy(), snapper::ImportPolicy::NONE );

    // check few private attributes (essential tests for further testing)
    BOOST_CHECK_EQUAL( snapshot.snapper, f_snapper );
    BOOST_CHECK_EQUAL( snapshot.p_idata, NULL );
}

// FIXME: Move this test case outside Lvm dependant unit tests
BOOST_FIXTURE_TEST_CASE ( tc_snapshot_import_constructor, ImportConstructorValid )
{
    snapper::Snapshot snapshot(f_snapper, f_type, f_num, f_date, f_import_policy, f_p_idata);

    BOOST_CHECK_EQUAL( snapshot.getType(), f_type );
    BOOST_CHECK_EQUAL( snapshot.getNum(), f_num );
    BOOST_CHECK_EQUAL( snapshot.getDate(), f_date );
    BOOST_CHECK_EQUAL( snapshot.getUid(), 0 );
    BOOST_CHECK_EQUAL( snapshot.getPreNum(), 0 );
    BOOST_CHECK_EQUAL( snapshot.getImportPolicy(), f_import_policy );

    // check few private attributes (essential tests for further testing)
    BOOST_CHECK_EQUAL( snapshot.snapper, f_snapper );
    BOOST_CHECK_EQUAL( snapshot.p_idata, f_p_idata );

    snapshot.p_idata = NULL;
}

// FIXME: Move this test case outside Lvm dependant unit tests
// TODO: depends on 'tc_snapshot_simple_constructor'
BOOST_FIXTURE_TEST_CASE ( tc_snapshot_copy_constructor_no_import, CopyConstructorNoImport )
{
    snapper::Snapshot copy(f_origin);

    // hmm
    BOOST_CHECK_EQUAL( f_origin, copy );

    BOOST_CHECK_EQUAL( copy.getType(), f_type );
    BOOST_CHECK_EQUAL( copy.getNum(), f_num );
    BOOST_CHECK_EQUAL( copy.getDate(), f_date );
    BOOST_CHECK_EQUAL( copy.getUid(), f_uid );
    BOOST_CHECK_EQUAL( copy.getPreNum(), f_pre_num );
    BOOST_CHECK_EQUAL( copy.getDescription(), f_description );
    BOOST_CHECK_EQUAL( copy.getCleanup(), f_cleanup );
    BOOST_CHECK_EQUAL( copy.getUserdata(), f_userdata );

    BOOST_CHECK_EQUAL( copy.getImportPolicy(), snapper::ImportPolicy::NONE );

    // check rest of private attributes
    BOOST_CHECK_EQUAL( copy.snapper, f_snapper );
    BOOST_CHECK_EQUAL( copy.info_modified, f_info_modified );
    BOOST_CHECK_EQUAL( copy.mount_checked, f_mount_checked );
    BOOST_CHECK_EQUAL( copy.mount_use_count, f_mount_use_count );
    BOOST_CHECK_EQUAL( copy.p_idata, NULL );
}
