
#ifndef BTRFSIMPORTMETADATA_FIXTURES_H
#define BTRFSIMPORTMETADATA_FIXTURES_H

#include <string>

#include "testsuite-import/general-fixtures.h"

namespace testsuiteimport { namespace btrfs
{
    using std::string;

    struct ValidMetadata : public BtrfsGeneralFixture
    {
	ValidMetadata();
	~ValidMetadata();

	static const string f_subv_prefix = "test_subv";

	string subvolume;
    };
}}
#endif // BTRFSIMPORTMETADATA_FIXTURES_H
