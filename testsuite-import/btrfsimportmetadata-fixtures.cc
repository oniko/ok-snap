#include <fcntl.h>
#include <sys/stat.h>

#include <sstream>

// get KEY_SUBVOLUME macro
#include "snapper/BtrfsImportMetadata.h"

#include "testsuite-import/helpers.h"
#include "testsuite-import/btrfsimportmetadata-fixtures.h"

namespace testsuiteimport { namespace btrfs
{
    string BtrfsMetadata::init_subvolume(const string& root_volume)
    {
	int rootfd = open(root_volume.c_str(), O_RDONLY | O_CLOEXEC | O_NOATIME);
	if (rootfd < 0)
	{
	    // TODO: create proper exception
	    throw std::exception();
	}

	unsigned int count = 0;

	std::ostringstream oss;
	oss << f_subv_prefix << count;

	struct stat buf;

	while (fstatat(rootfd, oss.str().c_str(), &buf, AT_SYMLINK_NOFOLLOW) && count < 100)
	{
	    count++;

	    if (errno != ENOENT)
	    {
		close(rootfd);
		// TODO: create proper exception
		throw std::exception();
	    }

	    oss.str(f_subv_prefix);
	    oss.clear();
	    oss << count;
	}

	close(rootfd);

	if (count >= 100)
	{
	    throw std::exception();
	}

	btrfs_create_subvolume(root_volume, oss.str());

	return oss.str();
    }

    string BtrfsMetadata::init_snapshot_ro(const string& root_volume, const string& dest_path, const string& name)
    {
	string tmp_dest = dest_path;

	string::size_type pos = tmp_dest.find(root_volume);
	if (pos != 0)
	{
	    tmp_dest = root_volume + "/" + dest_path;
	}

	std::cout << "root_volume: " << root_volume << ", dest_path: " << dest_path
		  << ", tmp_subv: " << tmp_dest << std::endl;

	btrfs_create_snapshot_ro(root_volume, tmp_dest, name);

	return dest_path + "/" + name;
    }


    ValidMetadata::ValidMetadata()
	: f_subvolume(init_subvolume(BtrfsGeneralFixture::f_conf_root_volume)),
	f_fullpath_subvolume(BtrfsGeneralFixture::f_conf_root_volume + "/" + init_subvolume(BtrfsGeneralFixture::f_conf_root_volume))
    {
    }

    ValidMetadata::~ValidMetadata()
    {
	try
	{
	    btrfs_delete_subvolume(BtrfsGeneralFixture::f_conf_root_volume, f_subvolume);
	} catch(...)
	{
	}

	btrfs_delete_subvolume(BtrfsGeneralFixture::f_conf_root_volume, f_fullpath_subvolume.substr(BtrfsGeneralFixture::f_conf_root_volume.length()) + 1);
    }


    ForeignSubvolume::ForeignSubvolume()
	: f_foreign_subvolume(BtrfsGeneralFixture::f_conf_foreign_root_volume + "/" + init_subvolume(BtrfsGeneralFixture::f_conf_foreign_root_volume))
    {
    }


    ForeignSubvolume::~ForeignSubvolume()
    {
	btrfs_delete_subvolume(BtrfsGeneralFixture::f_conf_foreign_root_volume, f_foreign_subvolume);
    }

    BtrfsImportConstructor::BtrfsImportConstructor()
	: ValidMetadata(), ForeignSubvolume(), BtrfsGeneralFixture(),
	f_subvolume_missing("AllWorkAndNoPlayMakesJackADullBoy"),
	f_subvolume_empty("	 ")
    {
	f_raw_valid.insert(make_pair(KEY_SUBVOLUME, f_subvolume));
	f_raw_fullpath_valid.insert(make_pair(KEY_SUBVOLUME, f_raw_fullpath_valid));
	f_raw_foreign.insert(make_pair(KEY_SUBVOLUME, f_foreign_subvolume));
	f_raw_missig.insert(make_pair(KEY_SUBVOLUME, f_subvolume_missing));
	f_raw_empty.insert(make_pair(KEY_SUBVOLUME, f_subvolume_empty));
	f_raw_invalid_key.insert(make_pair("subvolumE", f_subvolume));
    }

    CompareData::CompareData()
	: f_valid_1(init_subvolume(BtrfsGeneralFixture::f_conf_root_volume)),
	f_valid_2(init_subvolume(BtrfsGeneralFixture::f_conf_root_volume))
    {
	f_raw_valid_1.insert(make_pair(KEY_SUBVOLUME, f_valid_1));
	f_raw_fullpath_valid_1.make_pair(KEY_SUBVOLUME, BtrfsGeneralFixture::f_conf_root_volume + "/" + f_valid_1);

	f_raw_valid_2.insert(make_pair(KEY_SUBVOLUME, f_valid_2));
	f_raw_fullpath_valid_2.make_pair(KEY_SUBVOLUME, BtrfsGeneralFixture::f_conf_root_volume + "/" + f_valid_2);
    }

    CompareData::~CompareData()
    {
	try
	{
	    btrfs_delete_subvolume(BtrfsGeneralFixture::f_conf_root_volume, f_valid_1);
	} catch(...)
	{
	}

	btrfs_delete_subvolume(BtrfsGeneralFixture::f_conf_root_volume, f_valid_2);
    }

    CompareImportMetadata::CompareImportMetadata()
	: CompareData(), BtrfsGeneralFixture(),
	f_import_metadata_1(f_raw_valid_1, snapper::ImportPolicy::CLONE, f_btrfs),
	f_import_metadata_1_identical(f_raw_fullpath_valid_1, snapper::ImportPolicy::CLONE, f_btrfs),
	f_import_metadata_2(f_raw_valid_2, snapper::ImportPolicy::CLONE, f_btrfs),
	f_import_metadata_2_identical(f_raw_fullpath_valid_2, snapper::ImportPolicy::CLONE, f_btrfs)
    {
    }

    CheckImportData::CheckImportData()
	: f_subvolume_rw_1(init_subvolume(BtrfsGeneralFixture::f_conf_root_volume)),
	f_subvolume_ro_1(init_snapshot_ro(BtrfsGeneralFixture::f_conf_root_volume, f_subvolume_rw_1, f_subvolume_rw_1 + "-ro-snap")),
	f_subvolume_snapshots(".snapshots")
    {
	f_raw_ro_subvolume.insert(make_pair(KEY_SUBVOLUME, f_subvolume_ro_1));
	f_raw_rw_subvolume.insert(make_pair(KEY_SUBVOLUME, f_subvolume_rw_1));

	f_raw_snapshots.insert(make_pair(KEY_SUBVOLUME, f_subvolume_snapshots));
    }


    CheckImportData::~CheckImportData()
    {
	try
	{
	    btrfs_delete_subvolume(BtrfsGeneralFixture::f_conf_root_volume, f_subvolume_ro_1);
	}
	catch (...)
	{
	}
	btrfs_delete_subvolume(BtrfsGeneralFixture::f_conf_root_volume, f_subvolume_rw_1);
    }


    CheckImportMetadata::CheckImportMetadata()
	: CheckImportData(), BtrfsGeneralFixture(),
	f_clone_importdata_valid(f_raw_ro_subvolume, snapper::ImportPolicy::CLONE, f_btrfs),
	f_clone_importdata_valid_2(f_raw_rw_subvolume, snapper::ImportPolicy::CLONE, f_btrfs),
	f_adopt_import_valid(f_raw_ro_subvolume, snapper::ImportPolicy::ADOPT, f_btrfs),
	f_adopt_import_invalid(f_raw_rw_subvolume, snapper::ImportPolicy::ADOPT, f_btrfs),
	f_ack_import_valid(f_raw_ro_subvolume, snapper::ImportPolicy::ACKNOWLEDGE, f_btrfs),
	f_ack_import_invalid(f_raw_rw_subvolume, snapper::ImportPolicy::ACKNOWLEDGE, f_btrfs),
	f_snapshots_data_clone(f_raw_snapshots, snapper::ImportPolicy::CLONE, f_btrfs),
	f_snapshots_data_adopt(f_raw_snapshots, snapper::ImportPolicy::ADOPT, f_btrfs),
	f_snapshots_data_ack(f_raw_snapshots, snapper::ImportPolicy::ACKNOWLEDGE, f_btrfs)
    {
    }


    GetSnapshotDirData::GetSnapshotDirData()
	: f_subvolume(init_subvolume(BtrfsGeneralFixture::f_conf_root_volume)),
	f_subvolume_fullpath(BtrfsGeneralFixture::f_conf_root_volume + "/" + f_subvolume)
    {
	f_raw_subvolume.insert(make_pair(KEY_SUBVOLUME, f_subvolume));
	f_raw_subvolume_fullpath.insert(make_pair(KEY_SUBVOLUME, f_subvolume_fullpath));
    }


    GetSnapshotDirData::~GetSnapshotDirData()
    {
	btrfs_delete_subvolume(BtrfsGeneralFixture::f_conf_root_volume, f_subvolume);
    }


    GetSnapshotDir::GetSnapshotDir()
	: GetSnapshotDirData(), BtrfsGeneralFixture(),
	f_metadata(f_raw_subvolume, snapper::ImportPolicy::ADOPT, f_btrfs),
	f_metadata_fullpath(f_raw_subvolume_fullpath, snapper::ImportPolicy::ADOPT, f_btrfs)
    {
    }
}}
