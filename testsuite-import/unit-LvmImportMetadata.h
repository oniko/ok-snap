#include <boost/test/unit_test.hpp>
#include <boost/scoped_ptr.hpp>

#define private public
#define protected public
    #include "snapper/Lvm.h"
    #include "snapper/ImportMetadata.h"
    #include "snapper/LvmImportMetadata.h"
#undef private
#undef protected

#include "testsuite-import/lvmimportmetadata-fixtures.h"

using namespace testsuiteimport::lvm;

BOOST_FIXTURE_TEST_CASE( tc_import_ctor_valid, ValidMetadata )
{
    boost::scoped_ptr<snapper::LvmImportMetadata> p_imdata;

    BOOST_REQUIRE_NO_THROW( p_imdata.reset(new snapper::LvmImportMetadata(f_raw_data, f_dummy_lvm)));

    BOOST_CHECK_EQUAL( p_imdata->getVgName(), f_raw_data["vg_name"] );
    BOOST_CHECK_EQUAL( p_imdata->getLvName(), f_raw_data["lv_name"] );

    // TODO: delete me?
    BOOST_CHECK_EQUAL( p_imdata->getImportMetadataId(), snapper::ImportTypeId::LVM2 );
}

BOOST_FIXTURE_TEST_CASE( tc_import_ctor_missing_vg, FMissingVgName )
{
    BOOST_CHECK_THROW( snapper::LvmImportMetadata imdata(f_raw_data, f_dummy_lvm), snapper::InvalidImportMetadataException );
}

BOOST_FIXTURE_TEST_CASE ( tc_import_ctor_missing_lv, FMissingLvName )
{
    BOOST_CHECK_THROW( snapper::LvmImportMetadata imdata(f_raw_data, f_dummy_lvm), snapper::InvalidImportMetadataException );
}

// TODO: depends on basic ctor
BOOST_FIXTURE_TEST_CASE ( tc_import_copy_ctor, CopyConstructor )
{
    boost::scoped_ptr<snapper::LvmImportMetadata> p_copy;

    BOOST_REQUIRE_NO_THROW( p_copy.reset(new snapper::LvmImportMetadata(f_origin)) );

    BOOST_CHECK_EQUAL( p_copy->getVgName(), f_origin.getVgName() );
    BOOST_CHECK_EQUAL( p_copy->getLvName(), f_origin.getLvName() );
    BOOST_CHECK_EQUAL( p_copy->getCreationTime(), f_origin.getCreationTime() );
}

// TODO: depends on copy ctor
BOOST_FIXTURE_TEST_CASE ( tc_import_clone_method, CopyConstructor )
{
    boost::scoped_ptr<snapper::LvmImportMetadata> p_copy;

    BOOST_REQUIRE_NO_THROW( p_copy.reset(static_cast<snapper::LvmImportMetadata *>(f_origin.clone())) );

    BOOST_CHECK_EQUAL( p_copy->getVgName(), f_origin.getVgName() );
    BOOST_CHECK_EQUAL( p_copy->getLvName(), f_origin.getLvName() );
    BOOST_CHECK_EQUAL( p_copy->getCreationTime(), f_origin.getCreationTime() );
}

// TODO: depends on copy ctor
BOOST_FIXTURE_TEST_CASE ( tc_import_equal_method_true, EqualMethodTrue )
{
    BOOST_CHECK( f_origin.isEqualImpl(f_origin_copy) );
}

// TODO: depends on basic ctor
BOOST_FIXTURE_TEST_CASE (tc_import_equal_method_false, EqualMethodFalse )
{
    BOOST_CHECK ( !f_origin.isEqualImpl(f_data_diff_in_vg) );
    BOOST_CHECK ( !f_origin.isEqualImpl(f_data_diff_in_lv) );
    BOOST_CHECK ( !f_origin.isEqualImpl(f_data_different) );
}
