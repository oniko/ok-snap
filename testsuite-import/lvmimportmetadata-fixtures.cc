#include "testsuite-import/helpers.h"
#include "testsuite-import/lvmimportmetadata-fixtures.h"

#include "snapper/Exception.h"

namespace testsuiteimport { namespace lvm
{

    ValidMetadata::ValidMetadata()
	: f_dummy_lvm(reinterpret_cast<const snapper::Lvm *>(123456789)),
	f_raw_data("some_vg/some_lv")
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
	f_lvm_import_metadata(f_raw_data, snapper::ImportPolicy::CLONE, f_dummy_lvm),
	f_lvm_import_metadata_identical(f_raw_data, snapper::ImportPolicy::CLONE, f_dummy_lvm),
	f_lvm_import_metadata_diff_in_vg("another_vg/" + f_raw_data.substr(f_raw_data.find("/") + 1), snapper::ImportPolicy::ADOPT, f_dummy_lvm),
	f_lvm_import_metadata_diff_in_lv(f_raw_data.substr(0, f_raw_data.find("/")) + "/" + "another_lv", snapper::ImportPolicy::ADOPT, f_dummy_lvm),
	f_lvm_import_metadata_different("another_vg/another_lv", snapper::ImportPolicy::ADOPT, f_dummy_lvm)
    {
    }


    LvmGetDevicePath::LvmGetDevicePath()
	: ValidMetadata(), f_expected_retval("/dev/" + f_raw_data)
    {
    }


    CheckImportedSnapshot::CheckImportedSnapshot()
	: LvmCheckImportedSnapshot(),
	f_clone_import_data_valid_ro(f_ro_subvolume.subvolume(), snapper::ImportPolicy::CLONE, f_lvm),
	f_clone_import_data_valid_rw(f_rw_subvolume.subvolume(), snapper::ImportPolicy::CLONE, f_lvm),
	f_clone_import_missing_lv(f_missing_lv_vg + "/" + f_missing_lv_lv, snapper::ImportPolicy::CLONE, f_lvm),
	f_clone_import_nonthin_lv(f_nonthin_subvolume.subvolume(), snapper::ImportPolicy::CLONE, f_lvm),
	f_clone_import_foreign_vg(f_foreign_vg_subvolume.subvolume(), snapper::ImportPolicy::CLONE, f_lvm),
	f_clone_import_current_subvolume(f_current_subvolume_vg_name + "/" + f_current_subvolume_lv_name, snapper::ImportPolicy::CLONE, f_lvm),
	f_adopt_import_data_valid_ro(f_ro_subvolume.subvolume(), snapper::ImportPolicy::ADOPT, f_lvm),
	f_adopt_import_data_rw(f_rw_subvolume.subvolume(), snapper::ImportPolicy::ADOPT, f_lvm),
	f_adopt_import_missing_lv(f_missing_lv_vg + "/" + f_missing_lv_lv, snapper::ImportPolicy::ADOPT, f_lvm),
	f_adopt_import_nonthin_lv(f_nonthin_subvolume.subvolume(), snapper::ImportPolicy::ADOPT, f_lvm),
	f_adopt_import_foreign_vg(f_foreign_vg_subvolume.subvolume(), snapper::ImportPolicy::ADOPT, f_lvm),
	f_adopt_import_current_subvolume(f_current_subvolume_vg_name + "/" + f_current_subvolume_lv_name, snapper::ImportPolicy::ADOPT, f_lvm),
	f_ack_import_data_valid_ro(f_ro_subvolume.subvolume(), snapper::ImportPolicy::ACKNOWLEDGE, f_lvm),
	f_ack_import_data_rw(f_rw_subvolume.subvolume(), snapper::ImportPolicy::ACKNOWLEDGE, f_lvm),
	f_ack_import_missing_lv(f_missing_lv_vg + "/" + f_missing_lv_lv, snapper::ImportPolicy::ACKNOWLEDGE, f_lvm),
	f_ack_import_nonthin_lv(f_nonthin_subvolume.subvolume(), snapper::ImportPolicy::ACKNOWLEDGE, f_lvm),
	f_ack_import_foreign_vg(f_foreign_vg_subvolume.subvolume(), snapper::ImportPolicy::ACKNOWLEDGE, f_lvm),
	f_ack_import_current_subvolume(f_current_subvolume_vg_name + "/" + f_current_subvolume_lv_name, snapper::ImportPolicy::ACKNOWLEDGE, f_lvm)
    {
    }


    CloneSnapshot::CloneSnapshot()
	: f_env(LvmGeneralFixture::f_conf_lvm_snapshots_prefix, 1),
	f_origin_volume(LvmGeneralFixture::f_conf_lvm_vg_name, LvmGeneralFixture::f_conf_lvm_origin_lv_name, "test_clone_snapshot_01"),
	f_clone_valid_metadata(f_origin_volume.subvolume(), snapper::ImportPolicy::CLONE, f_lvm)
    {
    }


    CloneSnapshot::~CloneSnapshot()
    {
	try
	{
	    lvremove_wrapper(LvmGeneralFixture::f_conf_lvm_vg_name, f_lvm->snapshotLvName(f_env.f_num));
	}
	catch (const ImportTestsuiteException &e)
	{
	    std::cerr << "!!! there is no cloned snapshot "
		      << f_origin_volume.vg_name << "/"
		      << f_lvm->snapshotLvName(f_env.f_num) << std::endl;
	}
    }


    GetRawData::GetRawData()
	: ValidMetadata(),
	f_lvm_import_metadata(f_raw_data, snapper::ImportPolicy::CLONE, f_dummy_lvm)
    {
    }


    GetSnapshotDir::GetSnapshotDir()
	: ValidMetadata(), LvmGeneralFixture(),
	f_lvm_import_metadata(f_raw_data, snapper::ImportPolicy::ADOPT, f_lvm)
    {
    }

}}
