#include <iostream>

#include "testsuite-import/snapshots-fixtures.h"

#include "testsuite-import/helpers.h"

namespace testsuiteimport { namespace lvm
{
    ImportHelperTypeNone::ImportHelperTypeNone()
	: f_dummy_snapper(static_cast<const snapper::Snapper *>(135790)),
	f_shs(f_dummy_snapper),
	f_sh(f_dummy_snapper, snapper::SnapshotType::SINGLE, 42, (time_t) -1)
    {
    }

    ImportHelperTypeCloneValid::ImportHelperTypeCloneValid()
	: CloneSnapshotValid(),
	f_p_idata(new snapper::LvmImportMetadata(f_conf_lvm_vg_name, f_lv_name)),
	f_shs(f_snapper),
	f_sh(f_snapper, snapper::SnapshotType::SINGLE, f_num, (time_t) -1, snapper::ImportPolicy::CLONE, f_p_idata)
    {
    }

    ImportHelperTypeCloneInvalid::ImportHelperTypeCloneInvalid()
	: CloneSnapshotMissingOrigin(),
	f_p_idata(new snapper::LvmImportMetadata(f_conf_lvm_vg_name, f_lv_name)),
	f_shs(f_snapper),
	f_sh(f_snapper, snapper::SnapshotType::SINGLE, f_num, (time_t) -1, snapper::ImportPolicy::CLONE, f_p_idata)
    {
    }

    ImportHelperTypeAdoptValid::ImportHelperTypeAdoptValid()
	: CloneSnapshotValid(),
	f_p_idata(new snapper::LvmImportMetadata(f_conf_lvm_vg_name, f_lv_name)),
	f_shs(f_snapper),
	f_sh(f_snapper, snapper::SnapshotType::SINGLE, f_num, (time_t) -1, snapper::ImportPolicy::ADOPT, f_p_idata)
    {
    }

    ImportHelperTypeAdoptInvalid::ImportHelperTypeAdoptInvalid()
	: CloneSnapshotMissingOrigin(),
	f_p_idata(new snapper::LvmImportMetadata(f_conf_lvm_vg_name, f_lv_name)),
	f_shs(f_snapper),
	f_sh(f_snapper, snapper::SnapshotType::SINGLE, f_num, (time_t) -1, snapper::ImportPolicy::ADOPT, f_p_idata)
    {
    }

    ImportHelperTypeAdoptAlreadyImported::ImportHelperTypeAdoptAlreadyImported()
	: ImportHelperTypeAdoptValid(),
	f_lv_already_acked("lv_supposed_to_be_acked"),
	f_p_idata_adopted(new snapper::LvmImportMetadata(f_conf_lvm_vg_name, f_lv_name)),
	f_sh_adopted(f_snapper, snapper::SnapshotType::SINGLE, 42, (time_t) -1, snapper::ImportPolicy::ADOPT, f_p_idata_adopted),
	f_p_idata_acked(new snapper::LvmImportMetadata(f_conf_lvm_vg_name, f_lv_already_acked)),
	f_sh_acked(f_snapper, snapper::SnapshotType::SINGLE, 24, (time_t) -1, snapper::ImportPolicy::ACKNOWLEDGE, f_p_idata_acked),
	f_p_idata_2(new snapper::LvmImportMetadata(f_conf_lvm_vg_name, f_lv_already_acked)),
	f_sh_2(f_snapper, snapper::SnapshotType::SINGLE, 24, (time_t) -1, snapper::ImportPolicy::ACKNOWLEDGE, f_p_idata_2)
    {
	lvcreate_thin_snapshot_wrapper(f_conf_lvm_vg_name, f_conf_lvm_origin_lv_name, f_lv_already_acked);

	f_shs.entries.insert(f_shs.entries.end(), f_sh_adopted);
	f_shs.entries.insert(f_shs.entries.end(), f_sh_acked);
    }

    ImportHelperTypeAdoptAlreadyImported::~ImportHelperTypeAdoptAlreadyImported()
    {
	try
	{
	    lvremove_wrapper(f_conf_lvm_vg_name, f_lv_already_acked);
	}
	catch (const LvmImportTestsuiteException& e)
	{
	    std::cerr << "can't remove helper LV: " << f_conf_lvm_vg_name << "/"
		      << f_lv_already_acked << std::endl;
	}
    }

    ImportHelperTypeAckValid::ImportHelperTypeAckValid()
	: CloneSnapshotValid(),
	f_p_idata(new snapper::LvmImportMetadata(f_conf_lvm_vg_name, f_lv_name)),
	f_shs(f_snapper),
	f_sh(f_snapper, snapper::SnapshotType::SINGLE, f_num, (time_t) -1, snapper::ImportPolicy::ACKNOWLEDGE, f_p_idata)
    {
    }

    ImportHelperTypeAckInvalid::ImportHelperTypeAckInvalid()
	: CloneSnapshotMissingOrigin(),
	f_p_idata(new snapper::LvmImportMetadata(f_conf_lvm_vg_name, f_lv_name)),
	f_shs(f_snapper),
	f_sh(f_snapper, snapper::SnapshotType::SINGLE, f_num, (time_t) -1, snapper::ImportPolicy::ACKNOWLEDGE, f_p_idata)
    {
    }

    ImportHelperTypeAckAlreadyImported::ImportHelperTypeAckAlreadyImported()
	: ImportHelperTypeAckValid(),
	f_lv_already_adopted("lv_supposed_to_be_adopted"),
	f_p_idata_adopted(new snapper::LvmImportMetadata(f_conf_lvm_vg_name, f_lv_already_adopted)),
	f_sh_adopted(f_snapper, snapper::SnapshotType::SINGLE, 42, (time_t) -1, snapper::ImportPolicy::ADOPT, f_p_idata_adopted),
	f_p_idata_acked(new snapper::LvmImportMetadata(f_conf_lvm_vg_name, f_lv_name)),
	f_sh_acked(f_snapper, snapper::SnapshotType::SINGLE, 24, (time_t) -1, snapper::ImportPolicy::ACKNOWLEDGE, f_p_idata_acked),
	f_p_idata_2(new snapper::LvmImportMetadata(f_conf_lvm_vg_name, f_lv_already_adopted)),
	f_sh_2(f_snapper, snapper::SnapshotType::SINGLE, 24, (time_t) -1, snapper::ImportPolicy::ACKNOWLEDGE, f_p_idata_2)
    {
	lvcreate_thin_snapshot_wrapper(f_conf_lvm_vg_name, f_conf_lvm_origin_lv_name, f_lv_already_adopted);

	f_shs.entries.insert(f_shs.entries.end(), f_sh_adopted);
	f_shs.entries.insert(f_shs.entries.end(), f_sh_acked);
    }

    ImportHelperTypeAckAlreadyImported::~ImportHelperTypeAckAlreadyImported()
    {
	try
	{
	    lvremove_wrapper(f_conf_lvm_vg_name, f_lv_already_adopted);
	}
	catch (const LvmImportTestsuiteException& e)
	{
	    std::cerr << "can't remove helper LV: " << f_conf_lvm_vg_name << "/"
		      << f_lv_already_adopted << std::endl;
	}
    }
}}
