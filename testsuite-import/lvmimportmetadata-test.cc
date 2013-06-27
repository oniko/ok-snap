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

/*
    void LvmImportMetadataTestClass::tc_import_copy_ctor()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FCopyConstructor());
	fixture->test_method();
    }
*/

    void LvmImportMetadataTestClass::tc_import_equal_method_true()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FEqualMethodTrue());
	fixture->test_method();
    }

    void LvmImportMetadataTestClass::tc_import_equal_method_false()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FEqualMethodFalse());
	fixture->test_method();
    }

    void FLvmImportConstructor::test_method()
    {
	boost::scoped_ptr<snapper::LvmImportMetadata> p_imdata;

	BOOST_CHECK_THROW( p_imdata.reset(new snapper::LvmImportMetadata(f_raw_data_missing_vg, snapper::ImportPolicy::NONE, f_dummy_lvm)), snapper::InvalidImportMetadataException );
	BOOST_CHECK_THROW( p_imdata.reset(new snapper::LvmImportMetadata(f_raw_data_missing_lv, snapper::ImportPolicy::NONE, f_dummy_lvm)), snapper::InvalidImportMetadataException );

	BOOST_REQUIRE_NO_THROW( p_imdata.reset(new snapper::LvmImportMetadata(f_raw_data, snapper::ImportPolicy::NONE, f_dummy_lvm)));

	BOOST_CHECK_EQUAL( p_imdata->vg_name, f_raw_data["vg_name"] );
	BOOST_CHECK_EQUAL( p_imdata->lv_name, f_raw_data["lv_name"] );
    }

/*
    void FCopyConstructor::test_method()
    {
	boost::scoped_ptr<snapper::LvmImportMetadata> p_copy;

	BOOST_REQUIRE_NO_THROW( p_copy.reset(new snapper::LvmImportMetadata(f_origin)) );

	BOOST_CHECK_EQUAL( p_copy->getVgName(), f_origin.getVgName() );
	BOOST_CHECK_EQUAL( p_copy->getLvName(), f_origin.getLvName() );
	BOOST_CHECK_EQUAL( p_copy->getCreationTime(), f_origin.getCreationTime() );
    }
*/

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
