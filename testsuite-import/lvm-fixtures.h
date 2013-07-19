#ifndef LVM_IMPORT_FIXTURES_H
#define LVM_IMPORT_FIXTURES_H

#include <string>
#include <map>

#include <boost/noncopyable.hpp>

#include "testsuite-import/general-fixtures.h"

namespace testsuiteimport { namespace lvm
{
    using std::map;
    using std::string;

    // TODO: move me somewhere else lvm-fixtures.h?
    // NOTE: do not use vg/lv names with dash in names yet!
    struct LvmSubvolumeWrapper : public SubvolumeWrapper
    {
	LvmSubvolumeWrapper(const string& vg_name, const string& lv_orig_name, const string& lv_name, bool ro = true);
	LvmSubvolumeWrapper(const string& vg_name, const string& lv_name);
	~LvmSubvolumeWrapper();

	string subvolume() const { return vg_name + "/" + lv_name; }
	string devicepath() const { return "/dev/mapper/" + vg_name + "-" + lv_name; }

	const string vg_name;
	const string lv_name;
	const string lv_orig_name;

	virtual bool exists() const;
	virtual bool is_mounted() const;

	virtual string fstype() const { return LvmSubvolumeWrapper::lvm_fs_type; }
	virtual string infos_dir() const { return LvmGeneralFixture::f_conf_lvm_snapshots_prefix; }
    private:
	static const string lvm_fs_type;
    };


    struct CreateSnapshotEnvironment : public LvmGeneralFixture
    {
	const InfoDirectory f_valid_info_dir;
	const InfoDirWithSnapshotDir f_info_snapshot_dir_exists;
	const InfoDirWithInvalidSnapshotDir f_invalid_snapshot_dir;
    };


    struct CreateSnapshotFailOnEnvironment : public LvmGeneralFixture
    {
	~CreateSnapshotFailOnEnvironment();

	const InfoDirWithInvalidSnapshotDir f_invalid_snap_dir;
    };


    struct RemoveSnapshotEnvironment : public LvmGeneralFixture
    {
	const InfoDirWithSnapshotDir f_snapshot_dir;
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


    struct LvmMountSnapshotBySubvolume : LvmGeneralFixture
    {
	LvmMountSnapshotBySubvolume();

	const InfoDirWithSnapshotDir f_snapshot_dir_1;
	const InfoDirWithSnapshotDir f_snapshot_dir_2;

	const LvmSubvolumeWrapper f_mount_volume;

	const string f_missing_dev_path;
    };


    struct LvmCheckImportedSnapshot : public LvmGeneralFixture
    {
	LvmCheckImportedSnapshot();

	// missing vg test
	const string f_missing_vg_vg;
	const string f_missing_vg_lv;

	// missing lv test
	const string f_missing_lv_vg;
	const string f_missing_lv_lv;

	// not a thin volume
	const LvmSubvolumeWrapper f_nonthin_subvolume;

	// about to be removed. condition is useless
	// foreign vg test case
	const LvmSubvolumeWrapper f_foreign_vg_subvolume;

	// current subvolume test case (aka exercise importing snapshot 0)
	const string f_current_subvolume_vg_name;
	const string f_current_subvolume_lv_name;

	// test for rw enabled snapshots
	const LvmSubvolumeWrapper f_rw_wrong_fs_uuid_subvolume;

	const LvmSubvolumeWrapper f_rw_subvolume;

	// test for ro snapshots
	const LvmSubvolumeWrapper f_ro_wrong_fs_uuid_subvolume;

	const LvmSubvolumeWrapper f_ro_subvolume;
    };


    struct DeleteSnapshotByVgLv : public LvmGeneralFixture
    {
	DeleteSnapshotByVgLv();
	~DeleteSnapshotByVgLv();

	const LvmSubvolumeWrapper f_subvolume;

	const string f_missing_vg_vg;
	const string f_missing_vg_lv;

	const string f_missing_lv_vg;
	const string f_missing_lv_lv;
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
