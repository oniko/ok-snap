#ifndef SNAPSHOT_TEST_H
#define SNAPSHOT_TEST_H

#include "testsuite-import/general-test.h"
#include "testsuite-import/snapshot-fixtures.h"

namespace testsuiteimport { namespace lvm
{
    class SnapshotTestClass
    {
    public:
	void tc_snapshot_simple_ctor();
	void tc_snapshot_import_ctor();
	void tc_delete_filesystem_snapshot_import_type_none();
	void tc_delete_filesystem_snapshot_import_type_clone();
	void tc_delete_filesystem_snapshot_import_type_adopt();
	void tc_delete_filesystem_snapshot_import_type_ack();
	void tc_delete_filesystem_snapshot_origin();
	void tc_mount_filesystem_snapshot_import_none_non_user_request();
	void tc_mount_filesystem_snapshot_import_none_user_request();
	void tc_mount_filesystem_snapshot_import_clone_non_user_request();
	void tc_mount_filesystem_snapshot_import_clone_user_request();
	void tc_mount_filesystem_snapshot_import_adopt_non_user_request();
	void tc_mount_filesystem_snapshot_import_adopt_user_request();
	void tc_mount_filesystem_snapshot_import_ack_non_user_request();
	void tc_mount_filesystem_snapshot_import_ack_user_request();
	void tc_umount_filesystem_snapshot_import_none_non_user_request();
	void tc_umount_filesystem_snapshot_import_none_user_request();
	void tc_umount_filesystem_snapshot_import_clone_non_user_request();
	void tc_umount_filesystem_snapshot_import_clone_user_request();
	void tc_umount_filesystem_snapshot_import_adopt_non_user_request();
	void tc_umount_filesystem_snapshot_import_adopt_user_request();
	void tc_umount_filesystem_snapshot_import_ack_non_user_request();
	void tc_umount_filesystem_snapshot_import_ack_user_request();
	void tc_umount_filesystem_snapshot_invalid();
	void tc_handle_umount_filesystem_snapshot_non_user_request();
	void tc_handle_umount_filesystem_snapshot_user_request();
    };

    struct FSimpleConstructorValid : public GeneralFixture, SimpleConstructorValid
    {
	virtual void test_method();
    };

    struct FImportConstructorValid : public GeneralFixture, ImportConstructorValid
    {
	virtual void test_method();
    };

    struct FDeleteFilesystemSnapshotImportTypeNone : public GeneralFixture, DeleteFilesystemSnapshotImportTypeNone
    {
	virtual void test_method();
    };

    struct FDeleteFilesystemSnapshotImportTypeClone : public GeneralFixture, DeleteFilesystemSnapshotImportTypeClone
    {
	virtual void test_method();
    };

    struct FDeleteFilesystemSnapshotImportTypeAdopt : public GeneralFixture, DeleteFilesystemSnapshotImportTypeAdopt
    {
	virtual void test_method();
    };

    struct FDeleteFilesystemSnapshotImportTypeAcknowledge : public GeneralFixture, DeleteFilesystemSnapshotImportTypeAcknowledge
    {
	virtual void test_method();
    };

    struct FDeleteFileSystemSnapshotOrigin : public GeneralFixture, DeleteFileSystemSnapshotOrigin
    {
	virtual void test_method();
    };

    struct FMountFileSystemSnapshotImportNone : public GeneralFixture, MountFileSystemSnapshotImportNone
    {
	virtual void test_method();
    };

    struct FMountFileSystemSnapshotImportNoneUserRequest : public GeneralFixture, MountFileSystemSnapshotImportNone
    {
	virtual void test_method();
    };

    struct FMountFileSystemSnapshotImportClone : public GeneralFixture, MountFileSystemSnapshotImportClone
    {
	virtual void test_method();
    };

    struct FMountFileSystemSnapshotImportCloneUserRequest : public GeneralFixture, MountFileSystemSnapshotImportClone
    {
	virtual void test_method();
    };

    struct FMountFileSystemSnapshotImportAdopt : public GeneralFixture, MountFileSystemSnapshotImportAdopt
    {
	virtual void test_method();
    };

    struct FMountFileSystemSnapshotImportAdoptUserRequest : public GeneralFixture, MountFileSystemSnapshotImportAdopt
    {
	virtual void test_method();
    };

    struct FMountFileSystemSnapshotImportAck : public GeneralFixture, MountFileSystemSnapshotImportAck
    {
	virtual void test_method();
    };

    struct FMountFileSystemSnapshotImportAckUserRequest : public GeneralFixture, MountFileSystemSnapshotImportAck
    {
	virtual void test_method();
    };

    struct FUmountFilesystemSnapshotImportNone : public GeneralFixture, UmountFilesystemSnapshotImportNone
    {
	virtual void test_method();
    };

    struct FUmountFilesystemSnapshotImportNoneUserRequest : public GeneralFixture, UmountFilesystemSnapshotImportNone
    {
	virtual void test_method();
    };

    struct FUmountFilesystemSnapshotImportClone : public GeneralFixture, UmountFilesystemSnapshotImportClone
    {
	virtual void test_method();
    };

    struct FUmountFilesystemSnapshotImportCloneUserRequest : public GeneralFixture, UmountFilesystemSnapshotImportClone
    {
	virtual void test_method();
    };

    struct FUmountFilesystemSnapshotImportAdopt : public GeneralFixture, UmountFilesystemSnapshotImportAdopt
    {
	virtual void test_method();
    };

    struct FUmountFilesystemSnapshotImportAdoptUserRequest : public GeneralFixture, UmountFilesystemSnapshotImportAdopt
    {
	virtual void test_method();
    };

    struct FUmountFilesystemSnapshotImportAck : public GeneralFixture, UmountFilesystemSnapshotImportAck
    {
	virtual void test_method();
    };

    struct FUmountFilesystemSnapshotImportAckUserRequest : public GeneralFixture, UmountFilesystemSnapshotImportAck
    {
	virtual void test_method();
    };

    struct FUmountFilesystemInvalid : public GeneralFixture, UmountFilesystemInvalid
    {
	virtual void test_method();
    };

    struct FHandleUmountFilesystemSnapshot : public GeneralFixture, HandleUmountFilesystemSnapshot
    {
	virtual void test_method();
    };

    struct FHandleUmountFilesystemSnapshotUserRequest : public GeneralFixture, HandleUmountFilesystemSnapshot
    {
	virtual void test_method();
    };

}}
#endif // SNAPSHOT_TEST_H
