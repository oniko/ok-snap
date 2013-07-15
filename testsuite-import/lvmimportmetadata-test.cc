#include <boost/test/unit_test.hpp>
#include <boost/scoped_ptr.hpp>

#include "snapper/Exception.h"

#include "testsuite-import/helpers.h"
#include "testsuite-import/general-test.h"
#include "testsuite-import/lvmimportmetadata-test.h"

namespace testsuiteimport { namespace lvm
{
    void
    LvmImportMetadataTestClass::tc_import_ctor()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FLvmImportConstructor());
	fixture->test_method();
    }


    void
    LvmImportMetadataTestClass::tc_import_compare_metadata()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FLvmCompareCheck());
	fixture->test_method();
    }


    void
    LvmImportMetadataTestClass::tc_import_check_imported_snapshot()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FLvmIMDataCheckImportedSnapshot());
	fixture->test_method();
    }


    void
    LvmImportMetadataTestClass::tc_clone_imported_snapshot()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FCloneImportedSnapshot());
	fixture->test_method();
    }


    void
    LvmImportMetadataTestClass::tc_delete_imported_snapshot()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FDeleteImportedSnapshot());
	fixture->test_method();
    }


    void
    LvmImportMetadataTestClass::tc_import_raw_metadata()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FGetRawData());
	fixture->test_method();
    }


    void
    LvmImportMetadataTestClass::tc_import_get_snapshot_dir()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FGetSnapshotDir());
	fixture->test_method();
    }


    void
    FLvmImportConstructor::test_method()
    {
	boost::scoped_ptr<snapper::LvmImportMetadata> p_imdata;

	BOOST_CHECK_THROW( p_imdata.reset(new snapper::LvmImportMetadata(f_raw_data_missing_vg, snapper::ImportPolicy::NONE, f_dummy_lvm)), snapper::InvalidImportMetadataException );
	BOOST_CHECK_THROW( p_imdata.reset(new snapper::LvmImportMetadata(f_raw_data_missing_lv, snapper::ImportPolicy::NONE, f_dummy_lvm)), snapper::InvalidImportMetadataException );

	BOOST_CHECK_NO_THROW( p_imdata.reset(new snapper::LvmImportMetadata(f_raw_data, snapper::ImportPolicy::CLONE, f_dummy_lvm)) );
	BOOST_CHECK_THROW( p_imdata.reset(new snapper::LvmImportMetadata(f_raw_data, snapper::ImportPolicy::NONE, f_dummy_lvm)), snapper::InvalidImportMetadataException );
    }


    void
    FLvmCompareCheck::test_method()
    {
	BOOST_CHECK_EQUAL( f_lvm_import_metadata, f_lvm_import_metadata_identical );
	BOOST_CHECK_NE( f_lvm_import_metadata, f_lvm_import_metadata_diff_in_lv );
	BOOST_CHECK_NE( f_lvm_import_metadata, f_lvm_import_metadata_diff_in_vg );
	BOOST_CHECK_NE( f_lvm_import_metadata, f_lvm_import_metadata_different );
    }


    void
    FLvmIMDataCheckImportedSnapshot::test_method()
    {
	// check CLONE import type
	BOOST_CHECK_EQUAL( f_clone_import_data_valid_ro, true );
	BOOST_CHECK_EQUAL( f_clone_import_data_valid_rw, true );
	BOOST_CHECK_EQUAL( f_clone_import_missing_lv, false );
	BOOST_CHECK_EQUAL( f_clone_import_nonthin_lv, false );
	BOOST_CHECK_EQUAL( f_clone_import_foreign_vg, false ); // expected failure
	BOOST_CHECK_EQUAL( f_clone_import_current_subvolume, false );

	// check ADOPT import type
	BOOST_CHECK_EQUAL( f_adopt_import_data_valid_ro, true );
	BOOST_CHECK_EQUAL( f_adopt_import_data_rw, false );
	BOOST_CHECK_EQUAL( f_adopt_import_missing_lv, false );
	BOOST_CHECK_EQUAL( f_adopt_import_nonthin_lv, false );
	BOOST_CHECK_EQUAL( f_adopt_import_foreign_vg, false ); // expected failure
	BOOST_CHECK_EQUAL( f_adopt_import_current_subvolume, false );

	// check ACKNOWLEDGE import type
	BOOST_CHECK_EQUAL( f_ack_import_data_valid_ro, true );
	BOOST_CHECK_EQUAL( f_ack_import_data_valid_rw, false );
	BOOST_CHECK_EQUAL( f_ack_import_missing_lv, false );
	BOOST_CHECK_EQUAL( f_ack_import_nonthin_lv, false );
	BOOST_CHECK_EQUAL( f_ack_import_foreign_vg, false ); // expected failure
	BOOST_CHECK_EQUAL( f_ack_import_current_subvolume, false );
    }


    void
    FCloneImportedSnapshot::test_method()
    {
	BOOST_REQUIRE_NO_THROW( f_clone_valid_metadata.cloneImportedSnapshot(f_env.f_num) );
	BOOST_CHECK_EQUAL( check_lv_exists(f_origin_volume.vg_name, f_lvm->snapshotLvName(f_env.f_num)), true );
    }


    void
    FDeleteImportedSnapshot::test_method()
    {
	BOOST_REQUIRE_NO_THROW( f_clone_valid_metadata.deleteImportedSnapshot() );
	BOOST_CHECK_EQUAL( check_lv_exists(f_origin_volume.vg_name, f_origin_volume.lv_name), false );
    }


    void
    FGetRawData::test_method()
    {
	BOOST_CHECK_EQUAL( f_lvm_import_metadata.get_raw_metadata(), f_raw_data );
    }


    void
    FGetSnapshotDir::test_method()
    {
	BOOST_CHECK_EQUAL( f_lvm_import_metadata.getSnapshotDir(42), f_lvm->snapshotDir(42) );
    }

}}
