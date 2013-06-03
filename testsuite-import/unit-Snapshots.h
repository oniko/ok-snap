#include <boost/test/unit_test.hpp>

#include "testsuite-import/snapshots-fixtures.h"

BOOST_FIXTURE_TEST_CASE ( tc_import_helper_type_none, ImportHelperTypeNone )
{
    snapper::Snapshots::iterator it;
    BOOST_CHECK_THROW( it = f_shs.import_helper(f_sh), snapper::IllegalSnapshotException );
}

BOOST_FIXTURE_TEST_CASE ( tc_import_helper_type_clone_valid, ImportHelperTypeCloneValid )
{
    snapper::Snapshots::iterator it;

    BOOST_CHECK_NO_THROW( it = f_shs.importHelper(f_sh) );
}

BOOST_FIXTURE_TEST_CASE ( tc_import_helper_type_adopt_valid, ImportHelperTypeAdoptValid )
{
    snapper::Snapshots::iterator it;

    BOOST_CHECK_NO_THROW( f_shs.importHelper(f_sh) );
}

BOOST_FIXTURE_TEST_CASE ( tc_import_helper_type_adopt_invalid, ImportHelperTypeAdoptInvalid )
{
    snapper::Snapshots::iterator it;

    BOOST_CHECK_THROW( it = f_shs.importHelper(f_sh), snapper::IllegalSnapshotException );
}

BOOST_FIXTURE_TEST_CASE ( tc_import_helper_type_adopt_already_imported, ImportHelperTypeAdoptAlreadyImported )
{
    snapper::Snapshots::iterator it;

    BOOST_CHECK_THROW( it = f_shs.importHelper(f_sh), snapper::ImportSnapshotFailedException );
    BOOST_CHECK_THROW( it = f_shs.importHelper(f_sh_2), snapper::ImportSnapshotFailedException );
}

BOOST_FIXTURE_TEST_CASE ( tc_import_helper_type_ack_valid, ImportHelperTypeAckValid )
{
    snapper::Snapshots::iterator it;

    BOOST_CHECK_NO_THROW( f_shs.importHelper(f_sh) );
}

BOOST_FIXTURE_TEST_CASE ( tc_import_helper_type_ack_invalid, ImportHelperTypeAckInvalid )
{
    snapper::Snapshots::iterator it;

    BOOST_CHECK_THROW( it = f_shs.importHelper(f_sh), snapper::IllegalSnapshotException );
}

BOOST_FIXTURE_TEST_CASE ( tc_import_helper_type_ack_already_imported, ImportHelperTypeAckAlreadyImported )
{
    snapper::Snapshots::iterator it;

    BOOST_CHECK_THROW( it = f_shs.importHelper(f_sh), snapper::ImportSnapshotFailedException );
    BOOST_CHECK_THROW( it = f_shs.importHelper(f_sh_2), snapper::ImportSnapshotFailedException );
}
