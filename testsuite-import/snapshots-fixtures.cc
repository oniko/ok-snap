#include "testsuite-import/snapshots-fixtures.h"

namespace testsuiteimport { namespace lvm
{
    ImportHelperTypeNone::ImportHelperTypeNone()
	: f_dummy_snapper(static_cast<const snapper::Snapper *>(135790)),
	f_shs(f_dummy_snapper),
	f_sh(f_dummy_snapper, snapper::SnapshotType::SINGLE, 42, (time_t) -1)
    {
    }

    ImportHelperTypeClone::ImportHelperTypeClone()
	:
    {
    }

}}
