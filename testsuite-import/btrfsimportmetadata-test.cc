#include <boost/test/unit_test.hpp>
#include <boost/scoped_ptr.hpp>

#include "snapper/BtrfsImportMetadata.h"

#include "testsuite-import/helpers.h"
#include "testsuite-import/btrfsimportmetadata-test.h"

namespace testsuiteimport { namespace btrfs
{
    void BtrfsImportMetadataTestClass::tc_import_ctor()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FBtrfsImportConstructor());
	fixture->test_method();
    }

    void BtrfsImportMetadataTestClass::tc_import_compare_metadata()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FBtrfsImportConstructor());
	fixture->test_method();
    }


    void BtrfsImportMetadataTestClass::tc_import_check_imported_snapshot()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FImportCheckImportedMetadata());
	fixture->test_method();
    }

    void BtrfsImportMetadataTestClass::tc_import_get_snapshot_dir()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FGetSnapshotDir());
	fixture->test_method();
    }

    
    void BtrfsImportMetadataTestClass::tc_import_get_raw_metadata()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FGetRawData());
	fixture->test_method();
    }


    void BtrfsImportMetadataTestClass::tc_import_delete_snapshot()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FDeleteImportedSnapshot());
	fixture->test_method();
    }


    void FBtrfsImportConstructor::test_method()
    {
	boost::scoped_ptr<snapper::ImportMetadata> p_idata;

	BOOST_CHECK_NO_THROW(p_idata.reset(new snapper::BtrfsImportMetadata(f_raw_valid, snapper::ImportPolicy::CLONE, f_btrfs)));
	BOOST_CHECK_NO_THROW(p_idata.reset(new snapper::BtrfsImportMetadata(f_raw_fullpath_valid, snapper::ImportPolicy::CLONE, f_btrfs)));

	BOOST_CHECK_THROW(p_idata.reset(new snapper::BtrfsImportMetadata(f_raw_foreign_fullpath, snapper::ImportPolicy::CLONE, f_btrfs)), snapper::InvalidImportMetadataException);
	BOOST_CHECK_THROW(p_idata.reset(new snapper::BtrfsImportMetadata(f_raw_missing, snapper::ImportPolicy::CLONE, f_btrfs)), snapper::InvalidImportMetadataException);
	BOOST_CHECK_THROW(p_idata.reset(new snapper::BtrfsImportMetadata(f_raw_empty, snapper::ImportPolicy::CLONE, f_btrfs)), snapper::InvalidImportMetadataException);
	BOOST_CHECK_THROW(p_idata.reset(new snapper::BtrfsImportMetadata(f_raw_invalid_key, snapper::ImportPolicy::CLONE, f_btrfs)), snapper::InvalidImportMetadataException);
	BOOST_CHECK_THROW(p_idata.reset(new snapper::BtrfsImportMetadata(f_root_volume, snapper::ImportPolicy::CLONE, f_btrfs)), snapper::InvalidImportMetadataException);
    }


    void FImportCompareMetadata::test_method()
    {
	BOOST_CHECK(f_import_metadata_1 == f_import_metadata_1_identical);
	BOOST_CHECK(f_import_metadata_1_identical == f_import_metadata_1);

	BOOST_CHECK(f_import_metadata_2 == f_import_metadata_2_identical);
	BOOST_CHECK(f_import_metadata_2_identical == f_import_metadata_2);

	BOOST_CHECK(!(f_import_metadata_1 == f_import_metadata_2));
	BOOST_CHECK(!(f_import_metadata_2 == f_import_metadata_1));

	BOOST_CHECK(!(f_import_metadata_1_identical == f_import_metadata_2_identical));
	BOOST_CHECK(!(f_import_metadata_2_identical == f_import_metadata_1_identical));

	BOOST_CHECK(!(f_import_metadata_1 == f_import_metadata_2_identical));
	BOOST_CHECK(!(f_import_metadata_2_identical == f_import_metadata_1));

	BOOST_CHECK(!(f_import_metadata_2 == f_import_metadata_1_identical));
	BOOST_CHECK(!(f_import_metadata_1_identical == f_import_metadata_2));
     }


    void FImportCheckImportedMetadata::test_method()
    {
	BOOST_CHECK(f_clone_importdata_valid.checkImportedSnapshot());
	BOOST_CHECK(f_clone_importdata_valid_2.checkImportedSnapshot());

	BOOST_CHECK(f_adopt_import_valid.checkImportedSnapshot());
	BOOST_CHECK(!f_adopt_import_invalid.checkImportedSnapshot());

	BOOST_CHECK(f_ack_import_valid.checkImportedSnapshot());
	BOOST_CHECK(!f_ack_import_invalid.checkImportedSnapshot());

	BOOST_CHECK(!f_snapshots_data_clone.checkImportedSnapshot());
	BOOST_CHECK(!f_snapshots_data_adopt.checkImportedSnapshot());
	BOOST_CHECK(!f_snapshots_data_ack.checkImportedSnapshot());
    }

    void FGetSnapshotDir::test_method()
    {
	const string expected(f_subvolume.fullpath());

	BOOST_CHECK_EQUAL(f_metadata.getSnapshotDir(0), expected);
	BOOST_CHECK_EQUAL(f_metadata_fullpath.getSnapshotDir(0), expected);
	BOOST_CHECK_EQUAL(f_metadata_head_slash.getSnapshotDir(0), expected);
	BOOST_CHECK_EQUAL(f_metadata_trail_slash.getSnapshotDir(0), expected);
    }

    void FGetRawData::test_method()
    {
	map<string,string> result;
	map<string,string>::const_iterator cit;

	BOOST_REQUIRE_NO_THROW(result = f_metadata.raw_metadata());

	cit = result.find(KEY_SUBVOLUME);

	BOOST_REQUIRE(cit != result.end());
	BOOST_CHECK_EQUAL(cit->second, f_subvolume.subvolume);
    }

    void FDeleteImportedSnapshot::test_method()
    {
	BOOST_REQUIRE_NO_THROW(f_metadata_simple.deleteImportedSnapshot());
	BOOST_CHECK(!btrfs_subvolume_exists(f_subvolume_1.fullpath()));

	BOOST_REQUIRE_NO_THROW(f_metadata_subdirs.deleteImportedSnapshot());
	BOOST_CHECK(!btrfs_subvolume_exists(f_subvolume_2.fullpath()));
    }
}}
