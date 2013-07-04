#ifndef BTRFS_FIXTURES_H
#define BTRFS_FIXTURES_H

// move SubvolumeWrapper to general include file
#include "testsuite-import/btrfsimportmetadata-fixtures.h"
#include "testsuite-import/general-fixtures.h"

namespace testsuiteimport { namespace btrfs
{
    struct BtrfsConstructor : public BtrfsGeneralFixture
    {
	BtrfsConstructor();
	~BtrfsConstructor();

	// valid root volume w/ .snapshots subvolume
	const string f_valid_volume;
	// missing root volume
	const string f_missing_volume;
	// btrfs subvolume w/ .snapshots directory (not subvolume)
	const string f_wrong_snapshots_subvolume;
    };

    struct CreateSnapshotDirectory
    {
	CreateSnapshotDirectory();
	~CreateSnapshotDirectory();

	unsigned int f_num;
	string fullpath;
    };

    struct CloneSnapshot : public BtrfsGeneralFixture
    {
	CloneSnapshot();

	const string f_subdirs;
	const string f_subvolume_missing;

	const SubvolumeWrapper f_clone_subvolume_ro;
	const SubvolumeWrapper f_clone_subvolume_rw;
	const SubvolumeWrapper f_clone_subdirs;

	const CreateSnapshotDirectory f_env_1;
	const CreateSnapshotDirectory f_env_2;
	const CreateSnapshotDirectory f_env_3;
	const CreateSnapshotDirectory f_fail_env;
    };
}}
#endif // BTRFS_FIXTURES_H
