#include <boost/test/unit_test.hpp>
#include <boost/scoped_ptr.hpp>

#include "snapper/BtrfsImportMetadata.h"

#include "testsuite-import/btrfsimportmetadata-test.h"

namespace testsuiteimport { namespace btrfs
{
    void BtrfsImportMetadataTest::tc_import_ctor()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FBtrfsImportConstructor());
	fixture->test_method();
    }


    void BtrfsImportMetadataTest::tc_import_compare_metadata()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FBtrfsImportConstructor());
	fixture->test_method();
    }


    void BtrfsImportMetadataTest::tc_import_check_imported_snapshot()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FImportCheckImportedMetadata());
	fixture->test_method();
    }


    void FBtrfsImportConstructor::test_method()
    {
	boost::scoped_ptr<snapper::ImportMetadata> p_idata;

	BOOST_CHECK_NO_THROW(p_idata.reset(new snapper::BtrfsImportMetadata(f_raw_valid, snapper::ImportPolicy::CLONE, f_btrfs)));
	BOOST_CHECK_NO_THROW(p_idata.reset(new snapper::BtrfsImportMetadata(f_raw_fullpath_valid, snapper::ImportPolicy::CLONE, f_btrfs)));

	BOOST_CHECK_THROW(p_idata.reset(new snapper::BtrfsImportMetadata(f_raw_foreign, snapper::ImportPolicy::CLONE, f_btrfs)), snapper::InvalidImportMetadataException);
	BOOST_CHECK_THROW(p_idata.reset(new snapper::BtrfsImportMetadata(f_raw_missig, snapper::ImportPolicy::CLONE, f_btrfs)), snapper::InvalidImportMetadataException);
	BOOST_CHECK_THROW(p_idata.reset(new snapper::BtrfsImportMetadata(f_raw_empty, snapper::ImportPolicy::CLONE, f_btrfs)), snapper::InvalidImportMetadataException);
	BOOST_CHECK_THROW(p_idata.reset(new snapper::BtrfsImportMetadata(f_raw_invalid_key, snapper::ImportPolicy::CLONE, f_btrfs)), snapper::InvalidImportMetadataException);
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
}}
