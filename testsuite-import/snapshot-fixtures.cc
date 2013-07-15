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

    CreateRawLvmImportMetata::CreateRawLvmImportMetata(const string &vg_name, const string& lv_name)
    {
	f_raw_data.insert(make_pair("vg_name", vg_name));
	f_raw_data.insert(make_pair("lv_name", lv_name));
    }

    SimpleConstructorValid::SimpleConstructorValid()
	: f_dummy_snapper(reinterpret_cast<const snapper::Snapper *>(123456789)),
	f_type(snapper::SnapshotType::PRE), f_num(42), f_date(1234554321)
    {
    }

    ImportConstructorValid::ImportConstructorValid()
	: SimpleConstructorValid(), rm("dummy_vg", "dummy_lv"),
	f_import_policy(snapper::ImportPolicy::ADOPT),
	f_dummy_lvm(reinterpret_cast<const snapper::Lvm *>(123456)),
	f_dummy_idata(new snapper::LvmImportMetadata(rm.f_raw_data, f_import_policy, f_dummy_lvm))
    {
    }

    DeleteFilesystemSnapshotImportTypeNone::DeleteFilesystemSnapshotImportTypeNone()
	: InfoDirWithSnapshotDir(1),
	f_sh(f_snapper, snapper::SnapshotType::SINGLE, f_num, (time_t) -1, NULL),
	f_snapshot_lv_name(f_lvm->snapshotLvName(f_num))
    {
	lvcreate_thin_snapshot_wrapper(f_conf_lvm_vg_name, f_conf_lvm_origin_lv_name, f_snapshot_lv_name);
    }

    DeleteFilesystemSnapshotImportTypeNone::~DeleteFilesystemSnapshotImportTypeNone()
    {
	try
	{
	    lvremove_wrapper(f_conf_lvm_vg_name, f_lvm->snapshotLvName(f_num) );
	    std::cerr << "DeleteFilesystemSnapshotImportTypeNone dtor: "
		      << f_conf_lvm_vg_name << "/" << f_lvm->snapshotLvName(f_num)
		      << " has not been removed" << std::endl;
	}
	catch (const LvmImportTestsuiteException &e) {}
    }

    DeleteFilesystemSnapshotImportTypeClone::DeleteFilesystemSnapshotImportTypeClone()
	: InfoDirWithSnapshotDir(1), f_snapshot_lv_name(f_lvm->snapshotLvName(f_num)),
	rm(f_conf_lvm_vg_name, f_snapshot_lv_name), f_p_idata(new snapper::LvmImportMetadata(rm.f_raw_data, snapper::ImportPolicy::CLONE, f_lvm)),
	f_sh(f_snapper, snapper::SnapshotType::SINGLE, f_num, (time_t) -1, f_p_idata)
    {
	lvcreate_thin_snapshot_wrapper(f_conf_lvm_vg_name, f_conf_lvm_origin_lv_name, f_snapshot_lv_name);
    }

    DeleteFilesystemSnapshotImportTypeClone::~DeleteFilesystemSnapshotImportTypeClone()
    {
	try
	{
	    lvremove_wrapper(f_conf_lvm_vg_name, f_lvm->snapshotLvName(f_num) );
	    std::cerr << "DeleteFilesystemSnapshotImportTypeClone dtor: "
		      << f_conf_lvm_vg_name << "/" << f_lvm->snapshotLvName(f_num)
		      << " has not been removed" << std::endl;
	}
	catch (const LvmImportTestsuiteException &e) {}
    }

    DeleteFilesystemSnapshotImportTypeAdopt::DeleteFilesystemSnapshotImportTypeAdopt()
	: InfoDirWithSnapshotDir(1), f_snapshot_lv_name(f_test_snapshot_01),
	rm(f_conf_lvm_vg_name, f_snapshot_lv_name), f_p_idata(new snapper::LvmImportMetadata(rm.f_raw_data, snapper::ImportPolicy::ADOPT, f_lvm)),
	f_sh(f_snapper, snapper::SnapshotType::SINGLE, f_num, (time_t) -1, f_p_idata)
    {
	lvcreate_thin_snapshot_wrapper(f_conf_lvm_vg_name, f_conf_lvm_origin_lv_name, f_snapshot_lv_name);
    }

    DeleteFilesystemSnapshotImportTypeAdopt::~DeleteFilesystemSnapshotImportTypeAdopt()
    {
	try
	{
	    lvremove_wrapper(f_conf_lvm_vg_name, f_snapshot_lv_name);
	    std::cerr << "DeleteFilesystemSnapshotImportTypeAdopt dtor: "
		      << f_conf_lvm_vg_name << "/" << f_lvm->snapshotLvName(f_num)
		      << " has not been removed" << std::endl;
	}
	catch (const LvmImportTestsuiteException &e) {}
    }

    DeleteFilesystemSnapshotImportTypeAcknowledge::DeleteFilesystemSnapshotImportTypeAcknowledge()
	: InfoDirWithSnapshotDir(1), f_snapshot_lv_name(f_test_snapshot_01),
	rm(f_conf_lvm_vg_name, f_snapshot_lv_name), f_p_idata(new snapper::LvmImportMetadata(rm.f_raw_data, snapper::ImportPolicy::ACKNOWLEDGE, f_lvm)),
	f_sh(f_snapper, snapper::SnapshotType::SINGLE, f_num, (time_t) -1, f_p_idata)
    {
	lvcreate_thin_snapshot_wrapper(f_conf_lvm_vg_name, f_conf_lvm_origin_lv_name, f_snapshot_lv_name);
    }

    DeleteFilesystemSnapshotImportTypeAcknowledge::~DeleteFilesystemSnapshotImportTypeAcknowledge()
    {
	try
	{
	    lvremove_wrapper(f_conf_lvm_vg_name, f_snapshot_lv_name);
	}
	catch (const LvmImportTestsuiteException &e)
	{
	    std::cerr << "DeleteFilesystemSnapshotImportTypeAcknowledge dtor: "
		      << f_conf_lvm_vg_name << "/" << f_lvm->snapshotLvName(f_num)
		      << " has been removed!" << std::endl;
	}
    }

    DeleteFileSystemSnapshotOrigin::DeleteFileSystemSnapshotOrigin()
	: LvmGeneralFixture(),
	f_sh(f_snapper, snapper::SnapshotType::SINGLE, 0, (time_t) -1, NULL)
    {
    }

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
	f_sh(f_snapper, snapper::SnapshotType::SINGLE, f_num, (time_t) - 1, NULL)
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
