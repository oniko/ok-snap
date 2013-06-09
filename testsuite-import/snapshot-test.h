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
	void tc_snapshot_copy_ctor_no_import();
	void tc_snapshot_copy_ctor_import();
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
	void tc_umount_filesystem_snapshot_import_none_non_user_request();
	void tc_umount_filesystem_snapshot_import_clone_non_user_request();
	void tc_umount_filesystem_snapshot_import_clone_user_request();
	void tc_umount_filesystem_snapshot_import_adopt_non_user_request();
	void tc_umount_filesystem_snapshot_import_adopt_user_request();
	void tc_umount_filesystem_snapshot_import_ack_non_user_request();
	void tc_umount_filesystem_snapshot_import_ack_user_request();
	void tc_umount_filesystem_snapshot_invalid();
	void tc_handle_umount_filesystem_snapshot();
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

    struct FCopyConstructorNoImport : public GeneralFixture, CopyConstructorNoImport
    {
	virtual void test_method();
    };

    struct FCopyConstructorImport : public GeneralFixture, CopyConstructorImport
    {
	virtual void test_method();
    };

    struct FDeleteFilesystemSnapshotImportTypeNone : public GeneralFixture, DeleteFilesystemSnapshotImportTypeNone
    {
	virtual void test_method();
    };
}}
#endif // SNAPSHOT_TEST_H
