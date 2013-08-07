#ifndef GENERAL_FIXTURES_H
#define GENERAL_FIXTURES_H

#include <string>

#include <boost/noncopyable.hpp>

#include "snapper/Btrfs.h"
#include "snapper/Lvm.h"
#include "snapper/Snapper.h"

#include "snapper/Logger.h"

namespace testsuiteimport
{
    using std::string;


    class InfoDirectory : public boost::noncopyable {
    public:
	InfoDirectory(const string& infos_dir_loc);
	InfoDirectory(const string& infos_dir_loc, unsigned int num);
	~InfoDirectory();

	unsigned int f_num;
	string f_info_dir;

	unsigned int get_dirfd() const { return f_dirfd; }
    protected:
	int f_dirfd;
    private:
	void infodir_init();
	const string infos_dir;
    };


    class InfoDirWithSnapshotDir : public InfoDirectory {
    public:
	InfoDirWithSnapshotDir(const string& infos_dir_loc);
	InfoDirWithSnapshotDir(const string& infos_dir_loc, unsigned int num);
    private:
	void init();
    };


    class InfoDirWithInvalidSnapshotDir : public InfoDirectory {
    public:
	InfoDirWithInvalidSnapshotDir(const string& infos_dir_loc);
	InfoDirWithInvalidSnapshotDir(const string& infos_dir_loc, unsigned int num);
    private:
	void init();
    };


    class SubvolumeWrapper : public boost::noncopyable
    {
    public:
	SubvolumeWrapper(const InfoDirectory* info_dir) : p_info_dir(info_dir) {}
	virtual ~SubvolumeWrapper() { delete p_info_dir; }

	virtual bool is_mounted() const = 0;
	virtual bool exists() const = 0;

	virtual string fstype() const = 0;
	virtual string infos_dir() const = 0;
	virtual void umount() const = 0;

	unsigned int get_num() const { return p_info_dir ? p_info_dir->f_num : 42; }
    protected:
	const InfoDirectory* p_info_dir;
    };


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
	    static const string f_conf_lvm_vg_name;
	    static const string f_conf_lvm_foreign_vg_name;
	    static const string f_conf_lvm_origin_lv_name;
	    static const string f_conf_lvm_foreign_origin_lv_name;
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
