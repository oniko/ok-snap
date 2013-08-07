#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <iostream>

#include <boost/test/unit_test.hpp>

#include "testsuite-import/helpers.h"
#include "testsuite-import/general-fixtures.h"

namespace testsuiteimport
{
    void InfoDirectory::infodir_init()
    {
	std::cout << "CreateSnapshotEnvironment ctor" << std::endl;

	int dirfd = open(infos_dir.c_str(), O_RDONLY | O_NOATIME | O_CLOEXEC | O_NOFOLLOW);
	if (dirfd < 0)
	    BOOST_FAIL( "Couldn't open " << infos_dir << " directory" );

	std::ostringstream oss(std::ios_base::ate);
	oss << f_num;

	while (mkdirat(dirfd, oss.str().c_str(), 0755) && f_num < 100)
	{
	    f_num++;

	    oss.clear();
	    oss.str("");
	    oss << f_num;

	    if (errno == EEXIST)
		continue;

	    BOOST_FAIL( "Couldn't create the info directory" );
	}

	if (f_num >= 100)
	    BOOST_FAIL( "Something went terribly wrong" );

	f_info_dir = infos_dir + "/" + oss.str();
	
	std::cout << "info dir: " << f_info_dir << std::endl;

	f_dirfd = openat(dirfd, oss.str().c_str(), O_RDONLY | O_NOATIME | O_CLOEXEC | O_NOFOLLOW);

	if (f_dirfd < 0)
	    BOOST_FAIL( "Can't open: " << oss );

	struct stat buff;

	if (fstat(f_dirfd, &buff) || !S_ISDIR(buff.st_mode))
	{
	    close(f_dirfd);
	    BOOST_FAIL( "Can't stat dir: " << f_info_dir << " or the d-entry is not a directory" );
	}
    }

    InfoDirectory::InfoDirectory(const string& infos_dir_loc)
	: f_num(0), infos_dir(infos_dir_loc)
    {
	infodir_init();
    }

    InfoDirectory::InfoDirectory(const string& infos_dir_loc, unsigned int num)
	: f_num(num), infos_dir(infos_dir_loc)
    {
	infodir_init();
    }

    InfoDirectory::~InfoDirectory()
    {
	std::cout << "InfoDirectory dtor for: " << f_info_dir << std::endl;
	deep_rmdirat(f_dirfd);
	close(f_dirfd);

	if (rmdir(f_info_dir.c_str()))
	{
	    perror("rmdir");
	    BOOST_TEST_MESSAGE( "Can't remove: " << f_info_dir );
	}
	std::cout << "InfoDirectory dtor for: " << f_info_dir << " done" << std::endl;
    }

    void InfoDirWithSnapshotDir::init()
    {
	if (mkdirat(f_dirfd, "snapshot", 0755))
	    BOOST_FAIL( "Can't create snapshot directory in test environment" );
    }


    InfoDirWithSnapshotDir::InfoDirWithSnapshotDir(const string& infos_dir_loc)
	: InfoDirectory(infos_dir_loc)
    {
	init();
    }


    InfoDirWithInvalidSnapshotDir::InfoDirWithInvalidSnapshotDir(const string& infos_dir_loc, unsigned int num)
	: InfoDirectory(infos_dir_loc, num)
    {
	init();
    }


    void
    InfoDirWithInvalidSnapshotDir::init()
    {
	std::cout << "CreateSnapshotEnvironmentFailure ctor" << std::endl;

	int fd = openat(f_dirfd, "snapshot", O_WRONLY | O_CREAT | O_EXCL | O_CLOEXEC, 0755);
	if (fd < 0)
	    BOOST_FAIL( "Can't create 'snapshot' file");

	close(fd);
    }


    InfoDirWithInvalidSnapshotDir::InfoDirWithInvalidSnapshotDir(const string& infos_dir_loc)
	: InfoDirectory(infos_dir_loc)
    {
	init();
    }


    InfoDirWithSnapshotDir::InfoDirWithSnapshotDir(const string& infos_dir_loc, unsigned int num)
	: InfoDirectory(infos_dir_loc, num)
    {
	init();
    }


    namespace lvm
    {

	const string LvmGeneralFixture::f_conf_lvm_root_volume = "/testsuite-import";
	const string LvmGeneralFixture::f_conf_lvm_snapshots_prefix = LvmGeneralFixture::f_conf_lvm_root_volume + "/" + ".snapshots";
	const string LvmGeneralFixture::f_conf_lvm_vg_name = "vg_test";
	const string LvmGeneralFixture::f_conf_lvm_foreign_vg_name = "vg_test_2";
	const string LvmGeneralFixture::f_conf_lvm_origin_lv_name = "lv_test_thin_1";
	const string LvmGeneralFixture::f_conf_lvm_foreign_origin_lv_name = "lv_test_thin_2";

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
