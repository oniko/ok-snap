#include "testsuite-import/helpers.h"
#include "testsuite-import/lvmimportmetadata-fixtures.h"

#include "snapper/Exception.h"

namespace testsuiteimport { namespace lvm
{

    LvmSubvolumeWrapper::LvmSubvolumeWrapper(const string& vg_name, const string& lv_orig_name, const string& lv_name, bool ro)
	: vg_name(vg_name), lv_name(lv_name), lv_orig_name(lv_orig_name)
    {
	lvcreate_thin_snapshot_wrapper(vg_name, lv_orig_name, lv_name, ro);	    
    }


    LvmSubvolumeWrapper::LvmSubvolumeWrapper(const string& vg_name, const string& lv_name)
    {
	lvcreate_non_thin_lv_wrapper(vg_name, lv_name);
    }


    LvmSubvolumeWrapper::~LvmSubvolumeWrapper()
    {
	try
	{
	    lvremove_wrapper(vg_name, lv_name);
	}
	catch (const ImportTestsuiteException &e) {}
    }


    ValidMetadata::ValidMetadata()
	: f_dummy_lvm(reinterpret_cast<const snapper::Lvm *>(123456789)),
	f_raw_data("some_vg/some_lv"),
	f_lvm_import_metadata(f_raw_data, snapper::ImportPolicy::ADOPT, f_dummy_lvm)
    {
    }


    LvmImportConstructor::LvmImportConstructor()
	: ValidMetadata(), f_raw_data_missing_vg("/some_lv"),
	f_raw_data_missing_lv("some_vg/"),
	f_raw_data_illegal_1("some_vg/some_lv/some_nonsense"),
	f_raw_data_illegal_2("/looks_like/ordinary_path"),
	f_raw_data_illegal_3("vg/lv_with_trailing/"),
	f_raw_data_illegal_4("missing_slash_in_subvolume_string"),
	f_raw_data_empty("")
    {
    }


    LvmCompareImportMetadata::LvmCompareImportMetadata()
	: ValidMetadata(),
	f_lvm_import_metadata_identical(f_raw_data, snapper::ImportPolicy::CLONE, f_dummy_lvm),
	f_lvm_import_metadata_diff_in_vg("another_vg" + "/" + f_raw_data.substr(f_raw_data.find("/") + 1), snapper::ImportPolicy::ADOPT, f_dummy_lvm),
	f_lvm_import_metadata_diff_in_lv(f_raw_data.substr(0, f_raw_data.find("/")) + "/" + "another_lv", snapper::ImportPolicy::ADOPT, f_dummy_lvm),
	f_lvm_import_metadata_different("another_vg/another_lv", snapper::ImportPolicy::ADOPT, f_dummy_lvm)
    {
    }


    LvmGetDevicePath::LvmGetDevicePath()
	: ValidMetadata(), f_expected_retval("/dev/" + f_raw_data)
    {
    }


    CheckImportedSnapshot::CheckImportedSnapshot()
	: LvmIMCheckImportedSnapshot(),
	f_clone_import_data_valid_ro(f_ro_subvolume.subvolume(), snapper::ImportPolicy::CLONE, f_lvm),
	f_clone_import_data_valid_rw(f_rw_subvolume.subvolume(), snapper::ImportPolicy::CLONE, f_lvm),
	f_clone_import_missing_lv(f_missing_lv_vg_name + "/" + f_missing_lv_lv_name, snapper::ImportPolicy::CLONE, f_lvm),
	f_clone_import_nonthin_lv(f_nonthin_subvolume.subvolume(), snapper::ImportPolicy::CLONE, f_lvm),
	f_clone_import_foreign_vg(f_foreign_vg_subvolume.subvolume(), snapper::ImportPolicy::CLONE, f_lvm),
	f_clone_import_current_subvolume(f_current_subvolume_vg_name + "/" + f_current_subvolume_lv_name, snapper::ImportPolicy::CLONE, f_lvm),
	f_adopt_import_data_valid_ro(f_ro_subvolume.subvolume(), snapper::ImportPolicy::ADOPT, f_lvm),
	f_adopt_import_data_rw(f_rw_subvolume.subvolume(), snapper::ImportPolicy::ADOPT, f_lvm),
	f_adopt_import_missing_lv(f_missing_lv_vg_name + "/" + f_missing_lv_lv_name, snapper::ImportPolicy::ADOPT, f_lvm),
	f_adopt_import_nonthin_lv(f_nonthin_subvolume.subvolume(), snapper::ImportPolicy::ADOPT, f_lvm),
	f_adopt_import_foreign_vg(f_foreign_vg_subvolume.subvolume(), snapper::ImportPolicy::ADOPT, f_lvm),
	f_adopt_import_current_subvolume(f_current_subvolume_vg_name + "/" + f_current_subvolume_lv_name, snapper::ImportPolicy::ADOPT, f_lvm),
	f_ack_import_data_valid_ro(f_ro_subvolume.subvolume(), snapper::ImportPolicy::ACKNOWLEDGE, f_lvm),
	f_ack_import_data_rw(f_rw_subvolume.subvolume(), snapper::ImportPolicy::ACKNOWLEDGE, f_lvm),
	f_ack_import_missing_lv(f_missing_lv_vg_name + "/" + f_missing_lv_lv_name, snapper::ImportPolicy::ACKNOWLEDGE, f_lvm),
	f_ack_import_nonthin_lv(f_nonthin_subvolume.subvolume(), snapper::ImportPolicy::ACKNOWLEDGE, f_lvm),
	f_ack_import_foreign_vg(f_foreign_vg_subvolume.subvolume(), snapper::ImportPolicy::ACKNOWLEDGE, f_lvm),
	f_ack_import_current_subvolume(f_current_subvolume_vg_name + "/" + f_current_subvolume_lv_name, snapper::ImportPolicy::ACKNOWLEDGE, f_lvm)
    {
    }


    LvmIMCheckImportedSnapshot::LvmIMCheckImportedSnapshot()
	: LvmGeneralFixture(), f_missing_lv_vg_name(LvmGeneralFixture::f_conf_lvm_vg_name),
	f_missing_lv_lv_name("ThisIsMissingLogVol"),
	f_nonthin_subvolume(LvmGeneralFixture::f_conf_lvm_vg_name, "test_non_thin_volume_01"),
	f_foreign_vg_subvolume(LvmGeneralFixture::f_conf_lvm_vg_name, "test_lv_volume_01"),
	f_current_subvolume_vg_name(LvmGeneralFixture::f_conf_lvm_vg_name),
	f_current_subvolume_lv_name(LvmGeneralFixture::f_conf_lvm_origin_lv_name),
	f_rw_wrong_fs_uuid_subvolume(LvmGeneralFixture::f_conf_lvm_vg_name, LvmGeneralFixture::f_conf_lvm_origin_lv_name, "test_lv_wrong_fs_uuid_01", false),
	f_rw_subvolume(LvmGeneralFixture::f_conf_lvm_vg_name, LvmGeneralFixture::f_conf_lvm_origin_lv_name, "test_rw_snapshot_01", false),
	f_ro_wrong_fs_uuid_subvolume(LvmGeneralFixture::f_conf_lvm_vg_name, LvmGeneralFixture::f_conf_lvm_origin_lv_name, "test_ro_snapshot_01")
    {
    }


    CloneSnapshot::CloneSnapshot()
	: f_env(0),
	f_origin_volume(LvmGeneralFixture::f_conf_lvm_vg_name, LvmGeneralFixture::f_conf_lvm_origin_lv_name, "test_clone_snapshot_01"),
	f_clone_valid_metadata(f_origin_volume.subvolume(), snapper::ImportPolicy::CLONE, f_lvm)
    {
    }



}}
