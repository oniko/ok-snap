
#ifndef BTRFSIMPORTMETADATA_FIXTURES_H
#define BTRFSIMPORTMETADATA_FIXTURES_H

#include <map>
#include <string>

#include "snapper/BtrfsImportMetadata.h"

#include "testsuite-import/btrfs-fixtures.h"
#include "testsuite-import/general-fixtures.h"

namespace testsuiteimport { namespace btrfs
{
    using std::string;
    using std::map;

    class BtrfsMetadata
    {
    private:
	static const string f_subv_prefix = "test_subv";
    protected:
	static string init_subvolume(const string& root_volume);
	static string init_snapshot_ro(const string& source, const string& subvolume, const string& name);
    };

    struct ValidMetadata : public BtrfsMetadata
    {
	ValidMetadata();
	~ValidMetadata();

	const string f_subvolume;
	const string f_fullpath_subvolume;
    };

    struct ForeignSubvolume : public BtrfsMetadata
    {
	ForeignSubvolume();
	~ForeignSubvolume();

	const string f_foreign_subvolume;
    };

    struct BtrfsImportConstructor : public ValidMetadata, ForeignSubvolume, BtrfsGeneralFixture
    {
	BtrfsImportConstructor();

	const string f_subvolume_missing;
	const string f_subvolume_empty;

	map<string,string> f_raw_valid;
	map<string,string> f_raw_fullpath_valid;
	map<string,string> f_raw_foreign;
	map<string,string> f_raw_missig;
	map<string,string> f_raw_empty;
	map<string,string> f_raw_invalid_key;
    };

    struct CompareData : public BtrfsMetadata
    {
	CompareData();
	~CompareData();

	const string f_valid_1;
	const string f_valid_2;

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


    struct CheckImportData : BtrfsMetadata
    {
	CheckImportData();
	~CheckImportData();

	const string f_subvolume_rw_1;
	const string f_subvolume_ro_1;

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


    struct GetSnapshotDirData : public BtrfsMetadata
    {
	GetSnapshotDirData();
	~GetSnapshotDirData();

	const string f_subvolume;
	const string f_subvolume_fullpath;

	map<string,string> f_raw_subvolume;
	map<string,string> f_raw_subvolume_fullpath;
    };


    struct GetSnapshotDir : public GetSnapshotDirData, BtrfsGeneralFixture
    {
	GetSnapshotDir();

	const snapper::BtrfsImportMetadata f_metadata;
	const snapper::BtrfsImportMetadata f_metadata_fullpath;
    };

}}
#endif // BTRFSIMPORTMETADATA_FIXTURES_H
