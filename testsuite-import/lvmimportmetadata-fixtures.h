#ifndef LVMIMPORTMETADATA_FIXTURES_H
#define LVMIMPORTMETADATA_FIXTURES_H

#include <map>
#include <string>

#include "testsuite-import/general-fixtures.h"

namespace testsuiteimport { namespace lvm {

    using std::map;
    using std::string;

    struct ValidMetadata : public LvmGeneralFixture {
	ValidMetadata();
	~ValidMetadata() {}

	map<string,string> raw_data;
    };

    struct MissingVgName : public LvmGeneralFixture {
	MissingVgName();
	~MissingVgName() {}

	map<string,string> raw_data;
    };

    struct MissingLvName : public LvmGeneralFixture {
	MissingLvName();
	~MissingLvName() {}

	map<string,string> raw_data;
    };
}}
#endif //LVMIMPORTMETADATA_FIXTURES_H
