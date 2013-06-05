#include <boost/test/unit_test.hpp>
#include "testsuite-import/lvmimportmetadata-fixtures.h"

namespace testsuiteimport { namespace lvm {

    using std::make_pair;

    ValidMetadata::ValidMetadata()
	: f_dummy_lvm(reinterpret_cast<const snapper::Lvm *>(123456789))
    {
	f_raw_data.insert(make_pair("vg_name", "some_string"));
	f_raw_data.insert(make_pair("lv_name", "some_string"));
	f_raw_data.insert(make_pair("the_answer", "42"));
    }

    void ValidMetadata::test_method()
    {
	boost::scoped_ptr<snapper::LvmImportMetadata> p_imdata;

	BOOST_REQUIRE_NO_THROW( p_imdata.reset(new snapper::LvmImportMetadata(f_raw_data, f_dummy_lvm)));

	BOOST_CHECK_EQUAL( p_imdata->getVgName(), f_raw_data["vg_name"] );
	BOOST_CHECK_EQUAL( p_imdata->getLvName(), f_raw_data["lv_name"] );

	// TODO: delete me?
	BOOST_CHECK_EQUAL( p_imdata->getImportMetadataId(), snapper::ImportTypeId::LVM2 );
    }

    MissingVgName::MissingVgName()
	: f_dummy_lvm(reinterpret_cast<const snapper::Lvm *>(123456789))
    {
	f_raw_data.insert(make_pair("lv_name", "some_string"));
	f_raw_data.insert(make_pair("the_answer", "42"));
    }

    void MissingVgName::test_method()
    {
    }

    MissingLvName::MissingLvName()
	: f_dummy_lvm(reinterpret_cast<const snapper::Lvm *>(123456789))
    {
	f_raw_data.insert(make_pair("vg_name", "some_string"));
	f_raw_data.insert(make_pair("the_answer", "42"));
    }

    void MissingLvName::test_method()
    {
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

    void EqualFalseTestMetadata::test_method()
    {
    }

    CopyConstructor::CopyConstructor()
	: ValidMetadata(), f_origin(f_raw_data, reinterpret_cast<const snapper::Lvm *>(123456789))
    {
    }

    void CopyConstructor::test_method()
    {
    }

    EqualMethodTrue::EqualMethodTrue()
	: CopyConstructor(), f_raw_data_copy(f_raw_data), f_origin_copy(f_origin)
    {
    }

    void EqualMethodTrue::test_method()
    {
    }

    EqualMethodFalse::EqualMethodFalse()
	: EqualFalseTestMetadata(), CopyConstructor(),
	dummy_addr(reinterpret_cast<const snapper::Lvm *>(123456789)),
	f_data_diff_in_vg(f_raw_data_diff_in_vg, dummy_addr),
	f_data_diff_in_lv(f_raw_data_diff_in_lv, dummy_addr),
	f_data_different(f_raw_data_different, dummy_addr)
    {
    }

    void EqualMethodFalse::test_method()
    {
    }

}}
