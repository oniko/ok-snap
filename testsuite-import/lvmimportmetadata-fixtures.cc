//#include <boost/test/unit_test.hpp>
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

    LvmImportConstructor::LvmImportConstructor()
	: ValidMetadata()
    {
	f_raw_data_missing_vg.insert(make_pair("lv_name", "some_string"));
	f_raw_data_missing_vg.insert(make_pair("the_answer", "42"));

	f_raw_data_missing_lv.insert(make_pair("vg_name", "some_string"));
	f_raw_data_missing_lv.insert(make_pair("the_answer", "42"));
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

    CopyConstructor::CopyConstructor()
	: ValidMetadata(), f_origin(f_raw_data, f_dummy_lvm)
    {
    }

    EqualMethodTrue::EqualMethodTrue()
	: CopyConstructor(), f_raw_data_copy(f_raw_data)
    {
	f_raw_data_copy["the_answer"] = "43";
	f_p_origin_copy = new snapper::LvmImportMetadata(f_raw_data_copy, f_dummy_lvm);
    }

    EqualMethodTrue::~EqualMethodTrue()
    {
	delete f_p_origin_copy;
    }

    EqualMethodFalse::EqualMethodFalse()
	: EqualFalseTestMetadata(), f_origin(f_raw_data, f_dummy_lvm),
	f_data_diff_in_vg(f_raw_data_diff_in_vg, f_dummy_lvm),
	f_data_diff_in_lv(f_raw_data_diff_in_lv, f_dummy_lvm),
	f_data_different(f_raw_data_different, f_dummy_lvm)
    {
    }

}}
