#include "testsuite-import/lvmimportmetadata-fixtures.h"

namespace testsuiteimport { namespace lvm {

    using std::make_pair;

    ValidMetadata::ValidMetadata() : LvmGeneralFixture()
    {
	raw_data.insert(make_pair("vg_name", "some_string"));
	raw_data.insert(make_pair("lv_name", "some_string"));
	raw_data.insert(make_pair("the_answer", "42"));
    }

    MissingVgName::MissingVgName() : LvmGeneralFixture()
    {
	raw_data.insert(make_pair("lv_name", "some_string"));
	raw_data.insert(make_pair("the_answer", "42"));
    }

    MissingLvName::MissingLvName() : LvmGeneralFixture()
    {
	raw_data.insert(make_pair("vg_name", "some_string"));
	raw_data.insert(make_pair("the_answer", "42"));
    }

}}
