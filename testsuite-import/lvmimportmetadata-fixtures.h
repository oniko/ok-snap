#ifndef LVMIMPORTMETADATA_FIXTURES_H
#define LVMIMPORTMETADATA_FIXTURES_H

#include <string>

#include "testsuite-import/general-fixtures.h"
#include "testsuite-import/lvm-fixtures.h"

#include "snapper/Lvm.h"
#include "snapper/LvmImportMetadata.h"


namespace testsuiteimport { namespace lvm
{
    using std::string;


    struct ValidMetadata
    {
	ValidMetadata();
	virtual ~ValidMetadata() {}

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


    struct CloneSnapshot : public LvmGeneralFixture
    {
	CloneSnapshot();
	~CloneSnapshot();

	const InfoDirectory f_env;
	const LvmSubvolumeWrapper f_origin_volume;
	const snapper::LvmImportMetadata f_clone_valid_metadata;
    };


    struct GetRawData : public ValidMetadata
    {
	GetRawData();

	const snapper::LvmImportMetadata f_lvm_import_metadata;
    };


    struct GetSnapshotDir : public ValidMetadata, LvmGeneralFixture
    {
	GetSnapshotDir();

	const snapper::LvmImportMetadata f_lvm_import_metadata;
    };

}}
#endif //LVMIMPORTMETADATA_FIXTURES_H
