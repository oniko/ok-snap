#ifndef TESTSUITE_IMPORT_LVM_HELPERS_H
#define TESTSUITE_IMPORT_LVM_HELPERS_H

#include <exception>
#include <string>
#include <vector>

#include <boost/noncopyable.hpp>

namespace lvmimporttest
{
    using std::string;
    using std::vector;

    bool check_lv_exists(const string& vg_name, const string& lv_name);
    bool check_is_thin(const string& vg_name, const string& lv_name);

    bool check_is_mounted(const string& vg_name, const string& lv_name);

    void lvcreate_thin_snapshot_wrapper(const string& vg_name, const string& origin_lv_name, const string& snapshot_name, const bool readonly = true);
    void lvremove_wrapper(const string& vg_name, const string& lv_name);

    void modify_fs_uuid(const string& vg_name, const string& lv_name, const string& fs_type, const string& new_uuid = "");

    struct LvmImportTestsuiteException : std::exception
    {
	explicit LvmImportTestsuiteException() throw() {}
	virtual const char* what() const throw() { return "generic testsuite import lvm exception"; }
    };

    struct SimpleSystemCmdException : public LvmImportTestsuiteException
    {
	explicit SimpleSystemCmdException() throw() {}
	virtual const char* what() const throw() { return "Constructor failure"; }
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
}
#endif // TESTSUITE_IMPORT_LVM_HELPERS_H
