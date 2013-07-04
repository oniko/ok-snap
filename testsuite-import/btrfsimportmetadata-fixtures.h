#ifndef BTRFSIMPORTMETADATA_FIXTURES_H
#define BTRFSIMPORTMETADATA_FIXTURES_H

#include <map>
#include <string>

#include "snapper/BtrfsImportMetadata.h"

#include "testsuite-import/general-fixtures.h"

namespace testsuiteimport { namespace btrfs
{
    using std::string;
    using std::map;

    struct SubvolumeWrapper
    {
	SubvolumeWrapper(const string& root_path, const string& subdirs, bool ro = false);
	SubvolumeWrapper(const string& root_path, bool ro = false);
	~SubvolumeWrapper();

	const string root_path;
	const string subvolume;

	string fullpath() const { return root_path + "/" + subvolume; }
    private:
	string dirs;
	void init(bool rdonly) const;
	static const string f_subv_prefix;
	static string init_subvolume(const string& root_volume);
    };

    struct ValidMetadata
    {
	ValidMetadata();

	const SubvolumeWrapper f_subvolume;
	const SubvolumeWrapper f_subvolume_fullpath;

	map<string,string> f_raw_valid;
	map<string,string> f_raw_fullpath_valid;
    };

    struct ForeignSubvolume
    {
	ForeignSubvolume();

	const SubvolumeWrapper f_foreign_subvolume;
    };

    struct BtrfsImportConstructor : public ValidMetadata, ForeignSubvolume, BtrfsGeneralFixture
    {
	BtrfsImportConstructor();

	const string f_subvolume_missing;
	const string f_subvolume_empty;

	map<string,string> f_raw_foreign_fullpath;
	map<string,string> f_raw_missing;
	map<string,string> f_raw_empty;
	map<string,string> f_raw_invalid_key;
	map<string,string> f_root_volume;
	map<string,string> f_foreign_root_volume;
    };

    struct CompareData
    {
	CompareData();

	const SubvolumeWrapper f_subvolume_1;
	const SubvolumeWrapper f_subvolume_2;

	map<string,string> f_raw_valid_1;
	map<string,string> f_raw_fullpath_valid_1;

	map<string,string> f_raw_valid_2;
	map<string,string> f_raw_fullpath_valid_2;
    };

    struct CompareImportMetadata : public CompareData, BtrfsGeneralFixture
    {
	CompareImportMetadata();

	const snapper::BtrfsImportMetadata f_import_metadata_1;
	const snapper::BtrfsImportMetadata f_import_metadata_1_identical;

	const snapper::BtrfsImportMetadata f_import_metadata_2;
	const snapper::BtrfsImportMetadata f_import_metadata_2_identical;
    };


    struct CheckImportData
    {
	CheckImportData();

	const SubvolumeWrapper f_subvolume_rw_1;
	const SubvolumeWrapper f_subvolume_ro_1;

	const string f_subvolume_snapshots;

	map<string,string> f_raw_rw_subvolume;
	map<string,string> f_raw_ro_subvolume;

	map<string,string> f_raw_snapshots;
    };


    struct CheckImportMetadata : public CheckImportData, BtrfsGeneralFixture
    {
	CheckImportMetadata();

	const snapper::BtrfsImportMetadata f_clone_importdata_valid;
	const snapper::BtrfsImportMetadata f_clone_importdata_valid_2;

	const snapper::BtrfsImportMetadata f_adopt_import_valid;
	const snapper::BtrfsImportMetadata f_adopt_import_invalid;

	const snapper::BtrfsImportMetadata f_ack_import_valid;
	const snapper::BtrfsImportMetadata f_ack_import_invalid;

	const snapper::BtrfsImportMetadata f_snapshots_data_clone;
	const snapper::BtrfsImportMetadata f_snapshots_data_adopt;
	const snapper::BtrfsImportMetadata f_snapshots_data_ack;
    };


    struct GetSnapshotDirData
    {
	GetSnapshotDirData();

	const SubvolumeWrapper f_subvolume;

	map<string,string> f_raw_subvolume;
	map<string,string> f_raw_subvolume_fullpath;
	map<string,string> f_raw_subvolume_head_slash;
	map<string,string> f_raw_subvolume_trail_slash;
    };


    struct GetSnapshotDir : public GetSnapshotDirData, BtrfsGeneralFixture
    {
	GetSnapshotDir();

	const snapper::BtrfsImportMetadata f_metadata;
	const snapper::BtrfsImportMetadata f_metadata_fullpath;
	const snapper::BtrfsImportMetadata f_metadata_head_slash;
	const snapper::BtrfsImportMetadata f_metadata_trail_slash;
    };

    struct GetRawMetadata : public ValidMetadata, BtrfsGeneralFixture
    {
	GetRawMetadata();

	const snapper::BtrfsImportMetadata f_metadata;
    };

    struct DeleteImportSnapshotData
    {
	DeleteImportSnapshotData();

	const SubvolumeWrapper f_subvolume_1;
	const SubvolumeWrapper f_subvolume_2;

	map<string,string> f_raw_1;
	map<string,string> f_raw_2;
    };

    struct DeleteImportedSnapshot : public DeleteImportSnapshotData, BtrfsGeneralFixture
    {
	DeleteImportedSnapshot();

	const snapper::BtrfsImportMetadata f_metadata_simple;
	const snapper::BtrfsImportMetadata f_metadata_subdirs;
    };

}}
#endif // BTRFSIMPORTMETADATA_FIXTURES_H
