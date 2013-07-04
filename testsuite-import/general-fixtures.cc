#include <iostream>

#include "testsuite-import/general-fixtures.h"

namespace testsuiteimport
{
    namespace lvm
    {

	const string LvmGeneralFixture::f_conf_lvm_root_volume = "/testsuite-import";
	const string LvmGeneralFixture::f_conf_lvm_snapshots_prefix = LvmGeneralFixture::f_conf_lvm_root_volume + "/" + ".snapshots/";
	const string LvmGeneralFixture::f_conf_vg_name = "vg_test";
	const string LvmGeneralFixture::f_conf_origin_name = "lv_test_thin_1";

	const string LvmGeneralFixture::f_test_snapshot_01 = "lv_test_snapshot_01";

	LvmGeneralFixture::LvmGeneralFixture()
	    : f_snapper(new snapper::Snapper("testsuite-import")),
	    f_lvm(static_cast<const snapper::Lvm *>(f_snapper->getFilesystem()))
	{
	}

	LvmGeneralFixture::~LvmGeneralFixture()
	{
	    delete f_snapper;
	}
    }

    namespace btrfs
    {
	const string BtrfsGeneralFixture::f_conf_btrfs_root_volume = "/testsuite-import-btrfs";
	const string BtrfsGeneralFixture::f_conf_btrfs_snapshots_prefix = BtrfsGeneralFixture::f_conf_btrfs_root_volume + "/" + ".snapshots/";
	const string BtrfsGeneralFixture::f_conf_btrfs_foreign_root_volume = "/testsuite-import-btrfs-x";

	BtrfsGeneralFixture::BtrfsGeneralFixture()
	    : f_snapper(new snapper::Snapper("testsuite-import-btrfs")),
	    f_btrfs(static_cast<const snapper::Btrfs *>(f_snapper->getFilesystem()))
	{
	    std::cout << "BtrfsGeneralFixture done" << std::endl;
	}

	BtrfsGeneralFixture::~BtrfsGeneralFixture()
	{
	    delete f_snapper;
	}
    }
}
