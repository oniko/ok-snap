#include <memory>
#include <iostream>

#include "snapper/LvmImportMetadata.h"

#include "testsuite-import/lvm-ihelper-fixtures.h"
#include "testsuite-import/lvm-fixtures.h"

namespace testsuiteimport { namespace lvm
{
    typedef boost::shared_ptr<const LvmSubvolumeWrapper> shared_clvmsubv;

    ValidImportCheckFixtures
    LvmImportHelperFixFactory::import_valid_fixs() const
    {
	std::cout << "LvmImportHelperFixFactory entering" << std::endl;
	const shared_clvmsubv sv_clone_orig(new LvmSubvolumeWrapper(LvmGeneralFixture::f_conf_lvm_vg_name, LvmGeneralFixture::f_conf_lvm_origin_lv_name, "lvm-import-helper-clone-orig-01", 1, false));
	const shared_clvmsubv sv_adopt(new LvmSubvolumeWrapper(LvmGeneralFixture::f_conf_lvm_vg_name, LvmGeneralFixture::f_conf_lvm_origin_lv_name, "lvm-import-helper-adopt-01", 1, true));
	const shared_clvmsubv sv_ack(new LvmSubvolumeWrapper(LvmGeneralFixture::f_conf_lvm_vg_name, LvmGeneralFixture::f_conf_lvm_origin_lv_name, "lvm-import-helper-ack-01", 1, true));

	std::auto_ptr<const ::snapper::ImportMetadata> im_clone(new ::snapper::LvmImportMetadata(sv_clone_orig.get()->subvolume(), ::snapper::ImportPolicy::CLONE, f_lvm));
	std::auto_ptr<const ::snapper::ImportMetadata> im_adopt(new ::snapper::LvmImportMetadata(sv_adopt.get()->subvolume(), ::snapper::ImportPolicy::ADOPT, f_lvm));
	std::auto_ptr<const ::snapper::ImportMetadata> im_ack(new ::snapper::LvmImportMetadata(sv_ack.get()->subvolume(), ::snapper::ImportPolicy::ACKNOWLEDGE, f_lvm));

	std::cout << "LvmImportHelperFixFactory exiting" << std::endl;

	return ValidImportCheckFixtures(snapper, sv_clone_orig, sv_adopt, sv_ack, im_clone.release(), im_adopt.release(), im_ack.release());
    }


    ImportOriginCheckFixtures
    LvmImportHelperFixFactory::import_origin_fixs(::snapper::ImportPolicy origin) const
    {
	shared_cinfodir info_dir(new InfoDirWithSnapshotDir(LvmGeneralFixture::f_conf_lvm_snapshots_prefix, 1));
	std::auto_ptr<const ::snapper::ImportMetadata> im_origin(new ::snapper::LvmImportMetadata(LvmGeneralFixture::f_conf_lvm_vg_name + "/" + LvmGeneralFixture::f_conf_lvm_origin_lv_name, origin, f_lvm));

	return ImportOriginCheckFixtures(snapper, info_dir, im_origin.release());
    }


    ImportDupliciteCheckFixtures
    LvmImportHelperFixFactory::import_duplicite_fixs(snapper::ImportPolicy orig, snapper::ImportPolicy import) const
    {
	const shared_clvmsubv orig_vol(new LvmSubvolumeWrapper(LvmGeneralFixture::f_conf_lvm_vg_name, LvmGeneralFixture::f_conf_lvm_origin_lv_name, "duplicite_import_01"));
	const shared_cinfodir import_info_dir(new InfoDirWithSnapshotDir(LvmGeneralFixture::f_conf_lvm_snapshots_prefix, 1));

	const boost::shared_ptr<::snapper::ImportMetadata> shared_orig(new ::snapper::LvmImportMetadata(orig_vol.get()->subvolume(), orig , f_lvm));
	std::auto_ptr<::snapper::ImportMetadata> im_import(new ::snapper::LvmImportMetadata(orig_vol.get()->subvolume(), import , f_lvm));

	return ImportDupliciteCheckFixtures(snapper, orig_vol, import_info_dir, shared_orig, im_import.release());
    }


    ImportDupliciteClassicalFixtures
    LvmImportHelperFixFactory::import_classical_fixs(snapper::ImportPolicy import) const
    {
	::snapper::Snapper *tmp_snapper = const_cast<::snapper::Snapper *>(snapper);
	::snapper::Snapshots::iterator it = tmp_snapper->createSingleSnapshot("LvmImportHelperFixFactory::import_classical_fixs");

	try
	{
	    const shared_cinfodir import_infodir(new InfoDirWithSnapshotDir(LvmGeneralFixture::f_conf_lvm_snapshots_prefix, 1));
	    const ::snapper::ImportMetadata* idata = new ::snapper::LvmImportMetadata(LvmGeneralFixture::f_conf_lvm_vg_name + "/" + f_lvm->snapshotLvName(it->getNum()), import, f_lvm);

	    return ImportDupliciteClassicalFixtures(it, snapper, import_infodir, idata);
	}
	// TODO: catch proper exception
	catch (const std::exception& e)
	{
	    tmp_snapper->deleteSnapshot(it);
	    throw;
	}
    }

}}
