#include <boost/test/unit_test.hpp>

// move SubvolumeWrapper
#include "testsuite-import/btrfs-fixtures.h"
#include "testsuite-import/helpers.h"

namespace testsuiteimport { namespace btrfs
{
    BtrfsConstructor::BtrfsConstructor()
	: f_valid_volume(BtrfsGeneralFixture::f_conf_btrfs_root_volume),
	f_missing_volume("/path/to/some/missing/volume/"),
	f_wrong_snapshots_subvolume(BtrfsConstructor::f_conf_btrfs_foreign_root_volume)
    {
	deep_mkdirat(f_wrong_snapshots_subvolume, ".snapshots");
    }

    BtrfsConstructor::~BtrfsConstructor()
    {
	deep_rmdirat(f_wrong_snapshots_subvolume);
    }

    CreateSnapshotDirectory::CreateSnapshotDirectory()
	: f_num(0)
    {
	std::cout << "CreateSnapshotEnvironment ctor" << std::endl;

	std::ostringstream oss(std::ostringstream::ate);
	oss << f_num;

	while (!bool_deep_mkdirat(BtrfsGeneralFixture::f_conf_btrfs_snapshots_prefix, oss.str()) && f_num < 100)
	{
	    oss.clear();
	    oss.str(string());
	    oss << ++f_num;
	}

	if (f_num >= 100)
	    BOOST_FAIL( "Something went terribly wrong" );

	fullpath = BtrfsGeneralFixture::f_conf_btrfs_snapshots_prefix + oss.str();
    }

    CreateSnapshotDirectory::~CreateSnapshotDirectory()
    {
	try
	{
	    btrfs_delete_subvolume(fullpath, "snapshot");
	}
	catch (...)
	{}

	deep_rmdirat(fullpath);
	rmdir(fullpath.c_str());
    }


    CloneSnapshot::CloneSnapshot()
	: BtrfsGeneralFixture(), f_subdirs("1/2a/3"), f_subvolume_missing("i/have/no/body"),
	f_clone_subvolume_ro(BtrfsGeneralFixture::f_conf_btrfs_root_volume, true),
	f_clone_subvolume_rw(BtrfsGeneralFixture::f_conf_btrfs_root_volume),
	f_clone_subdirs(BtrfsGeneralFixture::f_conf_btrfs_root_volume, f_subdirs),
	f_env_1(), f_env_2(), f_env_3(), f_fail_env()
    {
    }

}}
