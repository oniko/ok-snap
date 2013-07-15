#ifndef LVMIMPORTMETADATA_FIXTURES_H
#define LVMIMPORTMETADATA_FIXTURES_H

#include "testsuite-import/general-fixtures.h"
#include "testsuite-import/lvm-fixtures.h"

#include <string>

#include "snapper/Lvm.h"
#include "snapper/LvmImportMetadata.h"


namespace testsuiteimport { namespace lvm
{

    using std::string;

    


    struct ValidMetadata
    {
	ValidMetadata();
	virtual ValidMetadata() {}

	const snapper::Lvm* f_dummy_lvm;
	const string f_raw_data;

	const snapper::LvmImportMetadata f_lvm_import_metadata;
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
    struct LvmIMCheckImportedSnapshot : public LvmGeneralFixture
    {
	LvmIMCheckImportedSnapshot();
	~LvmIMCheckImportedSnapshot();

	// missing lv test
	const string f_missing_lv_vg_name;
	const string f_missing_lv_lv_name;

	// not a thin volume
// 	const string f_nonthin_vg_name;
// 	const string f_nonthin_lv_name;
	const LvmSubvolumeWrapper f_nonthin_subvolume;

	// about to be removed. condition is useless
	// foreign vg test case
// 	const string f_foreign_vg_vg_name;
// 	const string f_foreign_vg_lv_name;
	const LvmSubvolumeWrapper f_foreign_vg_subvolume;

	// current subvolume test case (aka exercise importing snapshot 0)
	const string f_current_subvolume_vg_name;
	const string f_current_subvolume_lv_name;

	// test for rw enabled snapshots
// 	const string f_rw_wrong_fs_uuid_vg_name;
// 	const string f_rw_wrong_fs_uuid_lv_name;
	const LvmSubvolumeWrapper f_rw_wrong_fs_uuid_subvolume;

// 	const string f_rw_valid_vg_name;
// 	const string f_rw_valid_lv_name;
	const LvmSubvolumeWrapper f_rw_subvolume;

	// test for ro snapshots
// 	const string f_ro_wrong_fs_uuid_vg_name;
// 	const string f_ro_wrong_fs_uuid_lv_name;
	const LvmSubvolumeWrapper f_ro_wrong_fs_uuid_subvolume;

// 	const string f_ro_valid_vg_name;
// 	const string f_ro_valid_lv_name;
	const LvmSubvolumeWrapper f_ro_subvolume;
    };


    struct CheckImportedSnapshot : public LvmIMCheckImportedSnapshot
    {
	CheckImportedSnapshot();

	const InfoDirectory f_env_for_failures;

	// tests for CLONE import policy
	//const CreateSnapshotEnvironment f_clone_env_01;
	//const CreateSnapshotEnvironment f_clone_env_02;

	const snapper::LvmImportMetadata f_clone_import_data_valid_ro;
	const snapper::LvmImportMetadata f_clone_import_data_valid_rw;
	const snapper::LvmImportMetadata f_clone_import_missing_lv;
	const snapper::LvmImportMetadata f_clone_import_nonthin_lv;
	const snapper::LvmImportMetadata f_clone_import_foreign_vg;
	const snapper::LvmImportMetadata f_clone_import_current_subvolume;

	// tests for ADOPT policy
	//const CreateSnapshotEnvironment f_adopt_env_01;

	const snapper::LvmImportMetadata f_adopt_import_data_valid_ro;
	const snapper::LvmImportMetadata f_adopt_import_data_rw;
	const snapper::LvmImportMetadata f_adopt_import_missing_lv;
	const snapper::LvmImportMetadata f_adopt_import_nonthin_lv;
	const snapper::LvmImportMetadata f_adopt_import_foreign_vg;
	const snapper::LvmImportMetadata f_adopt_import_current_subvolume;

	// tests for ACKNOWLEDGE policy
	//const CreateSnapshotEnvironment f_ack_env_01;

	const snapper::LvmImportMetadata f_ack_import_data_valid_ro;
	const snapper::LvmImportMetadata f_ack_import_data_rw;
	const snapper::LvmImportMetadata f_ack_import_missing_lv;
	const snapper::LvmImportMetadata f_ack_import_nonthin_lv;
	const snapper::LvmImportMetadata f_ack_import_foreign_vg;
	const snapper::LvmImportMetadata f_ack_import_current_subvolume;
    };


    struct CloneSnapshot : public LvmGeneralFixture
    {
	CloneSnapshot();

	const InfoDirectory f_env;
	const LvmSubvolumeWrapper f_origin_volume;
	const snapper::LvmImportMetadata f_clone_valid_metadata;
    };




}}
#endif //LVMIMPORTMETADATA_FIXTURES_H
