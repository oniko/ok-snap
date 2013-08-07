#ifndef TESTSUITE_IMPORT_LVM_HELPERS_H
#define TESTSUITE_IMPORT_LVM_HELPERS_H

#include <exception>
#include <string>
#include <vector>

#include <boost/noncopyable.hpp>

namespace testsuiteimport
{
    using std::string;
    using std::vector;

    struct ImportTestsuiteException : public std::exception
    {
	explicit ImportTestsuiteException() throw() {}
	virtual const char* what() const throw() { return "generic testsuite import exception"; }
    };

    struct SimpleSystemCmdException : public ImportTestsuiteException
    {
	explicit SimpleSystemCmdException() throw() {}
	virtual const char* what() const throw() { return "SimpleSystemCmd constructor failure"; }
    };

    class SimpleSystemCmd : private boost::noncopyable
    {
    private:
	int ret_code;

	vector<string> out_vec;
	vector<string> err_vec;

    public:

	static char* convert(const string& str);

	SimpleSystemCmd(const string& cmd, const vector<string>& args);
	~SimpleSystemCmd() {}

	int retcode() const { return ret_code; }

	vector<string>::const_iterator stdout_cbegin() const { return out_vec.begin(); }
	vector<string>::const_iterator stdout_cend() const { return out_vec.end(); }

	vector<string>::const_iterator stderr_cbegin() const { return err_vec.begin(); }
	vector<string>::const_iterator stderr_cend() const { return err_vec.end(); }
    };

    void deep_rmdirat(int fd);
    void deep_rmdirat(const string& path);


    namespace lvm
    {

	bool check_lv_exists(const string& vg_name, const string& lv_name);
	bool check_is_thin(const string& vg_name, const string& lv_name);

	bool check_is_mounted(const string& vg_name, const string& lv_name);

	void lvcreate_thin_snapshot_wrapper(const string& vg_name, const string& origin_lv_name, const string& snapshot_name, bool readonly = true);
	void lvcreate_non_thin_lv_wrapper(const string& vg_name, const string& lv_name);
	void lvremove_wrapper(const string& vg_name, const string& lv_name);

	void modify_fs_uuid(const string& vg_name, const string& lv_name, const string& fs_type, const string& new_uuid = "");

	void change_permission(const string& vg_name, const string& lv_name, bool ro);

	struct LvmImportTestsuiteException : public ImportTestsuiteException
	{
	    explicit LvmImportTestsuiteException() throw() {}
	    virtual const char* what() const throw() { return "generic testsuite import lvm exception"; }
	};
    }

    namespace btrfs
    {
	void btrfs_create_subvolume(const string& root, const string& subvolume);
	void btrfs_delete_subvolume(const string& parent_dir, const string& name);

	void btrfs_create_snapshot_ro(const string& source, const string& dest_path, const string& name);

	bool btrfs_subvolume_exists(const string& path);

	string deep_mkdirat(const string& root, const string& new_dirs, bool fail_at_eexists = false);
	bool bool_deep_mkdirat(const string& root, const string& new_dirs);
    }
}
#endif // TESTSUITE_IMPORT_LVM_HELPERS_H
