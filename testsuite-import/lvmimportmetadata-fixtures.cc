#include "testsuite-import/lvmimportmetadata-fixtures.h"

#include "snapper/Lvm.h"

namespace testsuiteimport { namespace lvm {

    using std::make_pair;

    ValidMetadata::ValidMetadata()
    {
	f_raw_data.insert(make_pair("vg_name", "some_string"));
	f_raw_data.insert(make_pair("lv_name", "some_string"));
	f_raw_data.insert(make_pair("the_answer", "42"));
    }

    MissingVgName::MissingVgName()
    {
	f_raw_data.insert(make_pair("lv_name", "some_string"));
	f_raw_data.insert(make_pair("the_answer", "42"));
    }

    MissingLvName::MissingLvName()
    {
	f_raw_data.insert(make_pair("vg_name", "some_string"));
	f_raw_data.insert(make_pair("the_answer", "42"));
    }

    CopyConstructor::CopyConstructor()
	: ValidMetadata(), f_origin(f_raw_data, static_cast<const snapper::Lvm *>(123456789))
    {
    }

    EqualMethodTrue::EqualMethodTrue()
	: CopyConstructor(), f_raw_data_copy(f_raw_data), f_origin_copy(f_origin)
    {
    }

    EqualMethodFalse::EqualMethodFalse()
	: CopyConstructor()
    {
	f_raw_data_diff_in_vg.insert(make_pair("vg_name", "different_string"));
	f_raw_data_diff_in_vg.insert(*f_raw_data.find("lv_name"));

	f_raw_data_diff_in_lv.insert(*f_raw_data.find("vg_name"));
	f_raw_data_diff_in_lv.insert(make_pair("lv_name", "different_string"));

	f_raw_data_different(make_pair("vg_name", "and_now_for_some-"));
	f_raw_data_different(make_pair("lv_name", "thing_completely_different"));

	const snapper::Lvm* dummy_addr = static_cast<const snapper::Lvm *>(123456789);

	f_data_diff_in_vg = snapper::LvmImportMetadata(f_raw_data_diff_in_vg, dummy_addr);
	f_data_diff_in_lv = snapper::LvmImportMetadata(f_raw_data_diff_in_lv, dummy_addr);
	f_data_different = snapper::LvmImportMetadata(f_raw_data_different, dummy_addr);
    }
}}
