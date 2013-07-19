#ifndef SNAPSHOT_FIXTURES_H
#define SNAPSHOT_FIXTURES_H

#include <sys/stat.h>

#include "snapper/Snapshot.h"
#include "snapper/Snapper.h"
#include "snapper/LvmImportMetadata.h"

#include "testsuite-import/general-test.h"
#include "testsuite-import/general-fixtures.h"
#include "testsuite-import/lvm-fixtures.h"

// TODO: REMOVE THIS
#include "testsuite-import/snapshot-test.h"

namespace testsuiteimport { namespace lvm
{
    using std::map;
    using std::string;


    struct LvmSnapshotFixtures : public GeneralSnapshotFixtures
    {
	LvmSnapshotFixtures(const snapper::Snapper* snapper) : GeneralSnapshotFixtures(snapper) {}

	virtual GeneralSnapshotCtorFixture ctor_fixture() const;
	virtual GeneralGetImportPolicyFixture get_import_policy_fixture() const;
	virtual GeneralGetSnapshotDirFixture get_snapshot_dir_fixture() const;
	virtual GeneralMountFilesystemFixture mount_filesystem_fixture() const;
    };


//     struct DeleteFilesystemSnapshotImportTypeNone : public InfoDirWithSnapshotDir
//     {
// 	DeleteFilesystemSnapshotImportTypeNone();
// 	~DeleteFilesystemSnapshotImportTypeNone();
// 
// 	const snapper::Snapshot f_sh;
// 	const string f_snapshot_lv_name;
//     };
// 
//     struct DeleteFilesystemSnapshotImportTypeClone : public InfoDirWithSnapshotDir
//     {
// 	DeleteFilesystemSnapshotImportTypeClone();
// 	~DeleteFilesystemSnapshotImportTypeClone();
// 
// 	const string f_snapshot_lv_name;
// 	const CreateRawLvmImportMetata rm;
// 
// 	const snapper::ImportMetadata* f_p_idata;
// 	const snapper::Snapshot f_sh;
//     };
// 
//     struct DeleteFilesystemSnapshotImportTypeAdopt : public InfoDirWithSnapshotDir
//     {
// 	DeleteFilesystemSnapshotImportTypeAdopt();
// 	~DeleteFilesystemSnapshotImportTypeAdopt();
// 
// 	const string f_snapshot_lv_name;
// 	const CreateRawLvmImportMetata rm;
// 
// 	const snapper::ImportMetadata* f_p_idata;
// 	const snapper::Snapshot f_sh;
//     };
// 
//     struct DeleteFilesystemSnapshotImportTypeAcknowledge : public InfoDirWithSnapshotDir
//     {
// 	DeleteFilesystemSnapshotImportTypeAcknowledge();
// 	~DeleteFilesystemSnapshotImportTypeAcknowledge();
// 
// 	const string f_snapshot_lv_name;
// 	const CreateRawLvmImportMetata rm;
// 
// 	const snapper::ImportMetadata* f_p_idata;
// 	const snapper::Snapshot f_sh;
//     };
// 
//     struct DeleteFileSystemSnapshotOrigin : public LvmGeneralFixture
//     {
// 	DeleteFileSystemSnapshotOrigin();
// 	~DeleteFileSystemSnapshotOrigin() {}
// 
// 	const snapper::Snapshot f_sh;
//     };

    struct MountFileSystemSnapshotSimpleBase : public InfoDirWithSnapshotDir
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

    struct MountFileSystemSnapshotImportBase : public InfoDirWithSnapshotDir
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

}}
#endif //SNAPSHOT_FIXTURES_H
