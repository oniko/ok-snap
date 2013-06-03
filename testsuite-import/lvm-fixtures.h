#ifndef LVM_IMPORT_FIXTURES_H
#define LVM_IMPORT_FIXTURES_H

#include <string>
#include <map>

#include "testsuite-import/general-fixtures.h"

namespace testsuiteimport { namespace lvm
{    
    using std::map;
    using std::string;
    
    struct CreateSnapshotEnvironment : public LvmGeneralFixture {
	CreateSnapshotEnvironment();
	~CreateSnapshotEnvironment();

	string f_snapshot_dir;

	int f_dirfd;
	unsigned int f_num;
    };

    struct CreateSnapshotEnvironmentDirExists : public CreateSnapshotEnvironment {
	CreateSnapshotEnvironmentDirExists();
    };

    struct CreateSnapshotEnvironmentFailure : public CreateSnapshotEnvironment {
	CreateSnapshotEnvironmentFailure();
    };

    struct CreateSnapshotFailOnEnvironment : public CreateSnapshotEnvironmentFailure
    {
    };

    struct CloneSnapshotValid : public CreateSnapshotEnvironment {
	CloneSnapshotValid();
	~CloneSnapshotValid();

	const string f_vg_name;
	const string f_lv_name;
	const string f_origin_name;
    };

    struct CloneSnapshotMissingOrigin : public CreateSnapshotEnvironment {
	CloneSnapshotMissingOrigin();
	~CloneSnapshotMissingOrigin() {}

	const string f_vg_name;
	const string f_lv_name;
    };

    struct MountSnapshotByDeviceValid : public CreateSnapshotEnvironmentDirExists
    {
	MountSnapshotByDeviceValid();
	~MountSnapshotByDeviceValid();

	const string f_vg_name;
	const string f_lv_name;
	const string f_origin_name;

	string f_dev_path;
	string f_mountpoint;
    };

    struct MountSnapshotByDeviceAlreadyMounted : public MountSnapshotByDeviceValid
    {
	MountSnapshotByDeviceAlreadyMounted();
    };

    struct MountSnapshotByDeviceInvalidDevice : public CreateSnapshotEnvironmentDirExists
    {
	MountSnapshotByDeviceInvalidDevice();
	~MountSnapshotByDeviceInvalidDevice() {}

	string f_missing_dev_path;
    };

    struct CheckImportedSnapshotValid : public LvmGeneralFixture
    {
	CheckImportedSnapshotValid();
	~CheckImportedSnapshotValid();

	const string f_vg_name;
	const string f_lv_name;
	const string f_origin_name;
    };

    struct CheckImportedSnapshotWrongVg : public LvmGeneralFixture
    {
	CheckImportedSnapshotWrongVg();
	~CheckImportedSnapshotWrongVg() {}

	const string f_vg_name;
	const string f_lv_name;
	const string f_origin_name;
    };

    struct CheckImportedSnapshotVolumeImport : public LvmGeneralFixture
    {
	CheckImportedSnapshotVolumeImport();
	~CheckImportedSnapshotVolumeImport() {}

	const string f_vg_name;
	const string f_lv_name;
    };

    struct CheckImportedSnapshotFsUuidMismatch : public LvmGeneralFixture
    {
	CheckImportedSnapshotFsUuidMismatch();
	~CheckImportedSnapshotFsUuidMismatch();

	const string f_vg_name;
	const string f_lv_name;
	const string f_origin_name;
    };

    struct CheckImportedSnapshotNonThinLv : public LvmGeneralFixture
    {
	CheckImportedSnapshotNonThinLv();
	~CheckImportedSnapshotNonThinLv();

	const string f_vg_name;
	const string f_lv_name;
    };

    struct DeleteSnapshotByVgLv : public LvmGeneralFixture
    {
	DeleteSnapshotByVgLv();
	~DeleteSnapshotByVgLv();

	const string f_vg_name;
	const string f_lv_name;
	const string f_origin_name;
    };

    struct DeleteSnapshotByVgLvMissing : public LvmGeneralFixture
    {
	DeleteSnapshotByVgLvMissing();
	~DeleteSnapshotByVgLvMissing() {}

	const string f_vg_name;
	const string f_lv_name;
    };
}}
#endif // LVM_IMPORT_FIXTURES_H
