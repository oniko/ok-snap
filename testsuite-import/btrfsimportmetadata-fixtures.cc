#include <assert.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <sstream>

// get KEY_SUBVOLUME macro
#include "snapper/BtrfsImportMetadata.h"

#include "testsuite-import/helpers.h"
#include "testsuite-import/btrfsimportmetadata-fixtures.h"

namespace testsuiteimport { namespace btrfs
{
    using std::make_pair;

    SubvolumeWrapper::SubvolumeWrapper(const string& root_path, const string& subdirs, bool ro)
	: root_path(root_path),
	subvolume(subdirs + "/" + init_subvolume(root_path + "/" + deep_mkdirat(root_path, subdirs))),
	dirs(subdirs)
    {
	init(ro);
    }

    SubvolumeWrapper::SubvolumeWrapper(const string& root_path, bool ro)
	: root_path(root_path), subvolume(init_subvolume(root_path) + (ro ? "-snap-ro" : ""))
    {
	init(ro);
    }

    void
    SubvolumeWrapper::init(bool rdonly) const
    {
	std::cout << "root_path: " << root_path << ", dirs: " << dirs << ", subvolume: " << subvolume << std::endl;

	if (rdonly)
	{
	    if (dirs.empty())
		btrfs_create_snapshot_ro(root_path, root_path, subvolume);
	    else
		btrfs_create_snapshot_ro(root_path, root_path + "/" + dirs, subvolume.substr(dirs.length() + 1));
	}
	else
	{
	    if (dirs.empty())
		btrfs_create_subvolume(root_path, subvolume);
	    else
		btrfs_create_subvolume(root_path + "/" + dirs, subvolume.substr(dirs.length() + 1));
	}
    }

    SubvolumeWrapper::~SubvolumeWrapper()
    {
	string::size_type pos = subvolume.rfind("/");
	if (pos == string::npos)
	    pos = 0;
	else
	    pos++;

	try
	{
	    btrfs_delete_subvolume(root_path + "/" + dirs, subvolume.substr(pos));
	    // delete dirs...
	}
	// TODO: catch proper exception
	catch (const std::exception &e)
	{
	    std::cerr << "WAR: Couldn't delete subvolume: " << root_path << "/" << subvolume << std::endl;
	}
    }

    const string SubvolumeWrapper::f_subv_prefix = "test_subv";

    string SubvolumeWrapper::init_subvolume(const string& root_volume)
    {
	int rootfd = open(root_volume.c_str(), O_RDONLY | O_CLOEXEC | O_NOATIME);
	if (rootfd < 0)
	{
	    // TODO: create proper exception
	    std::cerr << "failed to open root volume" << std::endl;
	    throw std::exception();
	}

	unsigned int count = 0;

	std::ostringstream oss(std::ostringstream::ate);
	oss << f_subv_prefix << count;

	struct stat buf;

	while ((!fstatat(rootfd, oss.str().c_str(), &buf, AT_SYMLINK_NOFOLLOW) || errno != ENOENT) && count < 100)
	{
	    count++;

	    oss.clear();
	    oss.str(f_subv_prefix);
	    oss << count;
	}

	close(rootfd);

	if (count >= 100)
	{
	    std::cerr << "Ouch!" << std::endl;
	    throw std::exception();
	}

	return oss.str();
    }
/*
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

	//return dest_path + "/" + name;
	return name;
    }
*/

    ValidMetadata::ValidMetadata()
	: f_subvolume(BtrfsGeneralFixture::f_conf_root_volume),
	f_subvolume_fullpath(BtrfsGeneralFixture::f_conf_root_volume)
    {
	f_raw_valid.insert(make_pair(KEY_SUBVOLUME, f_subvolume.subvolume));
	f_raw_fullpath_valid.insert(make_pair(KEY_SUBVOLUME, f_subvolume.fullpath()));
    }


    ForeignSubvolume::ForeignSubvolume()
	: f_foreign_subvolume(BtrfsGeneralFixture::f_conf_foreign_root_volume)
    {
    }


    BtrfsImportConstructor::BtrfsImportConstructor()
	: ValidMetadata(), ForeignSubvolume(), BtrfsGeneralFixture(),
	f_subvolume_missing("AllWorkAndNoPlayMakesJackADullBoy"),
	f_subvolume_empty("	 ")
    {
	f_raw_foreign_fullpath.insert(make_pair(KEY_SUBVOLUME, f_foreign_subvolume.fullpath()));
	f_raw_missing.insert(make_pair(KEY_SUBVOLUME, f_subvolume_missing));
	f_raw_empty.insert(make_pair(KEY_SUBVOLUME, f_subvolume_empty));
	f_raw_invalid_key.insert(make_pair("subvolumE", f_subvolume.subvolume));
	f_root_volume.insert(make_pair(KEY_SUBVOLUME, BtrfsGeneralFixture::f_conf_root_volume));
	f_foreign_root_volume.insert(make_pair(KEY_SUBVOLUME, BtrfsGeneralFixture::f_conf_foreign_root_volume));
    }


    CompareData::CompareData()
	: f_subvolume_1(BtrfsGeneralFixture::f_conf_root_volume),
	f_subvolume_2(BtrfsGeneralFixture::f_conf_root_volume)
    {
	f_raw_valid_1.insert(make_pair(KEY_SUBVOLUME, f_subvolume_1.subvolume));
	f_raw_fullpath_valid_1.insert(make_pair(KEY_SUBVOLUME, f_subvolume_1.fullpath()));

	f_raw_valid_2.insert(make_pair(KEY_SUBVOLUME, f_subvolume_2.subvolume));
	f_raw_fullpath_valid_2.insert(make_pair(KEY_SUBVOLUME, f_subvolume_2.fullpath()));
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
	: f_subvolume_rw_1(BtrfsGeneralFixture::f_conf_root_volume),
	f_subvolume_ro_1(BtrfsGeneralFixture::f_conf_root_volume, true),
	f_subvolume_snapshots(".snapshots")
    {
	f_raw_ro_subvolume.insert(make_pair(KEY_SUBVOLUME, f_subvolume_ro_1.subvolume));
	f_raw_rw_subvolume.insert(make_pair(KEY_SUBVOLUME, f_subvolume_rw_1.subvolume));

	f_raw_snapshots.insert(make_pair(KEY_SUBVOLUME, f_subvolume_snapshots));
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
	: f_subvolume(BtrfsGeneralFixture::f_conf_root_volume)
    {
	f_raw_subvolume.insert(make_pair(KEY_SUBVOLUME, f_subvolume.subvolume));
	f_raw_subvolume_fullpath.insert(make_pair(KEY_SUBVOLUME, f_subvolume.fullpath()));
	f_raw_subvolume_head_slash.insert(make_pair(KEY_SUBVOLUME, "/" + f_subvolume.subvolume));
	f_raw_subvolume_trail_slash.insert(make_pair(KEY_SUBVOLUME, f_subvolume.subvolume + "/"));
    }


    GetSnapshotDir::GetSnapshotDir()
	: GetSnapshotDirData(), BtrfsGeneralFixture(),
	f_metadata(f_raw_subvolume, snapper::ImportPolicy::ADOPT, f_btrfs),
	f_metadata_fullpath(f_raw_subvolume_fullpath, snapper::ImportPolicy::ADOPT, f_btrfs),
	f_metadata_head_slash(f_raw_subvolume_head_slash, snapper::ImportPolicy::ADOPT, f_btrfs),
	f_metadata_trail_slash(f_raw_subvolume_trail_slash, snapper::ImportPolicy::ADOPT, f_btrfs)
    {
    }


    GetRawMetadata::GetRawMetadata()
	: ValidMetadata(), BtrfsGeneralFixture(),
	f_metadata(f_raw_valid, snapper::ImportPolicy::ACKNOWLEDGE, f_btrfs)
    {
    }


    DeleteImportSnapshotData::DeleteImportSnapshotData()
	: f_subvolume_1(BtrfsGeneralFixture::f_conf_root_volume),
	f_subvolume_2(BtrfsGeneralFixture::f_conf_root_volume, string("1/2/3"))
    {
	f_raw_1.insert(make_pair(KEY_SUBVOLUME, f_subvolume_1.subvolume));
	f_raw_2.insert(make_pair(KEY_SUBVOLUME, f_subvolume_2.subvolume));
    }


    DeleteImportedSnapshot::DeleteImportedSnapshot()
	: DeleteImportSnapshotData(), BtrfsGeneralFixture(),
	f_metadata_simple(f_raw_1, snapper::ImportPolicy::ADOPT, f_btrfs),
	f_metadata_subdirs(f_raw_2, snapper::ImportPolicy::ADOPT, f_btrfs)
    {
	assert(btrfs_subvolume_exists(f_subvolume_1.fullpath()));
	assert(btrfs_subvolume_exists(f_subvolume_2.fullpath()));
    }
}}
