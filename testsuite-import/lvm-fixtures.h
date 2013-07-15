#ifndef LVM_IMPORT_FIXTURES_H
#define LVM_IMPORT_FIXTURES_H

#include <string>
#include <map>

#include "testsuite-import/general-fixtures.h"

namespace testsuiteimport { namespace lvm
{
    using std::map;
    using std::string;

    // TODO: move me somewhere else lvm-fixtures.h?
    struct LvmSubvolumeWrapper
    {
	LvmSubvolumeWrapper(const string& vg_name, const string& lv_orig_name, const string& lv_name, bool ro = true);
	LvmSubvolumeWrapper(const string& vg_name, const string& lv_name);
	~LvmSubvolumeWrapper();

	string subvolume() const { return vg_name + "/" + lv_name; }

	const string vg_name;
	const string lv_name;
	const string lv_orig_name;
    };


    struct InfoDirectory : public LvmGeneralFixture {
	InfoDirectory();
	InfoDirectory(unsigned int num);
	~InfoDirectory();

	string f_info_dir;

	unsigned int f_num;

	unsigned int get_dirfd() const { return f_dirfd; }
    protected:
	int f_dirfd;
    private:
	void infodir_init();
    };


    struct InfoDirWithSnapshotDir : public InfoDirectory {
	InfoDirWithSnapshotDir();
	InfoDirWithSnapshotDir(unsigned int num);
    private:
	void init();
    };


    struct InfoDirWithInvalidSnapshotDir : public InfoDirectory {
	InfoDirWithInvalidSnapshotDir();
	InfoDirWithInvalidSnapshotDir(unsigned int num);
    private:
	void init();
    };


    struct CreateSnapshotEnvironment
    {
	const InfoDirectory f_valid_info_dir;
	const InfoDirWithSnapshotDir f_info_snapshot_dir_exists;
	const InfoDirWithInvalidSnapshotDir f_invalid_snapshot_dir;
    };


    struct LvmCloneSnapshot : public LvmGeneralFixture
    {
	LvmCloneSnapshot();
	~LvmCloneSnapshot();

	const LvmSubvolumeWrapper f_valid_subvolume;

	const string f_missing_lv_vg;
	const string f_missing_lv_lv;

	const InfoDirectory f_info_dir_1;
	const InfoDirectory f_info_dir_2;
    };


    struct MountSnapshotByDeviceValid : public InfoDirWithSnapshotDir
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

    struct MountSnapshotByDeviceInvalidDevice : public InfoDirWithSnapshotDir
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
