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
	: f_dummy_snapper(static_cast<const snapper::Snapper *>(123456789)),
	f_type(snapper::SnapshotType::PRE), f_num(42), f_date(1234554321)
    {
    }

    ImportConstructorValid::ImportConstructorValid()
	: SimpleConstructorValid(), f_import_policy(snapper::ImportPolicy::ADOPT),
	f_p_idata(static_cast<const snapper::ImportMetadata *>(987654321))
    {
    }

    CopyConstructorNoImport::CopyConstructorNoImport()
	: SimpleConstructorValid(), f_uid(1024), f_pre_num(42),
	f_description("test_desc"), f_cleanup("test_cleanup"),
	f_info_modified(true), f_mount_checked(false), f_mount_user_request(true),
	f_mount_use_count(42), f_origin(f_dummy_snapper, f_type, f_num, f_date)
    {
	f_userdata.insert(make_pair("t_key", "t_value"));

	f_origin.setUid(f_uid);
	f_origin.setDescription(f_description);
	f_origin.setCleanup(f_cleanup);
	f_origin.setUserdata(f_userdata);

	// set remaining private attributes for testing purposes
	f_origin.pre_num = f_pre_num;
	f_origin.info_modified = f_info_modified;
	f_origin.mount_user_request = f_mount_user_request;
	f_origin.mount_use_count = f_mount_use_count;
    }

    CopyConstructorImport::CopyConstructorImport()
	: SimpleConstructorValid(), f_uid(1024), f_pre_num(42),
	f_description("test_desc"), f_cleanup("test_cleanup"), f_info_modified(true),
	f_mount_checked(false), f_mount_user_request(true), f_mount_use_count(42),
	f_import_policy(snapper::ImportPolicy::ADOPT)
    {
	f_userdata.insert(make_pair("t_key", "t_value"));

	raw_data.insert(make_pair("vg_name", "vg_test"));
	raw_data.insert(make_pair("lv_name", "lv_test_thin_1"));

	f_p_idata = new snapper::LvmImportMetadata(raw_data, static_cast<const snapper::Lvm *>(24680));
	// Snapshot takes over ImportMetadata ownership, call Snapshot dtor to free ImportMetadata resources
	f_p_origin = new snapper::Snapshot(f_dummy_snapper, f_type, f_num, f_date, f_import_policy, f_p_idata);

	f_p_origin->setUid(f_uid);
	f_p_origin->setDescription(f_description);
	f_p_origin->setCleanup(f_cleanup);
	f_p_origin->setUserdata(f_userdata);

	// set remaining private attributes for testing purposes
	f_p_origin->pre_num = f_pre_num;
	f_p_origin->info_modified = f_info_modified;
	f_p_origin->mount_user_request = f_mount_user_request;
	f_p_origin->mount_use_count = f_mount_use_count;
    }

    CopyConstructorImport::~CopyConstructorImport()
    {
	delete f_p_origin;
    }

    DeleteFilesystemSnapshotImportTypeNone::DeleteFilesystemSnapshotImportTypeNone()
	: CreateSnapshotEnvironmentDirExists(),
	f_sh(f_snapper, snapper::SnapshotType::SINGLE, f_num, (time_t) -1),
	f_snapshot_lv_name(f_lvm->snapshotLvName(f_num))
    {
	lvcreate_thin_snapshot_wrapper(f_conf_vg_name, f_conf_origin_name, f_snapshot_lv_name);
    }

    DeleteFilesystemSnapshotImportTypeNone::~DeleteFilesystemSnapshotImportTypeNone()
    {
	try
	{
	    lvremove_wrapper(f_conf_vg_name, f_lvm->snapshotLvName(f_num) );
	    std::cerr << "DeleteFilesystemSnapshotImportTypeNone dtor: "
		      << f_conf_vg_name << "/" << f_lvm->snapshotLvName(f_num)
		      << " has not been removed" << std::endl;
	}
	catch (const LvmImportTestsuiteException &e) {}
	catch (const SimpleSystemCmdException &e) {}
    }

    DeleteFilesystemSnapshotImportTypeClone::DeleteFilesystemSnapshotImportTypeClone()
	: CreateSnapshotEnvironmentDirExists(), f_snapshot_lv_name(f_lvm->snapshotLvName(f_num)),
	rm(f_conf_vg_name, f_snapshot_lv_name),
	f_sh(f_snapper, snapper::SnapshotType::SINGLE, f_num, (time_t) -1, snapper::ImportPolicy::CLONE, rm.f_raw_data)
    {
	lvcreate_thin_snapshot_wrapper(f_conf_vg_name, f_conf_origin_name, f_snapshot_lv_name);
    }

    DeleteFilesystemSnapshotImportTypeClone::~DeleteFilesystemSnapshotImportTypeClone()
    {
	try
	{
	    lvremove_wrapper(f_conf_vg_name, f_lvm->snapshotLvName(f_num) );
	    std::cerr << "DeleteFilesystemSnapshotImportTypeClone dtor: "
		      << f_conf_vg_name << "/" << f_lvm->snapshotLvName(f_num)
		      << " has not been removed" << std::endl;
	}
	catch (const LvmImportTestsuiteException &e) {}
	catch (const SimpleSystemCmdException &e) {}
    }

    DeleteFilesystemSnapshotImportTypeAdopt::DeleteFilesystemSnapshotImportTypeAdopt()
	: CreateSnapshotEnvironmentDirExists(), f_snapshot_lv_name(f_test_snapshot_01),
	rm(f_conf_vg_name, f_snapshot_lv_name),
	f_sh(f_snapper, snapper::SnapshotType::SINGLE, f_num, (time_t) -1, snapper::ImportPolicy::ADOPT, rm.f_raw_data)
    {
	lvcreate_thin_snapshot_wrapper(f_conf_vg_name, f_conf_origin_name, f_snapshot_lv_name);
    }

    DeleteFilesystemSnapshotImportTypeAdopt::~DeleteFilesystemSnapshotImportTypeAdopt()
    {
	try
	{
	    lvremove_wrapper(f_conf_vg_name, f_snapshot_lv_name);
	    std::cerr << "DeleteFilesystemSnapshotImportTypeAdopt dtor: "
		      << f_conf_vg_name << "/" << f_lvm->snapshotLvName(f_num)
		      << " has not been removed" << std::endl;
	}
	catch (const LvmImportTestsuiteException &e) {}
	catch (const SimpleSystemCmdException &e) {}
    }

    DeleteFilesystemSnapshotImportTypeAcknowledge::DeleteFilesystemSnapshotImportTypeAcknowledge()
	: CreateSnapshotEnvironmentDirExists(), f_snapshot_lv_name(f_test_snapshot_01),
	rm(f_conf_vg_name, f_snapshot_lv_name),
	f_sh(f_snapper, snapper::SnapshotType::SINGLE, f_num, (time_t) -1, snapper::ImportPolicy::ACKNOWLEDGE, rm.f_raw_data)
    {
	lvcreate_thin_snapshot_wrapper(f_conf_vg_name, f_conf_origin_name, f_snapshot_lv_name);
    }

    DeleteFilesystemSnapshotImportTypeAcknowledge::~DeleteFilesystemSnapshotImportTypeAcknowledge()
    {
	try
	{
	    lvremove_wrapper(f_conf_vg_name, f_snapshot_lv_name);
	}
	catch (const LvmImportTestsuiteException &e)
	{
	    std::cerr << "DeleteFilesystemSnapshotImportTypeAcknowledge dtor: "
		      << f_conf_vg_name << "/" << f_lvm->snapshotLvName(f_num)
		      << " has been removed!" << std::endl;
	}
	catch (const SimpleSystemCmdException &e) {}
    }

    DeleteFileSystemSnapshotOrigin::DeleteFileSystemSnapshotOrigin()
	: LvmGeneralFixture(), f_sh(f_snapper, snapper::SnapshotType::SINGLE, 0, (time_t) -1)
    {
    }

    MountFileSystemSnapshotSimpleBase::MountFileSystemSnapshotSimpleBase()
	: CreateSnapshotEnvironmentDirExists(),
	f_snapshot_lv_name(f_lvm->snapshotLvName(f_num))
    {
	lvcreate_thin_snapshot_wrapper(f_conf_vg_name, f_conf_origin_name, f_snapshot_lv_name);

	std::ostringstream oss;
	oss << f_snapshots_prefix << f_num << "/snapshot";

	f_mountpoint = oss.str();
    }

    MountFileSystemSnapshotSimpleBase::~MountFileSystemSnapshotSimpleBase()
    {
	if (umount2(f_mountpoint.c_str(), MNT_DETACH))
	    std::cerr << "umount2( \"" << f_mountpoint << "\", MNT_DETACH) failed!" << std::endl;

	try
	{
	    lvremove_wrapper(f_conf_vg_name, f_snapshot_lv_name );
	}
	catch (const LvmImportTestsuiteException &e)
	{
	    std::cerr << "lvremove_wrapper( " << f_conf_vg_name << ", " << f_snapshot_lv_name << " ) failed" << std::endl;
	}
    }

    MountFileSystemSnapshotImportNone::MountFileSystemSnapshotImportNone()
	: MountFileSystemSnapshotSimpleBase(),
	f_sh(f_snapper, snapper::SnapshotType::SINGLE, f_num, (time_t)-1)
    {
    }

    MountFileSystemSnapshotImportClone::MountFileSystemSnapshotImportClone()
	: MountFileSystemSnapshotSimpleBase(),
	f_clone_origin_name("lv_clone_origin_name"),
	f_p_lvm_idata(new snapper::LvmImportMetadata(f_conf_vg_name, f_clone_origin_name)),
	f_sh(f_snapper, snapper::SnapshotType::SINGLE, f_num, (time_t)-1, snapper::ImportPolicy::CLONE, f_p_lvm_idata)
    {
	lvcreate_thin_snapshot_wrapper(f_conf_vg_name, f_conf_origin_name, f_clone_origin_name);
    }

    MountFileSystemSnapshotImportClone::~MountFileSystemSnapshotImportClone()
    {
	try
	{
	    lvremove_wrapper(f_conf_vg_name, f_clone_origin_name);
	}
	catch (const LvmImportTestsuiteException &e)
	{
	    std::cerr << "lvremove_wrapper( " << f_conf_vg_name << ", " << f_clone_origin_name << " ) failed" << std::endl;
	}
    }

    MountFileSystemSnapshotImportBase::MountFileSystemSnapshotImportBase()
	: CreateSnapshotEnvironmentDirExists(),
	f_snapshot_lv_name("lv_test_import_snapshot")
    {
	lvcreate_thin_snapshot_wrapper(f_conf_vg_name, f_conf_origin_name, f_snapshot_lv_name);

	std::ostringstream oss;
	oss << f_snapshots_prefix << f_num << "/snapshot";

	f_mountpoint = oss.str();
    }

    MountFileSystemSnapshotImportBase::~MountFileSystemSnapshotImportBase()
    {
	if (umount2(f_mountpoint.c_str(), MNT_DETACH))
		std::cerr << "umount2( \"" << f_mountpoint << "\", MNT_DETACH) failed!" << std::endl;

	try
	{
	    lvremove_wrapper(f_conf_vg_name, f_snapshot_lv_name );
	}
	catch (const LvmImportTestsuiteException &e)
	{
	    std::cerr << "lvremove_wrapper( " << f_conf_vg_name << ", " << f_snapshot_lv_name << " ) failed" << std::endl;
	}
    }

    MountFileSystemSnapshotImportAdopt::MountFileSystemSnapshotImportAdopt()
	: MountFileSystemSnapshotImportBase(),
	f_p_lvm_idata(new snapper::LvmImportMetadata(f_conf_vg_name, f_snapshot_lv_name)),
	f_sh(f_snapper, snapper::SnapshotType::SINGLE, f_num, (time_t) -1, snapper::ImportPolicy::ADOPT, f_p_lvm_idata)
    {
    }

    MountFileSystemSnapshotImportAck::MountFileSystemSnapshotImportAck()
	: MountFileSystemSnapshotImportBase(),
	f_p_lvm_idata(new snapper::LvmImportMetadata(f_conf_vg_name, f_snapshot_lv_name)),
	f_sh(f_snapper, snapper::SnapshotType::SINGLE, f_num, (time_t) -1, snapper::ImportPolicy::ACKNOWLEDGE, f_p_lvm_idata)
    {
    }

    UmountFilesystemSnapshotBase::UmountFilesystemSnapshotBase(const string& dev, const string& mount_point, const string& mount_type)
	: f_dev_path(dev)
    {
	std::cout << "UmountFilesystemSnapshotBase ctor" << std::endl;

	int ret = mount(f_dev_path.c_str(), mount_point.c_str(),
			mount_type.c_str(),
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
	UmountFilesystemSnapshotBase("/dev/mapper/" + f_conf_vg_name + "/" + f_snapshot_lv_name, f_mountpoint, f_lvm->mount_type)
    {
	f_sh.mount_use_count = 1;
    }

    UmountFilesystemSnapshotImportClone::UmountFilesystemSnapshotImportClone()
	: MountFileSystemSnapshotImportClone(),
	UmountFilesystemSnapshotBase("/dev/mapper/" + f_conf_vg_name + "/" + f_snapshot_lv_name, f_mountpoint, f_lvm->mount_type),
	f_dev_origin_path("/dev/mapper/" + f_conf_vg_name + "/" + f_clone_origin_name),
	f_origin_mount_point(f_snapshot_dir + "/tmp_mnt_point")
    {
	std::cout << "UmountFilesystemSnapshotImportClone ctor" << std::endl;

	if (mkdirat(f_dirfd, "tmp_mnt_point", 0755))
	    BOOST_FAIL( "Can't tmp_mnt_point directory in test environment" );

	int ret = mount(f_dev_origin_path.c_str(), f_origin_mount_point.c_str(),
			f_lvm->mount_type.c_str(),
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
	UmountFilesystemSnapshotBase("/dev/mapper/" + f_conf_vg_name + "/" + f_snapshot_lv_name, f_mountpoint, f_lvm->mount_type)
    {
	f_sh.mount_use_count = 1;
    }

    UmountFilesystemSnapshotImportAck::UmountFilesystemSnapshotImportAck()
	: MountFileSystemSnapshotImportAck(),
	UmountFilesystemSnapshotBase("/dev/mapper/" + f_conf_vg_name + "/" + f_snapshot_lv_name, f_mountpoint, f_lvm->mount_type)
    {
	f_sh.mount_use_count = 1;
    }
}}
