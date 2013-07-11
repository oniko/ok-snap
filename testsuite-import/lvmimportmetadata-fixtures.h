#ifndef LVMIMPORTMETADATA_FIXTURES_H
#define LVMIMPORTMETADATA_FIXTURES_H

#include "testsuite-import/general-fixtures.h"

#include <string>

#include "snapper/Lvm.h"
#include "snapper/LvmImportMetadata.h"


namespace testsuiteimport { namespace lvm {

    using std::string;

    struct LvmSubvolumeWrapper
    {
	LvmSubvolumeWrapper(const string& vg_name, const string& lv_orig_name, const string& lv_name, bool ro = true);
	~LvmSubvolumeWrapper();

	const string vg_name;
	const string lv_name;
	const string lv_orig_name;
    };


    struct ValidMetadata
    {
	ValidMetadata();
	virtual ValidMetadata() {}

	const snapper::Lvm* f_dummy_lvm;
	const string f_raw_data;
    };


    struct LvmImportConstructor : public ValidMetadata
    {
	LvmImportConstructor();

	const string f_raw_data_missing_vg;
	const string f_raw_data_missing_lv;
	const string f_raw_data_illegal_1;
	const string f_raw_data_illegal_2;
	const string f_raw_data_illegal_3;
	const string f_raw_data_illegal_4;
	const string f_raw_data_empty;
    };


    struct LvmCompareImportMetadata : public ValidMetadata
    {
	LvmCompareImportMetadata();

	const snapper::LvmImportMetadata f_lvm_import_metadata;
	const snapper::LvmImportMetadata f_lvm_import_metadata_identical;

	const snapper::LvmImportMetadata f_lvm_import_metadata_diff_in_vg;
	const snapper::LvmImportMetadata f_lvm_import_metadata_diff_in_lv;
	const snapper::LvmImportMetadata f_lvm_import_metadata_different;
    };


    struct LvmGetDevicePath : public ValidMetadata
    {
	LvmGetDevicePath();

	const string f_expected_retval;
    };


    // TODO: move me into lvm-fixtures
    struct LvmCheckImportedSnapshot : public LvmGeneralFixture
    {
	LvmCheckImportedSnapshot();
	~LvmCheckImportedSnapshot();

	// missing lv test
	const string f_missing_lv_vg_name;
	const string f_missing_lv_lv_name;

	// not a thin volume
	const string f_nonthin_vg_name;
	const string f_nonthin_lv_name;

	// foreign vg test case
	const string f_foreign_vg_vg_name;
	const string f_foreign_vg_lv_name;

	// current subvolume test case (aka trying import snapshot 0)
	const string f_current_subvolume_vg_name;
	const string f_current_subvolume_lv_name;

	// test for rw enabled snapshots
	const string f_rw_wrong_fs_uuid_vg_name;
	const string f_rw_wrong_fs_uuid_lv_name;

	const string f_rw_valid_vg_name;
	const string f_rw_valid_lv_name;

	// test for ro snapshots
	const string f_ro_wrong_fs_uuid_vg_name;
	const string f_ro_wrong_fs_uuid_lv_name;

	const string f_ro_valid_vg_name;
	const string f_ro_valid_lv_name;
    };


    struct CheckImportedSnapshot : public LvmCheckImportedSnapshot
    {
	CheckImportedSnapshot();

	// tests for CLONE import policy
	const snapper::LvmImportMetadata f_clone_import_data_valid_ro;
	const snapper::LvmImportMetadata f_clone_import_data_valid_rw;
	const snapper::LvmImportMetadata f_clone_import_missing_lv;
	const snapper::LvmImportMetadata f_clone_import_nonthin_lv;
	const snapper::LvmImportMetadata f_clone_import_foreign_vg;
	const snapper::LvmImportMetadata f_clone_import_current_subvolume;

	// tests for ADOPT policy
	const snapper::LvmImportMetadata f_adopt_import_data_valid_ro;
	const snapper::LvmImportMetadata f_adopt_import_data_rw;
	const snapper::LvmImportMetadata f_adopt_import_missing_lv;
	const snapper::LvmImportMetadata f_adopt_import_nonthin_lv;
	const snapper::LvmImportMetadata f_adopt_import_foreign_vg;
	const snapper::LvmImportMetadata f_adopt_import_current_subvolume;

	// tests for ACKNOWLEDGE policy
	const snapper::LvmImportMetadata f_ack_import_data_valid_ro;
	const snapper::LvmImportMetadata f_ack_import_data_rw;
	const snapper::LvmImportMetadata f_ack_import_missing_lv;
	const snapper::LvmImportMetadata f_ack_import_nonthin_lv;
	const snapper::LvmImportMetadata f_ack_import_foreign_vg;
	const snapper::LvmImportMetadata f_ack_import_current_subvolume;
    };


//     struct EqualMethodTrue : public CopyConstructor
//     {
// 	EqualMethodTrue();
// 	~EqualMethodTrue();
//
// 	map<string,string> f_raw_data_copy;
// 	snapper::LvmImportMetadata* f_p_origin_copy;
//     };


//     struct EqualMethodFalse : public EqualFalseTestMetadata
//     {
// 	EqualMethodFalse();
//
// 	const snapper::LvmImportMetadata f_origin;
//
// 	const snapper::LvmImportMetadata f_data_diff_in_vg;
// 	const snapper::LvmImportMetadata f_data_diff_in_lv;
// 	const snapper::LvmImportMetadata f_data_different;
//     };
}}
#endif //LVMIMPORTMETADATA_FIXTURES_H
