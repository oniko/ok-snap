#ifndef GENERAL_FIXTURES_H
#define GENERAL_FIXTURES_H

#include <string>

#include "snapper/Lvm.h"
#include "snapper/Snapper.h"

namespace testsuiteimport { namespace lvm
{
    using std::string;

// NOTE: do not add any fork/execs with childs possibly returning non-zero codes
// don't know how to disable --catch_system_errors in exec monitor for global
// fixtures :(
    struct LvmGlobalConfig {
	LvmGlobalConfig();
	~LvmGlobalConfig();
    };

    struct LvmGeneralFixture {
	LvmGeneralFixture();
	~LvmGeneralFixture();

	static const string f_snapshots_prefix;
	static const string f_conf_vg_name;
	static const string f_conf_origin_name;
	static const string f_test_snapshot_01;

	snapper::Snapper *f_snapper;
	const snapper::Lvm *f_lvm;
    };
}}
#endif //GENERAL_FIXTURES_H
