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
	/*void tc_create_snapshot_environment_failure() const;
	void tc_create_snapshot_fail_on_environment() const;
	void tc_remove_snapshot_environment() const;
	void tc_clone_snapshot() const;
	void tc_clone_snapshot_missing_origin() const;
	void tc_mount_snapshot_by_device() const;
	void tc_mount_snapshot_by_device_already_mounted() const;
	void tc_mount_snapshot_by_device_missing_device() const;
	void tc_check_imported_snapshot() const;
	void tc_check_imported_snapshot_wrong_vg() const;
	void tc_check_imported_snapshot_volume_import() const;
	void tc_check_imported_snapshot_fs_uuid_mismatch() const;
	void tc_check_imported_snapshot_non_thin_lv() const;
	void tc_check_delete_snapshot_by_vg_lv() const;
	void tc_check_delete_snapshot_by_vg_lv_missing() const;*/
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
    
    struct FMountSnapshotByDeviceAlreadyMounted : public GeneralFixture, MountSnapshotByDeviceValid
    {
	FMountSnapshotByDeviceAlreadyMounted();

	virtual void test_method();
    };

    struct FMountSnapshotByDeviceInvalidDevice : public GeneralFixture, CreateSnapshotEnvironmentDirExists
    {
	FMountSnapshotByDeviceInvalidDevice();
	~FMountSnapshotByDeviceInvalidDevice() {}

	string f_missing_dev_path;

	virtual void test_method();
    };

    struct FCheckImportedSnapshotValid : public GeneralFixture, LvmGeneralFixture
    {
	FCheckImportedSnapshotValid();
	~FCheckImportedSnapshotValid();

	const string f_vg_name;
	const string f_lv_name;
	const string f_origin_name;

	virtual void test_method();
    };

    struct FCheckImportedSnapshotWrongVg : public GeneralFixture, LvmGeneralFixture
    {
	FCheckImportedSnapshotWrongVg();
	~FCheckImportedSnapshotWrongVg() {}

	const string f_vg_name;
	const string f_lv_name;
	const string f_origin_name;

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
