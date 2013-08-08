#ifndef IMPORT_CHECKER_TESTSUITE_H
#define IMPORT_CHECKER_TESTSUITE_H

#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/test/unit_test.hpp>

#include "snapper/ImportMetadata.h"
#include "snapper/Snapper.h"
#include "snapper/Snapshot.h"


#include "testsuite-import/general-fixtures.h"
#include "testsuite-import/general-test.h"

namespace testsuiteimport
{
    using std::string;

    typedef const boost::shared_ptr<const SubvolumeWrapper> shared_csubv;
    typedef const boost::shared_ptr<const InfoDirectory> shared_cinfodir;


    struct ValidImportCheckFixtures
    {
	ValidImportCheckFixtures(const ::snapper::Snapper* snapper,
				 const shared_csubv subv_clone,
				 const shared_csubv subv_adop,
				 const shared_csubv subv_ack,
				 const ::snapper::ImportMetadata* im_clone,
				 const ::snapper::ImportMetadata* im_adopt,
				 const ::snapper::ImportMetadata* im_ack
				 );

	const ::snapper::Snapper* snapper;

	const shared_csubv subv_clone_orig;
	const shared_csubv subv_adopt;
	const shared_csubv subv_ack;

	// ImportMetadata must be owned by Snapshot class or destroyed properly
	const ::snapper::ImportMetadata* im_clone;
	const ::snapper::ImportMetadata* im_adopt;
	const ::snapper::ImportMetadata* im_ack;
    };


    struct ImportOriginCheckFixtures
    {
	ImportOriginCheckFixtures(const ::snapper::Snapper *snapper, const shared_cinfodir info_dir, const ::snapper::ImportMetadata* im_origin);

	const ::snapper::Snapper *snapper;
	const shared_cinfodir info_dir;
	const ::snapper::ImportMetadata* im_origin;
    };


    struct ImportDupliciteCheckFixtures
    {
	ImportDupliciteCheckFixtures(const ::snapper::Snapper *snapper,
				     const shared_csubv subv_orig,
				     const shared_cinfodir info_import,
				     const boost::shared_ptr<::snapper::ImportMetadata> shared_orig,
				     const ::snapper::ImportMetadata* im_import);

	::snapper::Snapper *snapper;

	const shared_csubv subv_orig;

	const shared_cinfodir info_import;

	const boost::shared_ptr<::snapper::ImportMetadata> shared_orig;
	const ::snapper::ImportMetadata* im_import;
    };


    struct ImportDupliciteClassicalFixtures
    {
	ImportDupliciteClassicalFixtures(::snapper::Snapshots::iterator iter,
					 const ::snapper::Snapper *snapper,
					 const shared_cinfodir info_import,
					 const ::snapper::ImportMetadata* im_import);

	::snapper::Snapshots::iterator iter;

	::snapper::Snapper *snapper;

	const shared_cinfodir info_import;

	const ::snapper::ImportMetadata* im_import;
    };


    struct ImportHelperFixtures
    {
	ImportHelperFixtures(const ::snapper::Snapper* snapper) : snapper(snapper) {}
	virtual ~ImportHelperFixtures() {}

	virtual ValidImportCheckFixtures import_valid_fixs() const = 0;
	virtual ImportOriginCheckFixtures import_origin_fixs(::snapper::ImportPolicy origin) const = 0;
	virtual ImportDupliciteCheckFixtures import_duplicite_fixs(::snapper::ImportPolicy orig, ::snapper::ImportPolicy import) const = 0;
	virtual ImportDupliciteClassicalFixtures import_classical_fixs(::snapper::ImportPolicy orig) const = 0;

    protected:
	const ::snapper::Snapper* snapper;
    };


    struct FImportValid : public GeneralFixture
    {
	FImportValid(const ValidImportCheckFixtures& gfix);
	~FImportValid();

	const ValidImportCheckFixtures f;

	const ::snapper::Snapshot f_sh_a;
	const ::snapper::Snapshot f_sh_b;
	const ::snapper::Snapshot f_sh_c;

	virtual void test_method();
    };


    struct FImportOrigin : public GeneralFixture
    {
	FImportOrigin(const ImportOriginCheckFixtures& gfix);
	~FImportOrigin();

	const ImportOriginCheckFixtures f;

	const ::snapper::Snapshot sh_import;
	const ::snapper::ImportHelper ihelper;

	virtual void test_method();
    };


    struct ImportDuplicite
    {
	ImportDuplicite(const ImportDupliciteCheckFixtures& gfix, const string& desc);
	virtual ~ImportDuplicite();

	const ImportDupliciteCheckFixtures f;

	const ::snapper::Snapshot sh_import;

	const ::snapper::ImportHelper ih;

	// TODO: private
	::snapper::Snapshots::iterator it;
    };


    struct ImportDupliciteClassical
    {
	ImportDupliciteClassical(const ImportDupliciteClassicalFixtures& gfix);
	virtual ~ImportDupliciteClassical();

	const ImportDupliciteClassicalFixtures f;

	const ::snapper::Snapshot sh_import;

	const ::snapper::ImportHelper ih;
    };


    struct FImportDupliciteValid : public GeneralFixture, ImportDuplicite
    {
	FImportDupliciteValid(const ImportDupliciteCheckFixtures& gfix);

	virtual void test_method();
    };


    struct FImportDupliciteInvalid : public GeneralFixture, ImportDuplicite
    {
	FImportDupliciteInvalid(const ImportDupliciteCheckFixtures& gfix);

	virtual void test_method();
    };


    struct FImportClassicDupliciteValid : public GeneralFixture, ImportDupliciteClassical
    {
	FImportClassicDupliciteValid(const ImportDupliciteClassicalFixtures& gfix);

	virtual void test_method();
    };


    struct FImportClassicDupliciteInvalid : public GeneralFixture, ImportDupliciteClassical
    {
	FImportClassicDupliciteInvalid(const ImportDupliciteClassicalFixtures& gfix);

	virtual void test_method();
    };


    class ImportHelperTestSuite : public boost::unit_test::test_suite
    {
    public:
	ImportHelperTestSuite(const string& fs_type, const ImportHelperFixtures* fix_fact);
	~ImportHelperTestSuite() { delete fix_fact; }

	void tc_import_valid();

	void tc_import_clone_origin();
	void tc_import_adopt_origin();
	void tc_import_ack_origin();

	void tc_import_clone_cloned();
	void tc_import_adopt_cloned();
	void tc_import_ack_cloned();
	void tc_import_clone_adopted();
	void tc_import_adopt_adopted();
	void tc_import_ack_adopted();
	void tc_import_clone_acked();
	void tc_import_adopt_acked();
	void tc_import_ack_acked();

	void tc_import_clone_classical_snapshot();
	void tc_import_adopt_classical_snapshot();
	void tc_import_ack_classical_snapshot();
    private:
	const ImportHelperFixtures* fix_fact;
    };
}
#endif // IMPORT_CHECKER_TESTSUITE_H
