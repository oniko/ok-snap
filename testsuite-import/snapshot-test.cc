#include <boost/scoped_ptr.hpp>
#include <boost/test/unit_test.hpp>

#include "testsuite-import/helpers.h"
#include "testsuite-import/snapshot-test.h"

namespace testsuiteimport
{
    void
    SnapshotTestClass::tc_snapshot_ctor()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FSnapsotCtor(fixtures->ctor_fixture()));
	fixture->test_method();
    }


    FSnapshotCtor::FSnapshotCtor(const GeneralSnapshotCtorFixture& gfix)
	: f(gfix), f_type(snapper::SnapshotType::PRE), f_num(42),
	f_date(1234554321), f_ctor_clone_passed(false), f_ctor_adopt_passed(false),
	f_ctor_ack_passed(false)
    {
    }


    FSnapshotCtor::~FSnapshotCtor()
    {
	if (!f_ctor_clone_passed)
	    delete f.clone_data;
	if (!f_ctor_adopt_passed)
	    delete f.adopt_data;
	if (!f_ctor_ack_passed)
	    delete f.ack_data;
    }


    FSnapshotGetImportPolicy::FSnapshotGetImportPolicy(const GeneralGetImportPolicyFixture& gfix)
	: f_sh_none(gfix.snapper, snapper::SnapshotType::SINGLE, 42, 1234554321),
	f_sh_clone(gfix.snapper, snapper::SnapshotType::SINGLE, 43, 1234554321, gfix.clone_data),
	f_sh_adopt(gfix.snapper, snapper::SnapshotType::SINGLE, 44, 1234554321, gfix.adopt_data),
	f_sh_ack(gfix.snapper, snapper::SnapshotType::SINGLE, 44, 1234554321, gfix.ack_data)
    {
    }


    FSnapshotMountFilesystem::FSnapshotMountFilesystem(const GeneralMountFilesystemFixture& gfix)
	: f(gfix),
	f_sh_none(f.snapper, snapper::SnapshotType::PRE, f.subvol_none->get_num(), 1234554321),
	f_sh_none_user(f.snapper, snapper::SnapshotType::PRE, f.subvol_none_user->get_num(), 1234554321),
	f_sh_clone(f.snapper, snapper::SnapshotType::PRE, f.subvol_clone->get_num(), 1234554321, f.im_clone),
	f_sh_clone_user(f.snapper, snapper::SnapshotType::PRE, f.subvol_clone_user->get_num(), 1234554321, f.im_clone_user),
	f_sh_adopt(f.snapper, snapper::SnapshotType::PRE, f.subvol_adopt->get_num(), 1234554321, f.im_adopt),
	f_sh_adopt_user(f.snapper, snapper::SnapshotType::PRE, f.subvol_adopt_user->get_num(), 1234554321, f.im_adopt_user),
	f_sh_ack(f.snapper, snapper::SnapshotType::PRE, f.subvol_ack->get_num(), 1234554321, f.im_ack),
	f_sh_ack_user(f.snapper, snapper::SnapshotType::PRE, f.subvol_ack_user->get_num(), 1234554321, f.im_ack_user)
    {
    }


    FSnapshotMountFilesystem::~FSnapshotMountFilesystem()
    {
	delete f.subvol_none;
	delete f.subvol_none_user;
	delete f.subvol_clone;
	delete f.subvol_clone_user;
	delete f.subvol_clone_orig;
	delete f.subvol_clone_orig_user;
	delete f.subvol_adopt;
	delete f.subvol_adopt_user;
	delete f.subvol_ack;
	delete f.subvol_ack_user;
    }


    void FSnapshotCtor::test_method()
    {
	boost::scoped_ptr<snapper::Snapshot> snapshot;

	BOOST_REQUIRE_NO_THROW( snapshot.reset(new snapper::Snapshot(f_snapper, f_type, f_num, f_date)) );
	BOOST_CHECK_EQUAL( snapshot->getType(), f_type );
	BOOST_CHECK_EQUAL( snapshot->getNum(), f_num );
	BOOST_CHECK_EQUAL( snapshot->getDate(), f_date );
	BOOST_CHECK_EQUAL( snapshot->getUid(), 0 );
	BOOST_CHECK_EQUAL( snapshot->getPreNum(), 0 );

	BOOST_REQUIRE_NO_THROW( snapshot.reset(new snapper::Snapshot(f_snapper, f_type, f_num, f_date, f_clone_data)) );
	f_ctor_clone_passed = true;
	BOOST_CHECK_EQUAL( snapshot->getType(), f_type );
	BOOST_CHECK_EQUAL( snapshot->getNum(), f_num );
	BOOST_CHECK_EQUAL( snapshot->getDate(), f_date );
	BOOST_CHECK_EQUAL( snapshot->getUid(), 0 );
	BOOST_CHECK_EQUAL( snapshot->getPreNum(), 0 );

	BOOST_REQUIRE_NO_THROW( snapshot.reset(new snapper::Snapshot(f_snapper, f_type, f_num, f_date, f_adopt_data)) );
	f_ctor_adopt_passed = true;
	BOOST_CHECK_EQUAL( snapshot->getType(), f_type );
	BOOST_CHECK_EQUAL( snapshot->getNum(), f_num );
	BOOST_CHECK_EQUAL( snapshot->getDate(), f_date );
	BOOST_CHECK_EQUAL( snapshot->getUid(), 0 );
	BOOST_CHECK_EQUAL( snapshot->getPreNum(), 0 );

	BOOST_REQUIRE_NO_THROW( snapshot.reset(new snapper::Snapshot(f_snapper, f_type, f_num, f_date, f_ack_data)) );
	f_ctor_ack_passed = true;
	BOOST_CHECK_EQUAL( snapshot->getType(), f_type );
	BOOST_CHECK_EQUAL( snapshot->getNum(), f_num );
	BOOST_CHECK_EQUAL( snapshot->getDate(), f_date );
	BOOST_CHECK_EQUAL( snapshot->getUid(), 0 );
	BOOST_CHECK_EQUAL( snapshot->getPreNum(), 0 );
    }


    void FSnapshotGetImportPolicy::test_method()
    {
	BOOST_CHECK_EQUAL( f_sh_none.getImportPolicy(), snapper::ImportPolicy::NONE );
	BOOST_CHECK_EQUAL( f_sh_clone.getImportPolicy(), snapper::ImportPolicy::CLONE );
	BOOST_CHECK_EQUAL( f_sh_adopt.getImportPolicy(), snapper::ImportPolicy::ADOPT );
	BOOST_CHECK_EQUAL( f_sh_ack.getImportPolicy(), snapper::ImportPolicy::ACKNOWLEDGE );
    }


    void FSnapshotGetSnapshotDir::test_method()
    {
	BOOST_CHECK_EQUAL( f_sh_none.snapshotDir(), f.expected_none );
	BOOST_CHECK_EQUAL( f_sh_clone.snapshotDir(), f.expected_clone );
	BOOST_CHECK_EQUAL( f_sh_adopt.snapshotDir(), f.expected_adopt );
	BOOST_CHECK_EQUAL( f_sh_ack.snapshotDir(), f.expected_ack );
    }


    void FSnapshotMountFilesystem::test_method()
    {
	// ------------------- NONE
	BOOST_REQUIRE_NO_THROW( f_sh_none.mountFilesystemSnapshot(false) );
	BOOST_CHECK_EQUAL( f.subvol_none->is_mounted(), true );
	BOOST_CHECK_NO_THROW( f_sh_none.mountFilesystemSnapshot(false) );

	// ------------------- NONE user
	BOOST_REQUIRE_NO_THROW( f_sh_none_user.mountFilesystemSnapshot(true) );
	BOOST_CHECK_EQUAL( f.subvol_none_user->is_mounted(), true );
	BOOST_CHECK_NO_THROW( f_sh_none_user.mountFilesystemSnapshot(true) );

	// ------------------- CLONE
	BOOST_REQUIRE_NO_THROW( f_sh_clone.mountFilesystemSnapshot(false) );
	BOOST_CHECK_EQUAL( f.subvol_clone->is_mounted(), true );
	BOOST_CHECK_EQUAL( f.subvol_clone_orig->is_mounted(), false );
	BOOST_CHECK_NO_THROW( f_sh_clone.mountFilesystemSnapshot(false) );

	// ------------------- CLONE user
	BOOST_REQUIRE_NO_THROW( f_sh_clone_user.mountFilesystemSnapshot(true) );
	BOOST_CHECK_EQUAL( f.subvol_clone_user->is_mounted(), true );
	BOOST_CHECK_EQUAL( f.subvol_clone_orig_user->is_mounted(), false );
	BOOST_CHECK_NO_THROW( f_sh_clone_user.mountFilesystemSnapshot(true) );

	// ------------------- ADOPT
	BOOST_REQUIRE_NO_THROW( f_sh_adopt.mountFilesystemSnapshot(true) );
	BOOST_CHECK_EQUAL( f.subvol_adopt->is_mounted(), true );
	BOOST_CHECK_NO_THROW( f_sh_adopt.mountFilesystemSnapshot(true) );

	// ------------------- ADOPT user
	BOOST_REQUIRE_NO_THROW( f_sh_adopt_user.mountFilesystemSnapshot(true) );
	BOOST_CHECK_EQUAL( f.subvol_adopt_user->is_mounted(), true );
	BOOST_CHECK_NO_THROW( f_sh_adopt_user.mountFilesystemSnapshot(true) );

	// ------------------- ACKNOWLEDGE
	BOOST_REQUIRE_NO_THROW( f_sh_ack.mountFilesystemSnapshot(true) );
	BOOST_CHECK_EQUAL( f.subvol_ack->is_mounted(), true );
	BOOST_CHECK_NO_THROW( f_sh_ack.mountFilesystemSnapshot(true) );

	// ------------------- ACKNOWLEDGE user
	BOOST_REQUIRE_NO_THROW( f_sh_ack_user.mountFilesystemSnapshot(true) );
	BOOST_CHECK_EQUAL( f.subvol_ack_user->is_mounted(), true );
	BOOST_CHECK_NO_THROW( f_sh_ack_user.mountFilesystemSnapshot(true) );
    }
}
