#ifndef LVM_TEST_H
#define LVM_TEST_H

#include "testsuite-import/general-test.h"
#include "testsuite-import/lvm-fixtures.h"

namespace testsuiteimport { namespace lvm
{
    class LvmTestClass
    {
    public:
	LvmTestClass() {}

	void tc_create_snapshot_environment();
	void tc_create_snapshot_environment_dir_exists();
	void tc_create_snapshot_environment_failure();
	void tc_create_snapshot_fail_on_environment();
	//void tc_remove_snapshot_environment(); // TODO: Nothing to test right now
	void tc_clone_snapshot();
	void tc_clone_snapshot_missing_origin();
	void tc_mount_snapshot_by_device();
	void tc_mount_snapshot_by_device_already_mounted();
	void tc_mount_snapshot_by_device_missing_device();
	void tc_check_imported_snapshot();
	void tc_check_imported_snapshot_wrong_vg();
	void tc_check_imported_snapshot_volume_import();
	void tc_check_imported_snapshot_fs_uuid_mismatch();
	void tc_check_imported_snapshot_non_thin_lv();
	void tc_check_delete_snapshot_by_vg_lv();
	void tc_check_delete_snapshot_by_vg_lv_missing();
    };

    struct FCreateSnapshotEnvironment : public GeneralFixture, CreateSnapshotEnvironment
    {
	virtual void test_method();
    };

    struct FCreateSnapshotEnvironmentDirExists : public GeneralFixture, CreateSnapshotEnvironmentDirExists
    {
	virtual void test_method();
    };

    struct FCreateSnapshotEnvironmentFailure : public GeneralFixture, CreateSnapshotEnvironmentFailure
    {
	virtual void test_method();
    };

    struct FCreateSnapshotFailOnEnvironment : public GeneralFixture, CreateSnapshotEnvironmentFailure
    {
	virtual void test_method();
    };
    
    struct FCloneSnapshotValid : public GeneralFixture, CloneSnapshotValid
    {
	virtual void test_method();
    };
    
    struct FCloneSnapshotMissingOrigin : public GeneralFixture, CloneSnapshotMissingOrigin
    {
	virtual void test_method();
    };
    
    struct FMountSnapshotByDeviceValid : public GeneralFixture, MountSnapshotByDeviceValid
    {
	virtual void test_method();
    };

    struct FMountSnapshotByDeviceAlreadyMounted : public GeneralFixture, MountSnapshotByDeviceAlreadyMounted
    {
	virtual void test_method();
    };

    struct FMountSnapshotByDeviceInvalidDevice : public GeneralFixture, MountSnapshotByDeviceInvalidDevice
    {
	virtual void test_method();
    };

    struct FCheckImportedSnapshotValid : public GeneralFixture, CheckImportedSnapshotValid
    {
	virtual void test_method();
    };

    struct FCheckImportedSnapshotWrongVg : public GeneralFixture, CheckImportedSnapshotWrongVg
    {
	virtual void test_method();
    };

    struct FCheckImportedSnapshotVolumeImport : public GeneralFixture, CheckImportedSnapshotVolumeImport
    {
	virtual void test_method();
    };

    struct FCheckImportedSnapshotFsUuidMismatch : public GeneralFixture, CheckImportedSnapshotFsUuidMismatch
    {
	virtual void test_method();
    };

    struct FCheckImportedSnapshotNonThinLv : public GeneralFixture, CheckImportedSnapshotNonThinLv
    {
	virtual void test_method();
    };

    struct FDeleteSnapshotByVgLv : public GeneralFixture, DeleteSnapshotByVgLv
    {
	virtual void test_method();
    };

    struct FDeleteSnapshotByVgLvMissing : public GeneralFixture, DeleteSnapshotByVgLvMissing
    {
	virtual void test_method();
    };

}}
#endif
