#include <boost/scoped_ptr.hpp>
#include <boost/test/unit_test.hpp>

#include "testsuite-import/btrfs-test.h"
#include "testsuite-import/helpers.h"

namespace testsuiteimport { namespace btrfs
{
    void
    BtrfsTestClass::tc_btrfs_ctor()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FBtrfsConstructor());
	fixture->test_method();
    }


    void
    BtrfsTestClass::tc_btrfs_clone_snapshot()
    {


    }


    void
    FBtrfsConstructor::test_method()
    {
	//boost::scoped_ptr<snapper::Btrfs> btrfs;

	BOOST_CHECK_NO_THROW( snapper::Btrfs btrfs(f_valid_volume) );
	BOOST_CHECK_THROW( snapper::Btrfs btrfs(f_missing_volume), snapper::InvalidConfigException );
	BOOST_CHECK_THROW( snapper::Btrfs btrfs(f_wrong_snapshots_subvolume), snapper::InvalidConfigException );

	deep_rmdirat(f_wrong_snapshots_subvolume);

	BOOST_CHECK_NO_THROW( snapper::Btrfs btrfs(f_wrong_snapshots_subvolume) );
    }

    void FCloneSnapshot::test_method()
    {
	BOOST_REQUIRE_NO_THROW( f_btrfs->cloneSnapshot(f_env_1.f_num, f_clone_subvolume_ro.subvolume) );
	BOOST_REQUIRE_NO_THROW( f_btrfs->cloneSnapshot(f_env_2.f_num, f_clone_subvolume_rw.subvolume) );
	BOOST_REQUIRE_NO_THROW( f_btrfs->cloneSnapshot(f_env_3.f_num, f_clone_subdirs.subvolume) );

	BOOST_CHECK( btrfs_subvolume_exists(f_env_1.fullpath + "/" + "snapshot") );
	BOOST_CHECK( btrfs_subvolume_exists(f_env_2.fullpath + "/" + "snapshot") );
	BOOST_CHECK( btrfs_subvolume_exists(f_env_3.fullpath + "/" + "snapshot") );

	BOOST_REQUIRE_THROW( f_btrfs->cloneSnapshot(f_fail_env.f_num, f_subvolume_missing), snapper::CreateSnapshotFailedException );
	BOOST_CHECK( !btrfs_subvolume_exists(f_fail_env.fullpath + "/" + "snapshot") );
    }
}}
