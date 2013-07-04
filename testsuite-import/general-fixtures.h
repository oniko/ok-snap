#ifndef GENERAL_FIXTURES_H
#define GENERAL_FIXTURES_H

#include <string>

#include "snapper/Btrfs.h"
#include "snapper/Lvm.h"
#include "snapper/Snapper.h"

#include "snapper/Logger.h"

namespace testsuiteimport
{
    using std::string;

    namespace lvm
    {
    

// NOTE: do not add any fork/execs with childs possibly returning non-zero codes
// don't know how to disable --catch_system_errors in exec monitor for global
// fixtures :(
    /*struct LvmGlobalConfig {
	LvmGlobalConfig();
	~LvmGlobalConfig();
    };*/

	struct LvmGeneralFixture {
	    LvmGeneralFixture();
	    ~LvmGeneralFixture();

	    static const string f_conf_lvm_root_volume;
	    static const string f_conf_lvm_snapshots_prefix;
	    static const string f_conf_vg_name;
	    static const string f_conf_origin_name;
	    static const string f_test_snapshot_01;

	    const snapper::Snapper *f_snapper;
	    const snapper::Lvm *f_lvm;
	};
    }


    namespace btrfs
    {
	struct BtrfsGeneralFixture {
	    BtrfsGeneralFixture();
	    ~BtrfsGeneralFixture();

	    static const string f_conf_btrfs_root_volume;
	    static const string f_conf_btrfs_snapshots_prefix;
	    static const string f_conf_btrfs_foreign_root_volume;

	    const snapper::Snapper *f_snapper;
	    const snapper::Btrfs *f_btrfs;
	};
    }
}
#endif //GENERAL_FIXTURES_H
