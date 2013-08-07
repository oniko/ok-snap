#ifndef LVM_IHELPER_FIXTURES_H
#define LVM_IHELPER_FIXTURES_H

#include "testsuite-import/import-helper-testsuite.h"

namespace testsuiteimport { namespace lvm
{
    class LvmImportHelperFixFactory : public LvmGeneralFixture, ImportCheckerFixture
    {
    public:
	LvmImportHelperFixFactory() : LvmGeneralFixture(), ImportCheckerFixture(f_snapper) {}

	virtual ValidImportCheckFixtures import_valid_fixs() const;
	virtual ImportOriginCheckFixtures import_origin_fixs(snapper::ImportPolicy origin) const;
	virtual ImportDupliciteCheckFixtures import_duplicite_fixs(snapper::ImportPolicy orig, snapper::ImportPolicy import) const;
	virtual ImportDupliciteClassicalFixtures import_classical_fixs(snapper::ImportPolicy orig) const;
    };
}}
#endif //LVM_IHELPER_FIXTURES_H
