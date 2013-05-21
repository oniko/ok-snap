#ifndef LVM_IMPORT_FIXTURES_H
#define LVM_IMPORT_FIXTURES_H

#include <string>
#include <map>

#include "snapper/Snapper.h"

#define private public
#include "snapper/Lvm.h"
#define private private

namespace lvmimporttest
{
    using std::map;
    using std::string;

    // NOTE: do not add any fork/execs with childs possibly returning non-zero codes
    // don't know how to disable --catch_system_errors in exec monitor for global
    // fixtures :(
    struct LvmGlobalConfig {
	LvmGlobalConfig();
	~LvmGlobalConfig();
    };

    struct GeneralFixture {
	GeneralFixture();
	~GeneralFixture();

	static const string f_snapshots_prefix;

	snapper::Snapper *sh;
	const snapper::Lvm *lvm;
    };

    struct ValidMetadata : public GeneralFixture {
	ValidMetadata();
	~ValidMetadata() {}

	map<string,string> raw_data;
    };

    struct MissingVgName : public GeneralFixture {
	MissingVgName();
	~MissingVgName() {}

	map<string,string> raw_data;
    };

    struct MissingLvName : public GeneralFixture {
	MissingLvName();
	~MissingLvName() {}

	map<string,string> raw_data;
    };
    
    struct CreateSnapshotEnvironment : public GeneralFixture {
	CreateSnapshotEnvironment();
	~CreateSnapshotEnvironment();

	string snapshot_dir;

	int dirfd;
	unsigned int num;
    };

    struct CreateSnapshotEnvironmentDirExists : public CreateSnapshotEnvironment {
	CreateSnapshotEnvironmentDirExists();
	~CreateSnapshotEnvironmentDirExists() {}
    };

    struct CreateSnapshotEnvironmentFailure : public CreateSnapshotEnvironment {
	CreateSnapshotEnvironmentFailure();
	~CreateSnapshotEnvironmentFailure() {}
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

	string dev_path;
	string mountpoint;
    };

    struct MountSnapshotByDeviceAlreadyMounted : public MountSnapshotByDeviceValid
    {
	MountSnapshotByDeviceAlreadyMounted();
    };

    struct MountSnapshotByDeviceInvalidDevice : public CreateSnapshotEnvironmentDirExists
    {
	MountSnapshotByDeviceInvalidDevice();
	~MountSnapshotByDeviceInvalidDevice() {}

	string missing_dev_path;
    };

    struct CheckImportedSnapshotValid : public GeneralFixture
    {
	CheckImportedSnapshotValid();
	~CheckImportedSnapshotValid();

	const string f_vg_name;
	const string f_lv_name;
	const string f_origin_name;
    };

    struct CheckImportedSnapshotWrongVg : public GeneralFixture
    {
	CheckImportedSnapshotWrongVg();
	~CheckImportedSnapshotWrongVg() {}

	const string f_vg_name;
	const string f_lv_name;
	const string f_origin_name;
    };

    struct CheckImportedSnapshotVolumeImport : public GeneralFixture
    {
	CheckImportedSnapshotVolumeImport();
	~CheckImportedSnapshotVolumeImport() {}

	const string f_vg_name;
	const string f_lv_name;
    };

    struct CheckImportedSnapshotFsUuidMismatch : public GeneralFixture
    {
	CheckImportedSnapshotFsUuidMismatch();
	~CheckImportedSnapshotFsUuidMismatch();

	const string f_vg_name;
	const string f_lv_name;
	const string f_origin_name;
    };

    struct CheckImportedSnapshotNonThinLv : public CheckImportedSnapshotValid
    {
    };

}
#endif // LVM_IMPORT_FIXTURES_H
