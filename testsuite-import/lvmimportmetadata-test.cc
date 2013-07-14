#include <boost/test/unit_test.hpp>
#include <boost/scoped_ptr.hpp>

#include "snapper/Exception.h"
#include "testsuite-import/general-test.h"
#include "testsuite-import/lvmimportmetadata-test.h"

namespace testsuiteimport { namespace lvm
{
    void LvmImportMetadataTestClass::tc_import_ctor()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FLvmImportConstructor());
	fixture->test_method();
    }


    void LvmImportMetadataTestClass::tc_import_compare_metadata()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FLvmCompareCheck());
	fixture->test_method();
    }


    void LvmImportMetadataTestClass::tc_import_check_imported_snapshot()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FLvmIMDataCheckImportedSnapshot());
	fixture->test_method();
    }


    void FLvmImportConstructor::test_method()
    {
	boost::scoped_ptr<snapper::LvmImportMetadata> p_imdata;

	BOOST_CHECK_THROW( p_imdata.reset(new snapper::LvmImportMetadata(f_raw_data_missing_vg, snapper::ImportPolicy::NONE, f_dummy_lvm)), snapper::InvalidImportMetadataException );
	BOOST_CHECK_THROW( p_imdata.reset(new snapper::LvmImportMetadata(f_raw_data_missing_lv, snapper::ImportPolicy::NONE, f_dummy_lvm)), snapper::InvalidImportMetadataException );

	BOOST_CHECK_NO_THROW( p_imdata.reset(new snapper::LvmImportMetadata(f_raw_data, snapper::ImportPolicy::CLONE, f_dummy_lvm)) );
	BOOST_CHECK_THROW( p_imdata.reset(new snapper::LvmImportMetadata(f_raw_data, snapper::ImportPolicy::NONE, f_dummy_lvm)), snapper::InvalidImportMetadataException );
    }


    void FLvmCompareCheck::test_method()
    {
	BOOST_CHECK_EQUAL( f_lvm_import_metadata, f_lvm_import_metadata_identical );
	BOOST_CHECK_NE( f_lvm_import_metadata, f_lvm_import_metadata_diff_in_lv );
	BOOST_CHECK_NE( f_lvm_import_metadata, f_lvm_import_metadata_diff_in_vg );
	BOOST_CHECK_NE( f_lvm_import_metadata, f_lvm_import_metadata_different );
    }


    void FLvmIMDataCheckImportedSnapshot::test_method()
    {
	
    }

    


    void FEqualMethodTrue::test_method()
    {
	BOOST_CHECK( f_origin.isEqual(*f_p_origin_copy) );
    }

    void FEqualMethodFalse::test_method()
    {
	BOOST_CHECK ( !f_origin.isEqual(f_data_diff_in_vg) );
	BOOST_CHECK ( !f_origin.isEqual(f_data_diff_in_lv) );
	BOOST_CHECK ( !f_origin.isEqual(f_data_different) );
    }
}}
