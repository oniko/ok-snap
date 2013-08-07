
#include "snapper/LvmImportMetadata.h"

#include "testsuite-import/lvm-snapshot-fixtures.h"
#include "testsuite-import/lvm-fixtures.h"

namespace testsuiteimport { namespace lvm 
{
    GeneralSnapshotCtorFixture
    LvmSnapshotFixtures::ctor_fixture() const
    {
	struct GeneralSnapshotCtorFixture gfix(f_snapper);

	gfix.clone_data = new ::snapper::LvmImportMetadata("dummy_vg/dummy_lv", ::snapper::ImportPolicy::CLONE, f_lvm);
	gfix.adopt_data = new ::snapper::LvmImportMetadata("dummy_vg/dummy_lv", ::snapper::ImportPolicy::ADOPT, f_lvm);
	gfix.ack_data = new ::snapper::LvmImportMetadata("dummy_vg/dummy_lv", ::snapper::ImportPolicy::ACKNOWLEDGE, f_lvm);

	return gfix;
    }


    GeneralGetImportPolicyFixture
    LvmSnapshotFixtures::get_import_policy_fixture() const
    {
	struct GeneralGetImportPolicyFixture gfix(f_snapper);

	gfix.clone_data = new ::snapper::LvmImportMetadata("dummy_vg/dummy_lv", ::snapper::ImportPolicy::CLONE, f_lvm);
	gfix.adopt_data = new ::snapper::LvmImportMetadata("dummy_vg/dummy_lv", ::snapper::ImportPolicy::ADOPT, f_lvm);
	gfix.ack_data = new ::snapper::LvmImportMetadata("dummy_vg/dummy_lv", ::snapper::ImportPolicy::ACKNOWLEDGE, f_lvm);

	return gfix;
    }


    GeneralGetSnapshotDirFixture
    LvmSnapshotFixtures::get_snapshot_dir_fixture() const
    {
	struct GeneralGetSnapshotDirFixture gfix(f_snapper);

	gfix.clone_data = new ::snapper::LvmImportMetadata("dummy_vg/dummy_lv", ::snapper::ImportPolicy::CLONE, f_lvm);
	gfix.adopt_data = new ::snapper::LvmImportMetadata("dummy_vg/dummy_lv", ::snapper::ImportPolicy::ADOPT, f_lvm);
	gfix.ack_data = new ::snapper::LvmImportMetadata("dummy_vg/dummy_lv", ::snapper::ImportPolicy::ACKNOWLEDGE, f_lvm);

	gfix.num_none = 1;
	gfix.num_clone = 2;
	gfix.num_adopt = 3;
	gfix.num_ack = 5;

	std::ostringstream oss(std::ostringstream::ate);

	oss << "/" << gfix.num_none;
	gfix.expected_none = LvmGeneralFixture::f_conf_lvm_snapshots_prefix + oss.str() + "/snapshot";

	oss.clear();
	oss.str("/");
	oss << gfix.num_clone;
	gfix.expected_clone = LvmGeneralFixture::f_conf_lvm_snapshots_prefix + oss.str() + "/snapshot";

	oss.clear();
	oss.str("/");
	oss << gfix.num_adopt;
	gfix.expected_adopt = LvmGeneralFixture::f_conf_lvm_snapshots_prefix + oss.str() + "/snapshot";

	oss.clear();
	oss.str("/");
	oss << gfix.num_ack;
	gfix.expected_ack = LvmGeneralFixture::f_conf_lvm_snapshots_prefix + oss.str() + "/snapshot";

	return gfix;
    }


    GeneralMountFilesystemFixture
    LvmSnapshotFixtures::mount_filesystem_fixture() const
    {
	struct GeneralMountFilesystemFixture gfix(f_snapper);

	//gfix.infos_dir = LvmGeneralFixture::f_conf_lvm_snapshots_prefix;

	// import type NONE (w/ default info dir and snapshot)
	std::auto_ptr<LvmSubvolumeWrapper> sn(new LvmSubvolumeWrapper(LvmGeneralFixture::f_conf_lvm_vg_name, LvmGeneralFixture::f_conf_lvm_origin_lv_name, 1, true));
	std::auto_ptr<LvmSubvolumeWrapper> snu(new LvmSubvolumeWrapper(LvmGeneralFixture::f_conf_lvm_vg_name, LvmGeneralFixture::f_conf_lvm_origin_lv_name, 1, true));

	// import type CLONE
	std::auto_ptr<LvmSubvolumeWrapper> sco(new LvmSubvolumeWrapper(LvmGeneralFixture::f_conf_lvm_vg_name, LvmGeneralFixture::f_conf_lvm_origin_lv_name, "mount_subvol_clone_orig", false));
	std::auto_ptr<LvmSubvolumeWrapper> sc(new LvmSubvolumeWrapper(LvmGeneralFixture::f_conf_lvm_vg_name, sco.get()->lv_name, 1, true));

	std::auto_ptr<LvmSubvolumeWrapper> scou(new LvmSubvolumeWrapper(LvmGeneralFixture::f_conf_lvm_vg_name, LvmGeneralFixture::f_conf_lvm_origin_lv_name, "mount_subvol_clone_orig_user", false));
	std::auto_ptr<LvmSubvolumeWrapper> scu(new LvmSubvolumeWrapper(LvmGeneralFixture::f_conf_lvm_vg_name, scou.get()->lv_name, 1, true));

	// import type ADOPT
	std::auto_ptr<LvmSubvolumeWrapper> sad(new LvmSubvolumeWrapper(LvmGeneralFixture::f_conf_lvm_vg_name, LvmGeneralFixture::f_conf_lvm_origin_lv_name, "mount_subvol_adopt", 1, true));
	std::auto_ptr<LvmSubvolumeWrapper> sadu(new LvmSubvolumeWrapper(LvmGeneralFixture::f_conf_lvm_vg_name, LvmGeneralFixture::f_conf_lvm_origin_lv_name, "mount_subvol_adopt_user", 1, true));

	// import type ACKNOWLEDGE
	std::auto_ptr<LvmSubvolumeWrapper> sack(new LvmSubvolumeWrapper(LvmGeneralFixture::f_conf_lvm_vg_name, LvmGeneralFixture::f_conf_lvm_origin_lv_name, "mount_subvol_ack", 1, true));
	std::auto_ptr<LvmSubvolumeWrapper> sacku(new LvmSubvolumeWrapper(LvmGeneralFixture::f_conf_lvm_vg_name, LvmGeneralFixture::f_conf_lvm_origin_lv_name, "mount_subvol_ack_user", 1, true));

	std::cout << "wtf!: " << sc.get()->vg_name << "/" << sc.get()->lv_orig_name << std::endl;
	std::auto_ptr<snapper::LvmImportMetadata> imc(new snapper::LvmImportMetadata(sc.get()->vg_name + "/" + sc.get()->lv_orig_name, ::snapper::ImportPolicy::CLONE, f_lvm));
	std::cout << "wtf!: " << scu.get()->vg_name << "/" << scu.get()->lv_orig_name << std::endl;
	std::auto_ptr<snapper::LvmImportMetadata> imcu(new snapper::LvmImportMetadata(scu.get()->vg_name + "/" + scu.get()->lv_orig_name, ::snapper::ImportPolicy::CLONE, f_lvm));

	std::cout << "clone done" << std::endl;
	std::auto_ptr<snapper::LvmImportMetadata> imad(new snapper::LvmImportMetadata(sad.get()->subvolume(), ::snapper::ImportPolicy::ADOPT, f_lvm));
	std::auto_ptr<snapper::LvmImportMetadata> imadu(new snapper::LvmImportMetadata(sadu.get()->subvolume(), ::snapper::ImportPolicy::ADOPT, f_lvm));

	std::cout << "adopt done" << std::endl;
	std::auto_ptr<snapper::LvmImportMetadata> imack(new snapper::LvmImportMetadata(sack.get()->subvolume(), ::snapper::ImportPolicy::ACKNOWLEDGE, f_lvm));
	std::auto_ptr<snapper::LvmImportMetadata> imacku(new snapper::LvmImportMetadata(sacku.get()->subvolume(), ::snapper::ImportPolicy::ACKNOWLEDGE, f_lvm));

	std::cout << "ack done" << std::endl;
	gfix.subvol_none = sn.release();
	gfix.subvol_none_user = snu.release();

	gfix.subvol_clone = sc.release();
	gfix.subvol_clone_orig = sco.release();
	gfix.subvol_clone_user = scu.release();
	gfix.subvol_clone_orig_user = scou.release();

	gfix.subvol_adopt = sad.release();
	gfix.subvol_adopt_user = sadu.release();

	gfix.subvol_ack = sack.release();
	gfix.subvol_ack_user = sacku.release();

	gfix.im_clone = imc.release();
	gfix.im_clone_user = imcu.release();
	gfix.im_adopt = imad.release();
	gfix.im_adopt_user = imadu.release();
	gfix.im_ack = imack.release();
	gfix.im_ack_user = imacku.release();

	return gfix;
    }

    
}}
