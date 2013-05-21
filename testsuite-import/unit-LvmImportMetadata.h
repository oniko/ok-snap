#ifndef UNIT_LVM_IMPORT_METADATA_H
#define UNIT_LVM_IMPORT_METADATA_H

//#include <boost/test/unit_test.hpp>

//#include "testsuite-import/lvm-import-fixtures.h"

/* #include "snapper/Snapper.h"

// ouch !!
//#define private public
    #include "snapper/LvmImportMetadata.h"
//#define private private
*/

BOOST_FIXTURE_TEST_CASE ( tc_import_ctor_valid, ValidMetadata )
{
    snapper::LvmImportMetadata imdata(raw_data, lvm);

    BOOST_CHECK_EQUAL( imdata.getVgName().empty(), false );
    BOOST_CHECK_EQUAL( imdata.getLvName().empty(), false );

    BOOST_CHECK_EQUAL( imdata.getVgName(), raw_data["vg_name"] );
    BOOST_CHECK_EQUAL( imdata.getLvName(), raw_data["lv_name"] );
}

BOOST_FIXTURE_TEST_CASE ( tc_import_ctor_missing_vg, MissingVgName )
{
    BOOST_CHECK_THROW( snapper::LvmImportMetadata imdata(raw_data, lvm), snapper::InvalidImportMetadataException );
}

BOOST_FIXTURE_TEST_CASE ( tc_import_ctor_missing_lv, MissingLvName )
{
    BOOST_CHECK_THROW( snapper::LvmImportMetadata imdata(raw_data, lvm), snapper::InvalidImportMetadataException );
}

BOOST_FIXTURE_TEST_CASE ( tc_import_clone_method, ValidMetadata )
{
    snapper::LvmImportMetadata imdata(raw_data, lvm);
    snapper::LvmImportMetadata *p_data = static_cast<snapper::LvmImportMetadata *>(imdata.clone());

    BOOST_CHECK_EQUAL(p_data->getVgName(), imdata.getVgName());
    BOOST_CHECK_EQUAL(p_data->getLvName(), imdata.getLvName());
    BOOST_CHECK_EQUAL(p_data->getCreationTime(), imdata.getCreationTime());
}
#endif
