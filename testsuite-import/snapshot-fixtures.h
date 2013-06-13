#ifndef SNAPSHOT_FIXTURES_H
#define SNAPSHOT_FIXTURES_H

#include <sys/stat.h>

// ouch !!!
#define private public
    #include "snapper/Snapshot.h"
    #include "snapper/Snapper.h"
    #include "snapper/LvmImportMetadata.h"
#undef private

#include "testsuite-import/general-fixtures.h"
#include "testsuite-import/lvm-fixtures.h"

namespace testsuiteimport { namespace lvm
{
    using std::map;
    using std::string;

    struct CreateRawLvmImportMetata
    {
	CreateRawLvmImportMetata(const string&, const string&);
	~CreateRawLvmImportMetata() {}

	map<string, string> f_raw_data;
    };

    struct SimpleConstructorValid
    {
	SimpleConstructorValid();
	~SimpleConstructorValid() {}

	const snapper::Snapper* f_dummy_snapper;
	const snapper::SnapshotType f_type;
	const unsigned int f_num;
	const time_t f_date;
    };

    struct ImportConstructorValid : public SimpleConstructorValid
    {
	ImportConstructorValid();

	const CreateRawLvmImportMetata rm;

	const snapper::ImportPolicy f_import_policy;
	const snapper::Lvm* f_dummy_lvm;
	const snapper::ImportMetadata* f_dummy_idata;
    };


    struct DeleteFilesystemSnapshotImportTypeNone : public CreateSnapshotEnvironmentDirExists
    {
	DeleteFilesystemSnapshotImportTypeNone();
	~DeleteFilesystemSnapshotImportTypeNone();

	const snapper::Snapshot f_sh;
	const string f_snapshot_lv_name;
    };

    struct DeleteFilesystemSnapshotImportTypeClone : public CreateSnapshotEnvironmentDirExists
    {
	DeleteFilesystemSnapshotImportTypeClone();
	~DeleteFilesystemSnapshotImportTypeClone();

	const string f_snapshot_lv_name;
	const CreateRawLvmImportMetata rm;

	const snapper::ImportMetadata* f_p_idata;
	const snapper::Snapshot f_sh;
    };

    struct DeleteFilesystemSnapshotImportTypeAdopt : public CreateSnapshotEnvironmentDirExists
    {
	DeleteFilesystemSnapshotImportTypeAdopt();
	~DeleteFilesystemSnapshotImportTypeAdopt();

	const string f_snapshot_lv_name;
	const CreateRawLvmImportMetata rm;

	const snapper::ImportMetadata* f_p_idata;
	const snapper::Snapshot f_sh;
    };

    struct DeleteFilesystemSnapshotImportTypeAcknowledge : public CreateSnapshotEnvironmentDirExists
    {
	DeleteFilesystemSnapshotImportTypeAcknowledge();
	~DeleteFilesystemSnapshotImportTypeAcknowledge();

	const string f_snapshot_lv_name;
	const CreateRawLvmImportMetata rm;

	const snapper::ImportMetadata* f_p_idata;
	const snapper::Snapshot f_sh;
    };

    struct DeleteFileSystemSnapshotOrigin : public LvmGeneralFixture
    {
	DeleteFileSystemSnapshotOrigin();
	~DeleteFileSystemSnapshotOrigin() {}

	const snapper::Snapshot f_sh;
    };

    struct MountFileSystemSnapshotSimpleBase : public CreateSnapshotEnvironmentDirExists
    {
	MountFileSystemSnapshotSimpleBase();
	~MountFileSystemSnapshotSimpleBase();

	const string f_snapshot_lv_name;

	string f_mountpoint;
    };

    struct MountFileSystemSnapshotImportNone : public MountFileSystemSnapshotSimpleBase
    {
	MountFileSystemSnapshotImportNone();

	const snapper::Snapshot f_sh;
    };

    struct MountFileSystemSnapshotImportClone : public MountFileSystemSnapshotSimpleBase
    {
	MountFileSystemSnapshotImportClone();
	~MountFileSystemSnapshotImportClone();

	const string f_clone_origin_name;
	const CreateRawLvmImportMetata rm;

	const snapper::ImportMetadata* f_p_idata;
	const snapper::Snapshot f_sh;
    };

    struct MountFileSystemSnapshotImportBase : public CreateSnapshotEnvironmentDirExists
    {
	MountFileSystemSnapshotImportBase();
	virtual ~MountFileSystemSnapshotImportBase();

	const string f_snapshot_lv_name;

	string f_mountpoint;
    };

    struct MountFileSystemSnapshotImportAdopt : public MountFileSystemSnapshotImportBase
    {
	MountFileSystemSnapshotImportAdopt();

	const CreateRawLvmImportMetata rm;

	const snapper::ImportMetadata* f_p_idata;
	const snapper::Snapshot f_sh;
    };

    struct MountFileSystemSnapshotImportAck : public MountFileSystemSnapshotImportBase
    {
	MountFileSystemSnapshotImportAck();

	const CreateRawLvmImportMetata rm;

	const snapper::ImportMetadata* f_p_idata;
	const snapper::Snapshot f_sh;
    };

    struct UmountFilesystemSnapshotBase
    {
	UmountFilesystemSnapshotBase(const string& dev, const string& mount_point, const string& mount_type);

	const string f_dev_path;
    };

    struct UmountFilesystemSnapshotImportNone : public MountFileSystemSnapshotImportNone, UmountFilesystemSnapshotBase
    {
	UmountFilesystemSnapshotImportNone();
    };

    struct UmountFilesystemSnapshotImportClone : public MountFileSystemSnapshotImportClone, UmountFilesystemSnapshotBase
    {
	UmountFilesystemSnapshotImportClone();
	~UmountFilesystemSnapshotImportClone();

	const string f_dev_origin_path;
	const string f_origin_mount_point;
    };

    struct UmountFilesystemSnapshotImportAdopt : public MountFileSystemSnapshotImportAdopt, UmountFilesystemSnapshotBase
    {
	UmountFilesystemSnapshotImportAdopt();
    };

    struct UmountFilesystemSnapshotImportAck : public MountFileSystemSnapshotImportAck, UmountFilesystemSnapshotBase
    {
	UmountFilesystemSnapshotImportAck();
    };

    struct UmountFilesystemInvalid : public DeleteFileSystemSnapshotOrigin
    {
    };

    struct HandleUmountFilesystemSnapshot : public UmountFilesystemSnapshotImportNone
    {
    };

}}
#endif //SNAPSHOT_FIXTURES_H
