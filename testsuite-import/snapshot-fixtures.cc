#include <errno.h>
#include <stdio.h>
#include <sys/mount.h>

#include <iostream>
#include <sstream>

#include <boost/test/unit_test.hpp>

#include "testsuite-import/snapshot-fixtures.h"
#include "testsuite-import/helpers.h"

#include "snapper/LvmImportMetadata.h"
#include "snapper/Snapshot.h"

namespace testsuiteimport { namespace lvm
{
    using std::make_pair;


    GeneralSnapshotCtorFixture
    LvmSnapshotFixtures::ctor_fixture() const
    {
	struct GeneralSnapshotCtorFixture gfix(snapper);

	gfix.clone_data = new ::snapper::LvmImportMetadata("dummy_vg", "dummy_lv", ::snapper::ImportPolicy::CLONE, snapper->getFilesystem());
	gfix.adopt_data = new ::snapper::LvmImportMetadata("dummy_vg", "dummy_lv", ::snapper::ImportPolicy::ADOPT, snapper->getFilesystem());
	gfix.ack_data = new ::snapper::LvmImportMetadata("dummy_vg", "dummy_lv", ::snapper::ImportPolicy::ACKNOWLEDGE, snapper->getFilesystem());

	return gfix;
    }


    GeneralGetImportPolicyFixture
    LvmSnapshotFixtures::get_import_policy_fixture() const
    {
	struct GeneralGetImportPolicyFixture gfix(snapper);

	gfix.clone_data = new ::snapper::LvmImportMetadata("dummy_vg", "dummy_lv", ::snapper::ImportPolicy::CLONE, snapper->getFilesystem());
	gfix.adopt_data = new ::snapper::LvmImportMetadata("dummy_vg", "dummy_lv", ::snapper::ImportPolicy::ADOPT, snapper->getFilesystem());
	gfix.ack_data = new ::snapper::LvmImportMetadata("dummy_vg", "dummy_lv", ::snapper::ImportPolicy::ACKNOWLEDGE, snapper->getFilesystem());

	return gfix;
    }


    GeneralGetSnapshotDirFixture
    LvmSnapshotFixtures::get_snapshot_dir_fixture() const
    {
	struct GeneralGetSnapshotDirFixture gfix(snapper);

	gfix.clone_data = new ::snapper::LvmImportMetadata("dummy_vg", "dummy_lv", ::snapper::ImportPolicy::CLONE, snapper->getFilesystem());
	gfix.adopt_data = new ::snapper::LvmImportMetadata("dummy_vg", "dummy_lv", ::snapper::ImportPolicy::ADOPT, snapper->getFilesystem());
	gfix.ack_data = new ::snapper::LvmImportMetadata("dummy_vg", "dummy_lv", ::snapper::ImportPolicy::ACKNOWLEDGE, snapper->getFilesystem());

	gfix.num_none = 1;
	gfix.num_clone = 2;
	gfix.num_adopt = 3;
	gfix.num_ack = 5;

	std::ostringstream oss(std::ostringstream::ate);

	oss << gfix.num_none;
	gfix.expected_none = LvmGeneralFixture::f_conf_lvm_snapshots_prefix + oss.str() + "/snapshot";

	oss.clear();
	oss.str(gfix.num_clone);
	gfix.expected_clone = LvmGeneralFixture::f_conf_lvm_snapshots_prefix + oss.str() + "/snapshot";

	oss.clear();
	oss.str(gfix.num_adopt);
	gfix.expected_adopt = LvmGeneralFixture::f_conf_lvm_snapshots_prefix + oss.str() + "/snapshot";

	oss.clear();
	oss.str(gfix.num_ack);
	gfix.expected_ack = LvmGeneralFixture::f_conf_lvm_snapshots_prefix + oss.str() + "/snapshot";
    }

        struct GeneralGetSnapshotDirFixture : public GeneralSnapshotCtorFixture
    {
	GeneralGetSnapshotDirFixture(const ::snapper::Snapper* snapper)
	    : GeneralSnapshotCtorFixture(snapper) {}

	unsigned int num_none;
	unsigned int num_clone;
	unsigned int num_adopt;
	unsigned int num_ack;

	string expected_none;
	string expected_clone;
	string expected_adopt;
	string expected_ack;
    };


//     struct GeneralMountFilesystemFixture
//     {
// 	GeneralMountFilesystemFixture(const ::snapper::Snapper* snapper)
// 	    : snapper(snapper) {}
// 
// 	const ::snapper::Snapper* snapper;
// 	string infos_dir;
// 
// 	SubvolumeWrapper* subvol_none;
// 	SubvolumeWrapper* subvol_none_user;
// 
// 	SubvolumeWrapper* subvol_clone_orig;
// 	SubvolumeWrapper* subvol_clone;
// 	SubvolumeWrapper* subvol_clone_orig_user;
// 	SubvolumeWrapper* subvol_clone_user;
// 
// 	SubvolumeWrapper* subvol_adopt;
// 	SubvolumeWrapper* subvol_adopt_user;
// 
// 	SubvolumeWrapper* subvol_ack;
// 	SubvolumeWrapper* subvol_ack_user;
// 
// 	/*
// 	 * NOTE: following memory will be freed by Snapshot dtor.
// 	 * 	 Do not pass pter to more than one Snapshot ctor!
// 	 */
// 	::snapper::ImportMetadata* im_clone;
// 	::snapper::ImportMetadata* im_clone_user;
// 
// 	::snapper::ImportMetadata* im_adopt;
// 	::snapper::ImportMetadata* im_adopt_user;
// 
// 	::snapper::ImportMetadata* im_ack;
// 	::snapper::ImportMetadata* im_ack_user;
//     };


    GeneralMountFilesystemFixture
    LvmSnapshotFixtures::mount_filesystem_fixture() const
    {
	const snapper::Lvm* f_lvm = static_cast<snapper::Lvm *>(snapper->getFilesystem());

	struct GeneralMountFilesystemFixture gfix(snapper);

	gfix.infos_dir = LvmGeneralFixture::f_conf_lvm_snapshots_prefix;

	// import type NONE (w/ default info dir and snapshot)
	std::auto_ptr<LvmSubvolumeWrapper> sn(new LvmSubvolumeWrapper(LvmGeneralFixture::f_conf_lvm_vg_name, LvmGeneralFixture::f_conf_lvm_origin_lv_name, 1));
	std::auto_ptr<LvmSubvolumeWrapper> snu(new LvmSubvolumeWrapper(LvmGeneralFixture::f_conf_lvm_vg_name, LvmGeneralFixture::f_conf_lvm_origin_lv_name, 1));

	// import type CLONE
	std::auto_ptr<LvmSubvolumeWrapper> sco(new LvmSubvolumeWrapper(LvmGeneralFixture::f_conf_lvm_vg_name, LvmGeneralFixture::f_conf_lvm_origin_lv_name, "mount_subvol_clone_orig", false));
	std::auto_ptr<LvmSubvolumeWrapper> sc(new LvmSubvolumeWrapper(LvmGeneralFixture::f_conf_lvm_vg_name, sco.get()->lv_name, 1));

	std::auto_ptr<LvmSubvolumeWrapper> scou(new LvmSubvolumeWrapper(LvmGeneralFixture::f_conf_lvm_vg_name, LvmGeneralFixture::f_conf_lvm_origin_lv_name, "mount_subvol_clone_orig_user", false));
	std::auto_ptr<LvmSubvolumeWrapper> scu(new LvmSubvolumeWrapper(LvmGeneralFixture::f_conf_lvm_vg_name, scou.get()->lv_name, 1));

	// import type ADOPT
	std::auto_ptr<LvmSubvolumeWrapper> sad(new LvmSubvolumeWrapper(LvmGeneralFixture::f_conf_lvm_vg_name, LvmGeneralFixture::f_conf_lvm_origin_lv_name, "mount_subvol_adopt", 1));
	std::auto_ptr<LvmSubvolumeWrapper> sadu(new LvmSubvolumeWrapper(LvmGeneralFixture::f_conf_lvm_vg_name, LvmGeneralFixture::f_conf_lvm_origin_lv_name, "mount_subvol_adopt_user", 1));

	// import type ACKNOWLEDGE
	std::auto_ptr<LvmSubvolumeWrapper> sack(new LvmSubvolumeWrapper(LvmGeneralFixture::f_conf_lvm_vg_name, LvmGeneralFixture::f_conf_lvm_origin_lv_name, "mount_subvol_ack", 1));
	std::auto_ptr<LvmSubvolumeWrapper> sacku(new LvmSubvolumeWrapper(LvmGeneralFixture::f_conf_lvm_vg_name, LvmGeneralFixture::f_conf_lvm_origin_lv_name, "mount_subvol_ack_user", 1));

	std::auto_ptr<snapper::LvmImportMetadata> imc(new snapper::LvmImportMetadata(sc.get()->vg_name, sc.get()->lv_orig_name, f_lvm));
	std::auto_ptr<snapper::LvmImportMetadata> imcu(new snapper::LvmImportMetadata(scu.get()->vg_name, scu.get()->lv_orig_name, f_lvm));

	std::auto_ptr<snapper::LvmImportMetadata> imad(new snapper::LvmImportMetadata(sad.get()->vg_name, sad.get()->lv_name, f_lvm));
	std::auto_ptr<snapper::LvmImportMetadata> imadu(new snapper::LvmImportMetadata(sadu.get()->vg_name, sadu.get()->lv_name, f_lvm));

	std::auto_ptr<snapper::LvmImportMetadata> imack(new snapper::LvmImportMetadata(sack.get()->vg_name, sack.get()->lv_name, f_lvm));
	std::auto_ptr<snapper::LvmImportMetadata> imacku(new snapper::LvmImportMetadata(sacku.get()->vg_name, sacku.get()->lv_name, f_lvm));

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


//     DeleteFilesystemSnapshotImportTypeNone::DeleteFilesystemSnapshotImportTypeNone()
// 	: InfoDirWithSnapshotDir(1),
// 	f_sh(f_snapper, snapper::SnapshotType::SINGLE, f_num, (time_t) -1, NULL),
// 	f_snapshot_lv_name(f_lvm->snapshotLvName(f_num))
//     {
// 	lvcreate_thin_snapshot_wrapper(f_conf_lvm_vg_name, f_conf_lvm_origin_lv_name, f_snapshot_lv_name);
//     }
// 
//     DeleteFilesystemSnapshotImportTypeNone::~DeleteFilesystemSnapshotImportTypeNone()
//     {
// 	try
// 	{
// 	    lvremove_wrapper(f_conf_lvm_vg_name, f_lvm->snapshotLvName(f_num) );
// 	    std::cerr << "DeleteFilesystemSnapshotImportTypeNone dtor: "
// 		      << f_conf_lvm_vg_name << "/" << f_lvm->snapshotLvName(f_num)
// 		      << " has not been removed" << std::endl;
// 	}
// 	catch (const LvmImportTestsuiteException &e) {}
//     }
// 
//     DeleteFilesystemSnapshotImportTypeClone::DeleteFilesystemSnapshotImportTypeClone()
// 	: InfoDirWithSnapshotDir(1), f_snapshot_lv_name(f_lvm->snapshotLvName(f_num)),
// 	rm(f_conf_lvm_vg_name, f_snapshot_lv_name), f_p_idata(new snapper::LvmImportMetadata(rm.f_raw_data, snapper::ImportPolicy::CLONE, f_lvm)),
// 	f_sh(f_snapper, snapper::SnapshotType::SINGLE, f_num, (time_t) -1, f_p_idata)
//     {
// 	lvcreate_thin_snapshot_wrapper(f_conf_lvm_vg_name, f_conf_lvm_origin_lv_name, f_snapshot_lv_name);
//     }
// 
//     DeleteFilesystemSnapshotImportTypeClone::~DeleteFilesystemSnapshotImportTypeClone()
//     {
// 	try
// 	{
// 	    lvremove_wrapper(f_conf_lvm_vg_name, f_lvm->snapshotLvName(f_num) );
// 	    std::cerr << "DeleteFilesystemSnapshotImportTypeClone dtor: "
// 		      << f_conf_lvm_vg_name << "/" << f_lvm->snapshotLvName(f_num)
// 		      << " has not been removed" << std::endl;
// 	}
// 	catch (const LvmImportTestsuiteException &e) {}
//     }
// 
//     DeleteFilesystemSnapshotImportTypeAdopt::DeleteFilesystemSnapshotImportTypeAdopt()
// 	: InfoDirWithSnapshotDir(1), f_snapshot_lv_name(f_test_snapshot_01),
// 	rm(f_conf_lvm_vg_name, f_snapshot_lv_name), f_p_idata(new snapper::LvmImportMetadata(rm.f_raw_data, snapper::ImportPolicy::ADOPT, f_lvm)),
// 	f_sh(f_snapper, snapper::SnapshotType::SINGLE, f_num, (time_t) -1, f_p_idata)
//     {
// 	lvcreate_thin_snapshot_wrapper(f_conf_lvm_vg_name, f_conf_lvm_origin_lv_name, f_snapshot_lv_name);
//     }
// 
//     DeleteFilesystemSnapshotImportTypeAdopt::~DeleteFilesystemSnapshotImportTypeAdopt()
//     {
// 	try
// 	{
// 	    lvremove_wrapper(f_conf_lvm_vg_name, f_snapshot_lv_name);
// 	    std::cerr << "DeleteFilesystemSnapshotImportTypeAdopt dtor: "
// 		      << f_conf_lvm_vg_name << "/" << f_lvm->snapshotLvName(f_num)
// 		      << " has not been removed" << std::endl;
// 	}
// 	catch (const LvmImportTestsuiteException &e) {}
//     }
// 
//     DeleteFilesystemSnapshotImportTypeAcknowledge::DeleteFilesystemSnapshotImportTypeAcknowledge()
// 	: InfoDirWithSnapshotDir(1), f_snapshot_lv_name(f_test_snapshot_01),
// 	rm(f_conf_lvm_vg_name, f_snapshot_lv_name), f_p_idata(new snapper::LvmImportMetadata(rm.f_raw_data, snapper::ImportPolicy::ACKNOWLEDGE, f_lvm)),
// 	f_sh(f_snapper, snapper::SnapshotType::SINGLE, f_num, (time_t) -1, f_p_idata)
//     {
// 	lvcreate_thin_snapshot_wrapper(f_conf_lvm_vg_name, f_conf_lvm_origin_lv_name, f_snapshot_lv_name);
//     }
// 
//     DeleteFilesystemSnapshotImportTypeAcknowledge::~DeleteFilesystemSnapshotImportTypeAcknowledge()
//     {
// 	try
// 	{
// 	    lvremove_wrapper(f_conf_lvm_vg_name, f_snapshot_lv_name);
// 	}
// 	catch (const LvmImportTestsuiteException &e)
// 	{
// 	    std::cerr << "DeleteFilesystemSnapshotImportTypeAcknowledge dtor: "
// 		      << f_conf_lvm_vg_name << "/" << f_lvm->snapshotLvName(f_num)
// 		      << " has been removed!" << std::endl;
// 	}
//     }
// 
//     DeleteFileSystemSnapshotOrigin::DeleteFileSystemSnapshotOrigin()
// 	: LvmGeneralFixture(),
// 	f_sh(f_snapper, snapper::SnapshotType::SINGLE, 0, (time_t) -1, NULL)
//     {
//     }

    MountFileSystemSnapshotSimpleBase::MountFileSystemSnapshotSimpleBase()
	: InfoDirWithSnapshotDir(1),
	f_snapshot_lv_name(f_lvm->snapshotLvName(f_num))
    {
	lvcreate_thin_snapshot_wrapper(f_conf_lvm_vg_name, f_conf_lvm_origin_lv_name, f_snapshot_lv_name);

	std::ostringstream oss;
	oss << f_conf_lvm_snapshots_prefix << f_num << "/snapshot";

	f_mountpoint = oss.str();
    }

    MountFileSystemSnapshotSimpleBase::~MountFileSystemSnapshotSimpleBase()
    {
	if (umount2(f_mountpoint.c_str(), MNT_DETACH))
	    std::cerr << "umount2( \"" << f_mountpoint << "\", MNT_DETACH) failed!" << std::endl;

	try
	{
	    lvremove_wrapper(f_conf_lvm_vg_name, f_snapshot_lv_name );
	}
	catch (const LvmImportTestsuiteException &e)
	{
	    std::cerr << "lvremove_wrapper( " << f_conf_lvm_vg_name << ", " << f_snapshot_lv_name << " ) failed" << std::endl;
	}
    }

    MountFileSystemSnapshotImportNone::MountFileSystemSnapshotImportNone()
	: MountFileSystemSnapshotSimpleBase(),
	f_sh(f_snapper, snapper::SnapshotType::SINGLE, f_num, (time_t) -1, NULL)
    {
	std::cout << "MountFileSystemSnapshotImportNone ctor" << std::endl
		  << "vg/lv: " << f_conf_lvm_vg_name << "/" << f_snapshot_lv_name << std::endl;
    }

    MountFileSystemSnapshotImportClone::MountFileSystemSnapshotImportClone()
	: MountFileSystemSnapshotSimpleBase(),
	f_clone_origin_name("lv_clone_origin_name"),
	rm(f_conf_lvm_vg_name, f_clone_origin_name),
	f_p_idata(new snapper::LvmImportMetadata(rm.f_raw_data, snapper::ImportPolicy::CLONE, f_lvm)),
	f_sh(f_snapper, snapper::SnapshotType::SINGLE, f_num, (time_t)-1, f_p_idata)
    {
	lvcreate_thin_snapshot_wrapper(f_conf_lvm_vg_name, f_conf_lvm_origin_lv_name, f_clone_origin_name);
    }

    MountFileSystemSnapshotImportClone::~MountFileSystemSnapshotImportClone()
    {
	try
	{
	    lvremove_wrapper(f_conf_lvm_vg_name, f_clone_origin_name);
	}
	catch (const LvmImportTestsuiteException &e)
	{
	    std::cerr << "lvremove_wrapper( " << f_conf_lvm_vg_name << ", " << f_clone_origin_name << " ) failed" << std::endl;
	}
    }

    MountFileSystemSnapshotImportBase::MountFileSystemSnapshotImportBase()
	: InfoDirWithSnapshotDir(1),
	f_snapshot_lv_name("lv_test_import_snapshot")
    {
	lvcreate_thin_snapshot_wrapper(f_conf_lvm_vg_name, f_conf_lvm_origin_lv_name, f_snapshot_lv_name);

	std::ostringstream oss;
	oss << f_conf_lvm_snapshots_prefix << f_num << "/snapshot";

	f_mountpoint = oss.str();
    }

    MountFileSystemSnapshotImportBase::~MountFileSystemSnapshotImportBase()
    {
	if (umount2(f_mountpoint.c_str(), MNT_DETACH))
		std::cerr << "umount2( \"" << f_mountpoint << "\", MNT_DETACH) failed!" << std::endl;

	try
	{
	    lvremove_wrapper(f_conf_lvm_vg_name, f_snapshot_lv_name );
	}
	catch (const LvmImportTestsuiteException &e)
	{
	    std::cerr << "lvremove_wrapper( " << f_conf_lvm_vg_name << ", " << f_snapshot_lv_name << " ) failed" << std::endl;
	}
    }

    MountFileSystemSnapshotImportAdopt::MountFileSystemSnapshotImportAdopt()
	: MountFileSystemSnapshotImportBase(),
	rm(f_conf_lvm_vg_name, f_snapshot_lv_name),
	f_p_idata(new snapper::LvmImportMetadata(rm.f_raw_data, snapper::ImportPolicy::ADOPT, f_lvm)),
	f_sh(f_snapper, snapper::SnapshotType::SINGLE, f_num, (time_t) -1, f_p_idata)
    {
    }

    MountFileSystemSnapshotImportAck::MountFileSystemSnapshotImportAck()
	: MountFileSystemSnapshotImportBase(), rm(f_conf_lvm_vg_name, f_snapshot_lv_name),
	f_p_idata(new snapper::LvmImportMetadata(rm.f_raw_data, snapper::ImportPolicy::ACKNOWLEDGE, f_lvm)),
	f_sh(f_snapper, snapper::SnapshotType::SINGLE, f_num, (time_t) -1, f_p_idata)
    {
    }

    UmountFilesystemSnapshotBase::UmountFilesystemSnapshotBase(const string& dev, const string& mount_point, const string& mount_type)
	: f_dev_path(dev)
    {
	std::cout << "UmountFilesystemSnapshotBase ctor" << std::endl;
	string type = mount_type;

	string::size_type open = mount_type.find("(");
	string::size_type close = mount_point.find(")");
	if (open != string::npos && close != string::npos)
	    type = mount_point.substr(open + 1, close - open - 1);

	int ret = mount(f_dev_path.c_str(), mount_point.c_str(),
			type.c_str(),
			MS_NOATIME | MS_NODEV | MS_NOEXEC | MS_RDONLY,
			NULL);

	if (ret)
	{
	    perror("mount");
	    BOOST_FAIL( "Can't mount filesystem for testing purposes: \"" <<
			f_dev_path << "\" -> \"" << mount_point << "\"");
	}
    }

    UmountFilesystemSnapshotImportNone::UmountFilesystemSnapshotImportNone()
	: MountFileSystemSnapshotImportNone(),
	UmountFilesystemSnapshotBase("/dev/" + f_conf_lvm_vg_name + "/" + f_snapshot_lv_name, f_mountpoint, f_lvm->fstype())
    {
	f_sh.mount_use_count = 1;
    }

    UmountFilesystemSnapshotImportClone::UmountFilesystemSnapshotImportClone()
	: MountFileSystemSnapshotImportClone(),
	UmountFilesystemSnapshotBase("/dev/" + f_conf_lvm_vg_name + "/" + f_snapshot_lv_name, f_mountpoint, f_lvm->fstype()),
	f_dev_origin_path("/dev/" + f_conf_lvm_vg_name + "/" + f_clone_origin_name),
	f_origin_mount_point(f_info_dir + "/tmp_mnt_point")
    {
	std::cout << "UmountFilesystemSnapshotImportClone ctor" << std::endl;

	if (mkdirat(f_dirfd, "tmp_mnt_point", 0755))
	    BOOST_FAIL( "Can't tmp_mnt_point directory in test environment" );

	string::size_type open = f_lvm->fstype().find("(");
	string::size_type close = f_lvm->fstype().find(")");

	int ret = mount(f_dev_origin_path.c_str(), f_origin_mount_point.c_str(),
			f_lvm->fstype().substr(open + 1, close - open - 1).c_str(),
			MS_NOATIME | MS_NODEV | MS_NOEXEC | MS_RDONLY,
			NULL);

	if (ret)
	{
	    perror("mount");
	    BOOST_FAIL( "Can't mount filesystem for testing purposes: \"" <<
			f_dev_origin_path << "\" -> \"" << f_origin_mount_point << "\"");
	}

	f_sh.mount_use_count = 1;
    }

    UmountFilesystemSnapshotImportClone::~UmountFilesystemSnapshotImportClone()
    {
	// NOTE: the f_origin_mount_point will deleted by CreateSnapshotEnvironment dtor
	std::cout << "UmountFilesystemSnapshotImportClone dtor" << std::endl;

	if (umount2(f_origin_mount_point.c_str(), MNT_DETACH))
		std::cerr << "umount2( \"" << f_origin_mount_point << "\", MNT_DETACH) failed!" << std::endl;
    }

    UmountFilesystemSnapshotImportAdopt::UmountFilesystemSnapshotImportAdopt()
	: MountFileSystemSnapshotImportAdopt(),
	UmountFilesystemSnapshotBase("/dev/" + f_conf_lvm_vg_name + "/" + f_snapshot_lv_name, f_mountpoint, f_lvm->fstype())
    {
	f_sh.mount_use_count = 1;
    }

    UmountFilesystemSnapshotImportAck::UmountFilesystemSnapshotImportAck()
	: MountFileSystemSnapshotImportAck(),
	UmountFilesystemSnapshotBase("/dev/" + f_conf_lvm_vg_name + "/" + f_snapshot_lv_name, f_mountpoint, f_lvm->fstype())
    {
	f_sh.mount_use_count = 1;
    }
}}
