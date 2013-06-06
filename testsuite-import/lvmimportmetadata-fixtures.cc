#include <boost/test/unit_test.hpp>
#include <boost/scoped_ptr.hpp>

#include "testsuite-import/lvmimportmetadata-fixtures.h"

#include "snapper/Exception.h"

namespace testsuiteimport { namespace lvm {

    using std::make_pair;   

    ValidMetadata::ValidMetadata()
	: f_dummy_lvm(reinterpret_cast<const snapper::Lvm *>(123456789))
    {
	f_raw_data.insert(make_pair("vg_name", "some_string"));
	f_raw_data.insert(make_pair("lv_name", "some_string"));
	f_raw_data.insert(make_pair("the_answer", "42"));
    }

    FLvmImportConstructor::FLvmImportConstructor()
	: ValidMetadata()
    {
	f_raw_data_missing_vg.insert(make_pair("lv_name", "some_string"));
	f_raw_data_missing_vg.insert(make_pair("the_answer", "42"));

	f_raw_data_missing_lv.insert(make_pair("vg_name", "some_string"));
	f_raw_data_missing_lv.insert(make_pair("the_answer", "42"));
    }

    void FLvmImportConstructor::test_method()
    {
	boost::scoped_ptr<snapper::LvmImportMetadata> p_imdata;

	BOOST_CHECK_THROW( p_imdata.reset(new snapper::LvmImportMetadata(f_raw_data_missing_vg, f_dummy_lvm)), snapper::InvalidImportMetadataException );
	BOOST_CHECK_THROW( p_imdata.reset(new snapper::LvmImportMetadata(f_raw_data_missing_lv, f_dummy_lvm)), snapper::InvalidImportMetadataException );

	BOOST_REQUIRE_NO_THROW( p_imdata.reset(new snapper::LvmImportMetadata(f_raw_data, f_dummy_lvm)));

	BOOST_CHECK_EQUAL( p_imdata->getVgName(), f_raw_data["vg_name"] );
	BOOST_CHECK_EQUAL( p_imdata->getLvName(), f_raw_data["lv_name"] );
    }

    EqualFalseTestMetadata::EqualFalseTestMetadata()
	: ValidMetadata()
    {
	f_raw_data_diff_in_vg.insert(make_pair("vg_name", "different_string"));
	f_raw_data_diff_in_vg.insert(*f_raw_data.find("lv_name"));

	f_raw_data_diff_in_lv.insert(*f_raw_data.find("vg_name"));
	f_raw_data_diff_in_lv.insert(make_pair("lv_name", "different_string"));

	f_raw_data_different.insert(make_pair("vg_name", "and_now_for_some-"));
	f_raw_data_different.insert(make_pair("lv_name", "thing_completely_different"));
    }

    FCopyConstructor::FCopyConstructor()
	: ValidMetadata(), f_origin(f_raw_data, f_dummy_lvm)
    {
    }

    void FCopyConstructor::test_method()
    {
	boost::scoped_ptr<snapper::LvmImportMetadata> p_copy;

	BOOST_REQUIRE_NO_THROW( p_copy.reset(new snapper::LvmImportMetadata(f_origin)) );

	BOOST_CHECK_EQUAL( p_copy->getVgName(), f_origin.getVgName() );
	BOOST_CHECK_EQUAL( p_copy->getLvName(), f_origin.getLvName() );
	BOOST_CHECK_EQUAL( p_copy->getCreationTime(), f_origin.getCreationTime() );
    }

    void FCloneMethod::test_method()
    {
	boost::scoped_ptr<snapper::LvmImportMetadata> p_copy;

	BOOST_REQUIRE_NO_THROW( p_copy.reset(dynamic_cast<snapper::LvmImportMetadata *>(f_origin.clone())) );

	BOOST_CHECK_EQUAL( p_copy->getVgName(), f_origin.getVgName() );
	BOOST_CHECK_EQUAL( p_copy->getLvName(), f_origin.getLvName() );
	BOOST_CHECK_EQUAL( p_copy->getCreationTime(), f_origin.getCreationTime() );
    }

    FEqualMethodTrue::FEqualMethodTrue()
	: FCopyConstructor(), f_raw_data_copy(f_raw_data)
    {
	f_raw_data_copy["the_answer"] = "43";
	f_p_origin_copy = new snapper::LvmImportMetadata(f_raw_data_copy, f_dummy_lvm);
    }

    FEqualMethodTrue::~FEqualMethodTrue()
    {
	delete f_p_origin_copy;
    }

    void FEqualMethodTrue::test_method()
    {
	BOOST_CHECK( f_origin.isEqualImpl(*f_p_origin_copy) );
    }

    FEqualMethodFalse::FEqualMethodFalse()
	: EqualFalseTestMetadata(), f_origin(f_raw_data, f_dummy_lvm),
	f_data_diff_in_vg(f_raw_data_diff_in_vg, f_dummy_lvm),
	f_data_diff_in_lv(f_raw_data_diff_in_lv, f_dummy_lvm),
	f_data_different(f_raw_data_different, f_dummy_lvm)
    {
    }

    void FEqualMethodFalse::test_method()
    {
	BOOST_CHECK ( !f_origin.isEqualImpl(f_data_diff_in_vg) );
	BOOST_CHECK ( !f_origin.isEqualImpl(f_data_diff_in_lv) );
	BOOST_CHECK ( !f_origin.isEqualImpl(f_data_different) );
    }

}}
