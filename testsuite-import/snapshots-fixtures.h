#ifndef SNAPSHOTS_FIXTURES_H
#define SNAPSHOTS_FIXTURES_H

#include "testsuite-import/lvm-fixtures.h"

#define private public
    #include "snapper/Snapshot.h"
#define private private

#include "snapper/Snapper.h"
#include "snapper/LvmImportMetadata.h"

namespace testsuiteimport { namespace lvm
{
    struct ImportHelperTypeNone
    {
	ImportHelperTypeNone();

	const snapper::Snapper* f_dummy_snapper;
	const snapper::Snapshots f_shs;
	const snapper::Snapshot f_sh;
    };

    struct ImportHelperTypeClone : public LvmGeneralFixture
    {
	ImportHelperTypeClone();

	const snapper::LvmImportMetadata* f_p_idata;
	const snapper::Snapper* f_dummy_snapper;
	const snapper::Snapshots f_shs;
	const snapper::Snapshot f_sh;
    };
}}
#endif // SNAPSHOTS_FIXTURES_H
