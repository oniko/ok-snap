#include <boost/bind.hpp>
#include <boost/scoped_ptr.hpp>

#include "testsuite-import/import-helper-testsuite.h"

namespace testsuiteimport
{
    using boost::unit_test::test_case;


    FImportValid::FImportValid(const ValidImportCheckFixtures& gfix)
	: f(gfix),
	f_sh_a(f.snapper, snapper::SnapshotType::SINGLE, f.subv_clone_orig->get_num(), (time_t) -1, f.im_clone),
	f_sh_b(f.snapper, snapper::SnapshotType::SINGLE, f.subv_adopt->get_num(), (time_t) -1, f.im_adopt),
	f_sh_c(f.snapper, snapper::SnapshotType::SINGLE, f.subv_ack->get_num(), (time_t) -1, f.im_ack)
    {
    }


    FImportValid::~FImportValid()
    {
	try
	{
	    f.snapper->getFilesystem()->deleteSnapshot(f_sh_a.getNum());
	}
	catch (const ::snapper::DeleteSnapshotFailedException& e)
	{
	}
    }


    FImportOrigin::FImportOrigin(const ImportOriginCheckFixtures& gfix)
	: f(gfix),
	sh_import(f.snapper, ::snapper::SnapshotType::SINGLE, f.info_dir.get()->f_num, (time_t) -1, f.im_origin),
	ihelper(sh_import, f.snapper->getSnapshots())
    {
    }


    FImportOrigin::~FImportOrigin()
    {
	try
	{
	    if (sh_import.getImportPolicy() == ::snapper::ImportPolicy::CLONE)
		f.snapper->getFilesystem()->deleteSnapshot(sh_import.getNum());
	}
	catch (const ::snapper::DeleteSnapshotFailedException& e)
	{
	}
    }


    ImportDuplicite::ImportDuplicite(const ImportDupliciteCheckFixtures& gfix, const string& desc)
	: f(gfix),
	sh_import(f.snapper, snapper::SnapshotType::SINGLE, f.info_import.get()->f_num, (time_t)-1, f.im_import),
	ih(sh_import, f.snapper->getSnapshots()),
	it(f.snapper->importSingleSnapshot(desc, f.shared_orig.get()->getImportPolicy(), f.shared_orig.get()->get_raw_metadata()))
    {
    }


    ImportDuplicite::~ImportDuplicite()
    {
	try
	{
	    f.snapper->deleteSnapshot(it);
	}
	catch (const ::snapper::IllegalSnapshotException& e)
	{
	}
	catch (const ::snapper::IOErrorException e)
	{
	}
	try
	{
	    if (sh_import.getImportPolicy() == ::snapper::ImportPolicy::CLONE)
	    {
		f.snapper->getFilesystem()->deleteSnapshot(sh_import.getNum());
	    }
	}
	catch (const ::snapper::DeleteSnapshotFailedException& e)
	{
	}
    }


    ImportDupliciteClassical::ImportDupliciteClassical(const ImportDupliciteClassicalFixtures& gfix)
	: f(gfix),
	sh_import(f.snapper, snapper::SnapshotType::SINGLE, f.info_import.get()->f_num, (time_t)-1, f.im_import),
	ih(sh_import, f.snapper->getSnapshots())
    {
    }


    ImportDupliciteClassical::~ImportDupliciteClassical()
    {
	try
	{
	    f.snapper->deleteSnapshot(f.iter);
	}
	catch (const ::snapper::IllegalSnapshotException& e)
	{
	}
	catch (const ::snapper::IOErrorException e)
	{
	}
	try
	{
	    if (sh_import.getImportPolicy() == ::snapper::ImportPolicy::CLONE)
	    {
		f.snapper->getFilesystem()->deleteSnapshot(sh_import.getNum());
	    }
	}
	catch (const ::snapper::DeleteSnapshotFailedException& e)
	{
	}
    }


    FImportDupliciteInvalid::FImportDupliciteInvalid(const ImportDupliciteCheckFixtures& gfix)
	: ImportDuplicite(gfix, "FImportDupliciteInvalid")
    {
    }


    FImportDupliciteValid::FImportDupliciteValid(const ImportDupliciteCheckFixtures& gfix)
	: ImportDuplicite(gfix, "FImportDupliciteValid")
    {
    }


    FImportClassicDupliciteValid::FImportClassicDupliciteValid(const ImportDupliciteClassicalFixtures& gfix)
	: ImportDupliciteClassical(gfix)
    {
    }


    FImportClassicDupliciteInvalid::FImportClassicDupliciteInvalid(const ImportDupliciteClassicalFixtures& gfix)
	: ImportDupliciteClassical(gfix)
    {
    }


    ImportDupliciteCheckFixtures::ImportDupliciteCheckFixtures(const ::snapper::Snapper* snapper,
							       const shared_csubv subv_orig,
							       const shared_cinfodir info_import,
							       const boost::shared_ptr<::snapper::ImportMetadata> shared_orig,
							       const ::snapper::ImportMetadata* im_import)
	: snapper(const_cast<::snapper::Snapper *>(snapper)), subv_orig(subv_orig), info_import(info_import),
	shared_orig(shared_orig), im_import(im_import)
    {
    }


    ImportDupliciteClassicalFixtures::ImportDupliciteClassicalFixtures(::snapper::Snapshots::iterator iter,
								       const ::snapper::Snapper* snapper,
								       const shared_cinfodir info_import,
								       const ::snapper::ImportMetadata* im_import)
	: iter(iter), snapper(const_cast<::snapper::Snapper *>(snapper)), info_import(info_import),
	im_import(im_import)
    {
    }


    ImportOriginCheckFixtures::ImportOriginCheckFixtures(const ::snapper::Snapper* snapper,
							 const shared_cinfodir info_dir,
							 const ::snapper::ImportMetadata* im_origin)
	: snapper(snapper), info_dir(info_dir), im_origin(im_origin)
    {
    }


    ValidImportCheckFixtures::ValidImportCheckFixtures(const ::snapper::Snapper* snapper,
						       const shared_csubv subv_clone,
						       const shared_csubv subv_adop,
						       const shared_csubv subv_ack,
						       const ::snapper::ImportMetadata* im_clone,
						       const ::snapper::ImportMetadata* im_adopt,
						       const ::snapper::ImportMetadata* im_ack)
	: snapper(snapper), subv_clone_orig(subv_clone), subv_adopt(subv_adop),
	subv_ack(subv_ack), im_clone(im_clone), im_adopt(im_adopt), im_ack(im_ack)
    {
    }


    void
    ImportHelperTestSuite::tc_import_valid()
    {
	std::cout << "tc_import_valid" << std::endl;

	boost::scoped_ptr<GeneralFixture> fixture(new FImportValid(fix_fact->import_valid_fixs()));
	fixture->test_method();
    }


    void
    ImportHelperTestSuite::tc_import_clone_origin()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FImportOrigin(fix_fact->import_origin_fixs(::snapper::ImportPolicy::CLONE)));
	fixture->test_method();
    }


    void
    ImportHelperTestSuite::tc_import_adopt_origin()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FImportOrigin(fix_fact->import_origin_fixs(::snapper::ImportPolicy::ADOPT)));
	fixture->test_method();
    }


    void
    ImportHelperTestSuite::tc_import_ack_origin()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FImportOrigin(fix_fact->import_origin_fixs(::snapper::ImportPolicy::ACKNOWLEDGE)));
	fixture->test_method();
    }


    void
    ImportHelperTestSuite::tc_import_clone_cloned()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FImportDupliciteValid(fix_fact->import_duplicite_fixs(snapper::ImportPolicy::CLONE, snapper::ImportPolicy::CLONE)));
	fixture->test_method();
    }


    void
    ImportHelperTestSuite::tc_import_adopt_cloned()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FImportDupliciteValid(fix_fact->import_duplicite_fixs(snapper::ImportPolicy::CLONE, snapper::ImportPolicy::ADOPT)));
	fixture->test_method();
    }


    void
    ImportHelperTestSuite::tc_import_ack_cloned()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FImportDupliciteValid(fix_fact->import_duplicite_fixs(snapper::ImportPolicy::CLONE, snapper::ImportPolicy::ACKNOWLEDGE)));
	fixture->test_method();
    }


    void
    ImportHelperTestSuite::tc_import_clone_adopted()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FImportDupliciteValid(fix_fact->import_duplicite_fixs(snapper::ImportPolicy::ADOPT, snapper::ImportPolicy::CLONE)));
	fixture->test_method();
    }


    void
    ImportHelperTestSuite::tc_import_adopt_adopted()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FImportDupliciteInvalid(fix_fact->import_duplicite_fixs(snapper::ImportPolicy::ADOPT, snapper::ImportPolicy::ADOPT)));
	fixture->test_method();
    }


    void
    ImportHelperTestSuite::tc_import_ack_adopted()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FImportDupliciteInvalid(fix_fact->import_duplicite_fixs(snapper::ImportPolicy::ADOPT, snapper::ImportPolicy::ACKNOWLEDGE)));
	fixture->test_method();
    }


    void
    ImportHelperTestSuite::tc_import_clone_acked()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FImportDupliciteValid(fix_fact->import_duplicite_fixs(snapper::ImportPolicy::ACKNOWLEDGE, snapper::ImportPolicy::CLONE)));
	fixture->test_method();
    }


    void
    ImportHelperTestSuite::tc_import_adopt_acked()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FImportDupliciteInvalid(fix_fact->import_duplicite_fixs(snapper::ImportPolicy::ACKNOWLEDGE, snapper::ImportPolicy::ADOPT)));
	fixture->test_method();
    }


    void
    ImportHelperTestSuite::tc_import_ack_acked()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FImportDupliciteInvalid(fix_fact->import_duplicite_fixs(snapper::ImportPolicy::ACKNOWLEDGE, snapper::ImportPolicy::ACKNOWLEDGE)));
	fixture->test_method();
    }


    void
    ImportHelperTestSuite::tc_import_clone_classical_snapshot()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FImportClassicDupliciteValid(fix_fact->import_classical_fixs(snapper::ImportPolicy::CLONE)));
	fixture->test_method();
    }


    void
    ImportHelperTestSuite::tc_import_adopt_classical_snapshot()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FImportClassicDupliciteInvalid(fix_fact->import_classical_fixs(snapper::ImportPolicy::ADOPT)));
	fixture->test_method();
    }


    void
    ImportHelperTestSuite::tc_import_ack_classical_snapshot()
    {
	boost::scoped_ptr<GeneralFixture> fixture(new FImportClassicDupliciteInvalid(fix_fact->import_classical_fixs(snapper::ImportPolicy::ACKNOWLEDGE)));
	fixture->test_method();
    }


    void
    FImportValid::test_method()
    {
	boost::scoped_ptr<::snapper::ImportHelper> imc_a;
	boost::scoped_ptr<::snapper::ImportHelper> imc_b;
	boost::scoped_ptr<::snapper::ImportHelper> imc_c;

	BOOST_REQUIRE_NO_THROW( imc_a.reset(new ::snapper::ImportHelper(f_sh_a, f.snapper->getSnapshots())) );
	BOOST_REQUIRE_NO_THROW( imc_b.reset(new ::snapper::ImportHelper(f_sh_b, f.snapper->getSnapshots())) );
	BOOST_REQUIRE_NO_THROW( imc_c.reset(new ::snapper::ImportHelper(f_sh_c, f.snapper->getSnapshots())) );

	BOOST_CHECK_NO_THROW( imc_a.get()->importSnapshot(); );
	BOOST_CHECK_NO_THROW( imc_b.get()->importSnapshot(); );
	BOOST_CHECK_NO_THROW( imc_c.get()->importSnapshot(); );
    }


    void
    FImportOrigin::test_method()
    {
	BOOST_CHECK_THROW( ihelper.importSnapshot(), snapper::IllegalSnapshotException );
    }


    void
    FImportDupliciteValid::test_method()
    {
	BOOST_CHECK_NO_THROW( ih.importSnapshot() );
    }


    void
    FImportDupliciteInvalid::test_method()
    {
	BOOST_CHECK_THROW( ih.importSnapshot(), snapper::ImportSnapshotFailedException );
    }


    void
    FImportClassicDupliciteValid::test_method()
    {
	BOOST_CHECK_NO_THROW( ih.importSnapshot() );
    }


    void
    FImportClassicDupliciteInvalid::test_method()
    {
	BOOST_CHECK_THROW( ih.importSnapshot(), snapper::ImportSnapshotFailedException );
    }


    ImportHelperTestSuite::ImportHelperTestSuite(const string& fs_type, const ImportHelperFixtures* fix_fact)
	: test_suite("ImportHelper - " + fs_type), fix_fact(fix_fact)
    {
 	test_case* import_valid = BOOST_TEST_CASE( boost::bind(&ImportHelperTestSuite::tc_import_valid, this) );
 	test_case* import_origin = BOOST_TEST_CASE( boost::bind(&ImportHelperTestSuite::tc_import_clone_origin, this) );

	test_case* import_clone_cloned = BOOST_TEST_CASE( boost::bind(&ImportHelperTestSuite::tc_import_clone_cloned, this) );
 	test_case* import_adopt_cloned = BOOST_TEST_CASE( boost::bind(&ImportHelperTestSuite::tc_import_adopt_cloned, this) );
	test_case* import_ack_cloned = BOOST_TEST_CASE( boost::bind(&ImportHelperTestSuite::tc_import_ack_cloned, this) );
	test_case* import_clone_adopted = BOOST_TEST_CASE( boost::bind(&ImportHelperTestSuite::tc_import_clone_adopted, this) );
	test_case* tc_import_adopt_adopted = BOOST_TEST_CASE( boost::bind(&ImportHelperTestSuite::tc_import_adopt_adopted, this) );
	test_case* tc_import_ack_adopted = BOOST_TEST_CASE( boost::bind(&ImportHelperTestSuite::tc_import_ack_adopted, this) );
	test_case* tc_import_clone_acked = BOOST_TEST_CASE( boost::bind(&ImportHelperTestSuite::tc_import_clone_acked, this) );
	test_case* tc_import_adopt_acked = BOOST_TEST_CASE( boost::bind(&ImportHelperTestSuite::tc_import_adopt_acked, this) );
	test_case* tc_import_ack_acked = BOOST_TEST_CASE( boost::bind(&ImportHelperTestSuite::tc_import_ack_acked, this) );

	test_case* tc_import_clone_classical_snapshot = BOOST_TEST_CASE( boost::bind(&ImportHelperTestSuite::tc_import_clone_classical_snapshot, this) );
	test_case* tc_import_adopt_classical_snapshot = BOOST_TEST_CASE( boost::bind(&ImportHelperTestSuite::tc_import_adopt_classical_snapshot, this) );
	test_case* tc_import_ack_classical_snapshot = BOOST_TEST_CASE( boost::bind(&ImportHelperTestSuite::tc_import_ack_classical_snapshot, this) );

	import_origin->depends_on(import_valid);
	import_clone_cloned->depends_on(import_valid);
	import_adopt_cloned->depends_on(import_valid);
	import_ack_cloned->depends_on(import_valid);
	import_clone_adopted->depends_on(import_valid);
	tc_import_adopt_adopted->depends_on(import_valid);
	tc_import_ack_adopted->depends_on(import_valid);
	tc_import_clone_acked->depends_on(import_valid);
	tc_import_adopt_acked->depends_on(import_valid);
	tc_import_ack_acked->depends_on(import_valid);

	tc_import_clone_classical_snapshot->depends_on(import_valid);
	tc_import_adopt_classical_snapshot->depends_on(import_valid);
	tc_import_ack_classical_snapshot->depends_on(import_valid);

	add(import_valid);
	add(import_origin);

	add(import_clone_cloned);
	add(import_adopt_cloned);
	add(import_ack_cloned);
	add(import_clone_adopted);
	add(tc_import_adopt_adopted);
	add(tc_import_ack_adopted);
	add(tc_import_clone_acked);
	add(tc_import_adopt_acked);
	add(tc_import_ack_acked);

	add(tc_import_clone_classical_snapshot);
	add(tc_import_adopt_classical_snapshot);
	add(tc_import_ack_classical_snapshot);
    }
}
