#include <dirent.h>
#include <stdio.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stddef.h>

#include <iostream>
#include <sstream>
#include <map>

#include <boost/test/unit_test.hpp>

#include "testsuite-import/lvm-import-fixtures.h"
#include "testsuite-import/helpers.h"

namespace lvmimporttest
{
    using std::cout;
    using std::cerr;
//    using std::map;
    using std::make_pair;

    const string GeneralFixture::f_snapshots_prefix =  "/testsuite-import/.snapshots/";

    LvmGlobalConfig::LvmGlobalConfig()
    {
	snapper::Snapper::createConfig("testsuite-import", "/testsuite-import", "lvm(ext4)", "default");
    }

    LvmGlobalConfig::~LvmGlobalConfig()
    {
	snapper::Snapper::deleteConfig("testsuite-import");
    }

    GeneralFixture::GeneralFixture()
    {
	BOOST_TEST_MESSAGE( "GeneralFixture ctor" );

	sh = new snapper::Snapper("testsuite-import");
	lvm = static_cast<const snapper::Lvm *>(sh->getFilesystem());
    }

    GeneralFixture::~GeneralFixture()
    {
	BOOST_TEST_MESSAGE( "GeneralFixture dtor" );

	delete sh;
    }

    ValidMetadata::ValidMetadata() : GeneralFixture()
    {
	raw_data.insert(make_pair("vg_name", "some_string"));
	raw_data.insert(make_pair("lv_name", "some_string"));
	raw_data.insert(make_pair("the_answer", "42"));
    }

    MissingVgName::MissingVgName() : GeneralFixture()
    {
	raw_data.insert(make_pair("lv_name", "some_string"));
	raw_data.insert(make_pair("the_answer", "42"));
    }

    MissingLvName::MissingLvName() : GeneralFixture()
    {
	raw_data.insert(make_pair("vg_name", "some_string"));
	raw_data.insert(make_pair("the_answer", "42"));
    }

    CreateSnapshotEnvironment::CreateSnapshotEnvironment()
	: GeneralFixture(), num(0)
    {
	std::cout << "CreateSnapshotEnvironment ctor" << std::endl;

	std::ostringstream oss;
	oss << f_snapshots_prefix << num;

	while (mkdir(oss.str().c_str(), 0755) && num < 100)
	{
	    num++;

	    oss.str(f_snapshots_prefix);
	    oss.clear();
	    oss << f_snapshots_prefix << num;

	    if (errno == EEXIST)
		continue;

	    BOOST_FAIL( "Couldn't create the info directory" );
	}

	if (num >= 100)
	    BOOST_FAIL( "Something went terribly wrong" );

	snapshot_dir = oss.str();

	dirfd = open(oss.str().c_str(), O_RDONLY | O_CLOEXEC | O_NOFOLLOW);

	if (dirfd < 0)
	    BOOST_FAIL( "Can't open: " << oss );

	struct stat buff;

	if (fstat(dirfd, &buff) || !S_ISDIR(buff.st_mode))
	{
	    close(dirfd);
	    BOOST_FAIL( "Can't stat dir: /testsuite-import/.snapshots/" << num << " or the d-entry is not a directory" );
	}
    }

    CreateSnapshotEnvironment::~CreateSnapshotEnvironment()
    {
	DIR *dr = fdopendir(dirfd);
	if (!dr)
	{
	    BOOST_TEST_MESSAGE( "Can't open directory stream" );
	    close(dirfd);
	    return;
	}

	long name_max = fpathconf(dirfd, _PC_NAME_MAX);
	if (name_max < 0)
	    name_max = 255;

	size_t len = offsetof(struct dirent, d_name) + static_cast<size_t>(name_max) + 1;
	struct dirent *de = static_cast<struct dirent *>(malloc(len));
	struct dirent *dres;

	while (!readdir_r(dr, de, &dres) && dres != NULL)
	{
	    if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0)
	    {
		std::cout << "going to remove: " << snapshot_dir << "/" << de->d_name << std::endl;

		struct stat buff;

		if (fstatat(dirfd, de->d_name, &buff, AT_SYMLINK_NOFOLLOW))
		{
		    perror("fstatat");
		    BOOST_TEST_MESSAGE( "Can't fstatat: " << snapshot_dir << de->d_name );
		}

		if (S_ISDIR(buff.st_mode))
		{
		    if (unlinkat(dirfd, de->d_name, AT_REMOVEDIR))
		    {
			perror("unlinkat");
			BOOST_TEST_MESSAGE( "Can't remove (dir): " << snapshot_dir << de->d_name );
		    }
		    continue;
		}

		if (S_ISREG(buff.st_mode))
		{
		    if (unlinkat(dirfd, de->d_name, 0))
		    {
			perror("unlinkat");
			BOOST_TEST_MESSAGE( "Can't remove: " << snapshot_dir << de->d_name );
		    }
		    continue;
		}
		BOOST_TEST_MESSAGE( "WARN: what file type is this? : " << snapshot_dir << de->d_name );
	    }
	}

	closedir(dr);
	free(de);
	close(dirfd);

	if (rmdir(snapshot_dir.c_str()))
	{
	    perror("rmdir");
	    BOOST_TEST_MESSAGE( "Can't remove: " << snapshot_dir );
	}

    }

    CreateSnapshotEnvironmentDirExists::CreateSnapshotEnvironmentDirExists()
	: CreateSnapshotEnvironment()
    {
	std::cout << "CreateSnapshotEnvironmentDirExists ctor" << std::endl;

	if (mkdirat(dirfd, "snapshot", 0755))
	    BOOST_FAIL( "Can't create snapshot directory in test environment" );
    }

    CreateSnapshotEnvironmentFailure::CreateSnapshotEnvironmentFailure()
	: CreateSnapshotEnvironment()
    {
	std::cout << "CreateSnapshotEnvironmentFailure ctor" << std::endl;

	int fd;

	fd = openat(dirfd, "snapshot", O_WRONLY | O_CREAT | O_EXCL | O_CLOEXEC, 0755);
	if (fd < 0)
	    BOOST_FAIL( "Can't create 'snapshot' file");

	close(fd);
    }

    CloneSnapshotValid::CloneSnapshotValid()
	: CreateSnapshotEnvironment(), f_vg_name("vg_test"), f_lv_name("lv_test_snapshot_01"),
	f_origin_name("lv_test_thin_1")  
    {
	std::cout << "CloneSnapshotValid ctor" << std::endl;

	// let boost handle the exception
	lvcreate_thin_snapshot_wrapper( f_vg_name, f_origin_name, f_lv_name );
    }

// TEST_VG_NAME=vg_test
// TEST_POOL_NAME=lv_test_pool
// TEST_THIN_LV_NAME=lv_test_thin
// TEST_VOLUME=/testsuite-import
// TEST_SNAP_01=lv_test_snapshot_01

    CloneSnapshotValid::~CloneSnapshotValid()
    {
	try {
	    lvremove_wrapper(f_vg_name, f_lv_name );
	}
	catch (const LvmImportTestsuiteException &e)
	{
	    std::cerr << "lvremove_wrapper( " << f_vg_name << ", " << f_lv_name << " ) failed" << std::endl;
	}

	try {
	    lvremove_wrapper(f_vg_name, lvm->snapshotLvName(num));
	}
	catch (const LvmImportTestsuiteException &e)
	{
	    std::cerr << "lvremove_wrapper( " << f_vg_name << ", " << lvm->snapshotLvName(num) << " ) failed" << std::endl;
	}
    }

    CloneSnapshotMissingOrigin::CloneSnapshotMissingOrigin()
	: f_vg_name("vg_test"), f_lv_name("lv_missing_snapshot_name")
    {
	std::cout << "CloneSnapshotMissingOrigin ctor" << std::endl;
    }

    MountSnapshotByDeviceValid::MountSnapshotByDeviceValid()
	: CreateSnapshotEnvironmentDirExists(), f_vg_name("vg_test"),
	  f_lv_name("lv_test_snapshot_01"), f_origin_name("lv_test_thin_1")
    {
	std::cout << "MountSnapshotByDeviceValid ctor" << std::endl;

	// let boost handle the exception
	lvcreate_thin_snapshot_wrapper( f_vg_name, f_origin_name, f_lv_name );

	std::ostringstream oss;
	oss << "/testsuite-import/.snapshots/" << num << "/snapshot";

	dev_path = "/dev/mapper/" + f_vg_name + "-" + f_lv_name;
	mountpoint = oss.str();
    }

    MountSnapshotByDeviceValid::~MountSnapshotByDeviceValid()
    {
	if (umount2(mountpoint.c_str(), MNT_DETACH))
	    std::cerr << "umount2( \"" << mountpoint << "\", MNT_DETACH) failed!" << std::endl;

	try
	{
	    lvremove_wrapper(f_vg_name, f_lv_name );
	}
	catch (const LvmImportTestsuiteException &e)
	{
	    std::cerr << "lvremove_wrapper( " << f_vg_name << ", " << f_lv_name << " ) failed" << std::endl;
	}
    }

    MountSnapshotByDeviceAlreadyMounted::MountSnapshotByDeviceAlreadyMounted()
	: MountSnapshotByDeviceValid()
    {
	std::cout << "MountSnapshotByDeviceAlreadyMounted ctor" << std::endl;

	int ret = mount(dev_path.c_str(), mountpoint.c_str(),
			lvm->mount_type.c_str(),
			MS_NOATIME | MS_NODEV | MS_NOEXEC | MS_RDONLY,
			NULL);

	if (ret)
	{
	    perror("mount");
	    BOOST_FAIL( "Can't mount filesystem for testing purposes: \"" <<
			dev_path << "\" -> \"" << mountpoint << "\"");
	}
    }

    MountSnapshotByDeviceInvalidDevice::MountSnapshotByDeviceInvalidDevice()
	: CreateSnapshotEnvironmentDirExists()
    {
	std::cout << "MountSnapshotByDeviceInvalidDevice ctor" << std::endl;

	missing_dev_path = "/dev/mapper/this_device_do_not_exists";
    }

    CheckImportedSnapshotValid::CheckImportedSnapshotValid()
	: GeneralFixture(), f_vg_name("vg_test"), f_lv_name("lv_test_snapshot_01"),
	  f_origin_name("lv_test_thin_1")
    {
	std::cout << "CheckImportedSnapshotValid ctor" << std::endl;

	// let boost handle the exception
	lvcreate_thin_snapshot_wrapper( f_vg_name, f_origin_name, f_lv_name );
    }

    CheckImportedSnapshotValid::~CheckImportedSnapshotValid()
    {
	try {
	    lvremove_wrapper(f_vg_name, f_lv_name );
	}
	catch (const LvmImportTestsuiteException &e)
	{
	    std::cerr << "lvremove_wrapper( " << f_vg_name << ", " << f_lv_name << " ) failed" << std::endl;
	}
    }

    CheckImportedSnapshotWrongVg::CheckImportedSnapshotWrongVg()
	: GeneralFixture(), f_vg_name("vg_test_2"), f_lv_name("lv_test_thin_2")
    {
	std::cout << "CheckImportedSnapshotWrongVg ctor" << std::endl;
    }

    CheckImportedSnapshotVolumeImport::CheckImportedSnapshotVolumeImport()
	: GeneralFixture(), f_vg_name(lvm->vg_name), f_lv_name(lvm->lv_name)
    {
	std::cout << "CheckImportedSnapshotVolumeImport ctor" << std::endl;
    }

    CheckImportedSnapshotFsUuidMismatch::CheckImportedSnapshotFsUuidMismatch()
	: GeneralFixture(), f_vg_name("vg_test"), f_lv_name("lv_test_snapshot_01"),
	  f_origin_name("lv_test_thin_1")
    {
	std::cout << "CheckImportedSnapshotFsUuidMismatch ctor" << std::endl;

	lvcreate_thin_snapshot_wrapper( f_vg_name, f_origin_name, f_lv_name, false);

	modify_fs_uuid(f_vg_name, f_lv_name, lvm->mount_type);
    }
}
