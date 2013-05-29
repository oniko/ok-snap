#ifndef SNAPSHOT_FIXTURES_H
#define SNAPSHOT_FIXTURES_H

#include <sys/stat.h>

// ouch !!!
#define private public
    #include "snapper/Snapshot.h"
#define private private

#include "snapper/Snapper.h"
#include "snapper/LvmImportMetadata.h"

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
	~ImportConstructorValid() {}

	const snapper::ImportPolicy f_import_policy;
	const snapper::ImportMetadata* f_p_idata;
    };

    struct CopyConstructorNoImport : public SimpleConstructorValid
    {
	CopyConstructorNoImport();
	~CopyConstructorNoImport() {}

	const uid_t f_uid;
	const unsigned int f_pre_num;
	const string f_description;
	const string f_cleanup;
	const map<string,string> f_userdata;
	const bool f_info_modified;
	const bool f_mount_checked;
	const bool f_mount_user_request;
	const unsigned int f_mount_use_count;

	snapper::Snapshot f_origin;
    };

    struct CopyConstructorImport : public SimpleConstructorValid
    {
	CopyConstructorImport();
	~CopyConstructorImport();

	const uid_t f_uid;
	const unsigned int f_pre_num;
	const string f_description;
	const string f_cleanup;
	const map<string,string> f_userdata;
	const bool f_info_modified;
	const bool f_mount_checked;
	const bool f_mount_user_request;
	const unsigned int f_mount_use_count;
	const snapper::ImportPolicy f_import_policy;

	map<string,string> raw_data;

	const snapper::ImportMetadata* f_p_idata;

	snapper::Snapshot* f_p_origin;
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

	const CreateRawLvmImportMetata rm;

	const snapper::LvmImportMetadata f_idata;
	const snapper::Snapshot f_sh;
	const string f_snapshot_lv_name;
    };

    struct DeleteFilesystemSnapshotImportTypeAdopt : public CreateSnapshotEnvironmentDirExists
    {
	DeleteFilesystemSnapshotImportTypeAdopt();
	~DeleteFilesystemSnapshotImportTypeAdopt();

	const CreateRawLvmImportMetata rm;

	const snapper::LvmImportMetadata f_idata;
	const snapper::Snapshot f_sh;
	const string f_snapshot_lv_name;
    };

    struct DeleteFilesystemSnapshotImportTypeAcknowledge : public CreateSnapshotEnvironmentDirExists
    {
	DeleteFilesystemSnapshotImportTypeAcknowledge();
	~DeleteFilesystemSnapshotImportTypeAcknowledge();

	const CreateRawLvmImportMetata rm;

	const snapper::LvmImportMetadata f_idata;
	const snapper::Snapshot f_sh;
	const string f_snapshot_lv_name;
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
	~MountFileSystemSnapshotImportNone() {}

	const snapper::Snapshot f_sh;
    };

    struct MountFileSystemSnapshotImportClone : public MountFileSystemSnapshotSimpleBase
    {
	MountFileSystemSnapshotImportClone();
	~MountFileSystemSnapshotImportClone();

	const string f_clone_origin_name;
	const snapper::LvmImportMetadata* f_p_lvm_idata;
	const snapper::Snapshot f_sh;
    };

    struct MountFileSystemSnapshotImportBase : public CreateSnapshotEnvironmentDirExists
    {
	MountFileSystemSnapshotImportBase();
	virtual ~MountFileSystemSnapshotImportBase();

	const string f_snapshot_lv_name;

	string f_mountpoint;
    };

    struct MountFileSystemSnapshotImportAdoptOrAck : public MountFileSystemSnapshotImportBase
    {
	MountFileSystemSnapshotImportAdoptOrAck();
	~MountFileSystemSnapshotImportAdoptOrAck() {}

	const snapper::LvmImportMetadata* f_p_lvm_idata;
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

    struct UmountFilesystemSnapshotImportAdoptOrAck : public MountFileSystemSnapshotImportAdoptOrAck, UmountFilesystemSnapshotBase
    {
	UmountFilesystemSnapshotImportAdoptOrAck();
    };

}}
#endif //SNAPSHOT_FIXTURES_H
