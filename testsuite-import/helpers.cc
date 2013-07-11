#include <errno.h>
#include <dirent.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <asm/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <iostream>
#include <list>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>


#include <boost/algorithm/string.hpp>

#include "testsuite-import/helpers.h"

#ifdef HAVE_LIBBTRFS
#include <btrfs/ioctl.h>
#else
#define BTRFS_IOCTL_MAGIC 0x94
#define BTRFS_PATH_NAME_MAX 4087
#define BTRFS_SUBVOL_NAME_MAX 4039
#define BTRFS_SUBVOL_RDONLY (1ULL << 1)

#define BTRFS_IOC_SNAP_CREATE _IOW(BTRFS_IOCTL_MAGIC, 1, struct btrfs_ioctl_vol_args)
#define BTRFS_IOC_SUBVOL_CREATE _IOW(BTRFS_IOCTL_MAGIC, 14, struct btrfs_ioctl_vol_args)
#define BTRFS_IOC_SNAP_DESTROY _IOW(BTRFS_IOCTL_MAGIC, 15, struct btrfs_ioctl_vol_args)
#define BTRFS_IOC_SNAP_CREATE_V2 _IOW(BTRFS_IOCTL_MAGIC, 23, struct btrfs_ioctl_vol_args_v2)

struct btrfs_ioctl_vol_args
{
    __s64 fd;
    char name[BTRFS_PATH_NAME_MAX + 1];
};

struct btrfs_ioctl_vol_args_v2
{
    __s64 fd;
    __u64 transid;
    __u64 flags;
    __u64 unused[4];
    char name[BTRFS_SUBVOL_NAME_MAX + 1];
};

#endif //HAVE_LIBBTRFS

namespace testsuiteimport
{
    using std::string;
    using std::vector;

    namespace lvm
    {

	bool check_lv_exists(const string& vg_name, const string& lv_name)
	{
	    vector<string> args;
	    args.push_back(vg_name + "/" + lv_name);

	    try
	    {
		SimpleSystemCmd cmd("/usr/sbin/lvs", args);

		return cmd.retcode() ? false : true;
	    }
	    catch (const SimpleSystemCmdException &e)
	    {
		std::cerr << "SimpleSystemCmd(\"/usr/sbin/lvs\") failed" << std::endl;
		throw;
	    }
	}

	bool check_is_thin(const string& vg_name, const string& lv_name)
	{
	    string tmp("-o segtype --noheadings " + vg_name + "/" + lv_name);

	    vector<string> args;
	    boost::split(args, tmp, boost::is_any_of(" \t\n"), boost::token_compress_on);

	    try
	    {
		SimpleSystemCmd cmd("/usr/sbin/lvs", args);

		if (cmd.retcode())
		{
		    std::cerr << "lvs failed with ret_code: " << cmd.retcode() << std::endl;
		    for (vector<string>::const_iterator cit = cmd.stderr_cbegin(); cit != cmd.stderr_cend(); cit++)
			std::cerr << "lvs err: " << *cit << std::endl;
		    throw LvmImportTestsuiteException();
		}

		assert(cmd.stdout_cbegin() != cmd.stdout_cend());

		string segtype = boost::trim_copy(*cmd.stdout_cbegin());

		return segtype.compare("thin") ? false : true;
	    }
	    catch (const SimpleSystemCmdException &e)
	    {
		std::cerr << "SimpleSystemCmd(\"/usr/sbin/lvs\") failed" << std::endl;
		throw;
	    }
	}

	bool check_is_mounted(const string& vg_name, const string& lv_name)
	{
	    vector<string> args;
	    args.push_back("/dev/mapper/" +
			boost::replace_all_copy(vg_name, "-", "--") +
			"-" +
			boost::replace_all_copy(lv_name, "-", "--"));

	    try
	    {
		SimpleSystemCmd cmd("/usr/bin/findmnt", args);

		return cmd.retcode() ? false : true;
	    }
	    catch (const SimpleSystemCmdException &e)
	    {
		std::cerr << "SimpleSystemCmd(\"/usr/bin/findmnt\", " << args.front() << ") failed" << std::endl;
		throw;
	    }
	}

	void lvcreate_thin_snapshot_wrapper(const string& vg_name, const string& origin_lv_name, const string& snapshot_name, bool readonly)
	{
	    std::ostringstream oss;
	    oss << "-qq ";

	    if (readonly)
	    {
		oss << "--permission r ";
	    }

	    oss << "--snapshot --name " << snapshot_name << " " << vg_name
		<< "/" + origin_lv_name;

	    string tmp(oss.str());
	    vector<string> args;
	    boost::split(args, tmp, boost::is_any_of(" \t\n"), boost::token_compress_on);

	    try
	    {
		SimpleSystemCmd cmd("/usr/sbin/lvcreate", args);

		if (cmd.retcode())
		{
		    std::cerr << "lvcreate failed with ret_code: " << cmd.retcode() << std::endl;
		    for (vector<string>::const_iterator cit = cmd.stderr_cbegin(); cit != cmd.stderr_cend(); cit++)
			std::cerr << "lvcreate err: " << *cit << std::endl;
		    throw LvmImportTestsuiteException();
		}
	    }
	    catch (const SimpleSystemCmdException &e)
	    {
		std::cerr << "SimpleSystemCmd(\"/usr/sbin/lvcreate\") failed" << std::endl;
		throw;
	    }
	}

	void lvcreate_non_thin_lv_wrapper(const string& vg_name, const string& lv_name)
	{
	    string tmp("-qq --name " + lv_name + " -L100M " + vg_name);

	    vector<string> args;
	    boost::split(args, tmp, boost::is_any_of(" \t\n"), boost::token_compress_on);

	    try
	    {
		SimpleSystemCmd cmd("/usr/sbin/lvcreate", args);

		if (cmd.retcode())
		{
		    std::cerr << "lvcreate failed with ret_code: " << cmd.retcode() << std::endl;
		    for (vector<string>::const_iterator cit = cmd.stderr_cbegin(); cit != cmd.stderr_cend(); cit++)
			std::cerr << "lvcreate err: " << *cit << std::endl;
		    throw LvmImportTestsuiteException();
		}
	    }
	    catch (const SimpleSystemCmdException &e)
	    {
		std::cerr << "SimpleSystemCmd(\"/usr/sbin/lvcreate\", \"" << tmp
			<< "\") failed" << std::endl;
		throw;
	    }
	}

	void lvremove_wrapper(const string& vg_name, const string& lv_name)
	{
	    string tmp("-qq -f " + vg_name + "/" + lv_name);

	    vector<string> args;
	    boost::split(args, tmp, boost::is_any_of(" \t\n"), boost::token_compress_on);

	    try
	    {
		SimpleSystemCmd cmd("/usr/sbin/lvremove", args);

		if (cmd.retcode())
		{
		    std::cerr << "lvremove failed with ret_code: " << cmd.retcode() << std::endl;
		    for (vector<string>::const_iterator cit = cmd.stderr_cbegin(); cit != cmd.stderr_cend(); cit++)
			std::cerr << "lvremove err: " << *cit << std::endl;
		    throw LvmImportTestsuiteException();
		}
	    }
	    catch (const SimpleSystemCmdException &e)
	    {
		std::cerr << "SimpleSystemCmd(\"/usr/sbin/lvremove\") failed" << std::endl;
		throw;
	    }
	}

	void modify_fs_uuid(const string& vg_name, const string& lv_name, const string& fs_type, const string& new_uuid)
	{
	    // FIXME: proper fs_type check with decision which tune2fs to use
	    if (fs_type.substr(0, 3) != "ext")
	    {
		std::cerr << "unimplemented fs" << std::endl;
		throw std::exception();
	    }

	    string tmp("-U " + (new_uuid.empty() ? "random" : new_uuid) + " /dev/" + vg_name + "/" + lv_name);

	    vector<string> args;
	    boost::split(args, tmp, boost::is_any_of(" \t\n"), boost::token_compress_on);

	    try
	    {
		SimpleSystemCmd cmd("/usr/sbin/tune2fs", args);

		if (cmd.retcode())
		{
		    std::cerr << "tune2fs failed with ret_code: " << cmd.retcode() << std::endl
			    << "/usr/sbin/tune2fs " << tmp << std::endl;
		    for (vector<string>::const_iterator cit = cmd.stderr_cbegin(); cit != cmd.stderr_cend(); cit++)
			std::cerr << "tune2fs err: " << *cit << std::endl;
		    throw LvmImportTestsuiteException();
		}
	    }
	    catch (const SimpleSystemCmdException &e)
	    {
		std::cerr << "SimpleSystemCmd(\"/usr/sbin/tun2fs\", \"" << tmp << "\") failed" << std::endl;
		throw;
	    }
	}
    } // lvm


    namespace btrfs {
	void btrfs_create_subvolume(const string& root, const string& subvolume)
	{
	    std::cout << "btrfs_create_subvolume(" << root << ", " << subvolume << ")" << std::endl;

	    int fddst = open(root.c_str(), O_RDONLY | O_CLOEXEC | O_NOATIME );
	    if (fddst < 0)
	    {
		std::cerr << "failed to open: " << root << std::endl;
		throw std::exception();
	    }

	    struct btrfs_ioctl_vol_args args;
	    memset(&args, 0, sizeof(args));

	    strncpy(args.name, subvolume.c_str(), sizeof(args.name) - 1);

	    int ret = ioctl(fddst, BTRFS_IOC_SUBVOL_CREATE, &args);
	    close(fddst);

	    if (ret)
	    {
		throw std::exception();
	    }
	}


	void btrfs_delete_subvolume(const string& parent_dir, const string& name)
	{
	    int fd = open(parent_dir.c_str(), O_RDONLY | O_CLOEXEC | O_NOATIME);
	    if (fd < 0)
	    {
		std::cerr << "can't open: " << parent_dir << std::endl;
		throw std::exception();
	    }

	    struct btrfs_ioctl_vol_args args;
	    memset(&args, 0, sizeof(args));

	    strncpy(args.name, name.c_str(), sizeof(args.name) - 1);

	    int ret = ioctl(fd, BTRFS_IOC_SNAP_DESTROY, &args);

	    close(fd);

	    if (ret)
	    {
		std::cerr << "ioctl(" << name << ", BTRFS_IOC_SNAP_DESTROY) failed" << std::endl;
		throw std::exception();
	    }
	}

	void btrfs_create_snapshot_ro(const string& source, const string& dest_path, const string& name)
	{
	    int fddst = open(dest_path.c_str(), O_RDONLY | O_CLOEXEC | O_NOATIME);

	    if (fddst < 0 )
	    {
		throw std::exception();
	    }
	    
	    int fd = open(source.c_str(), O_RDONLY | O_CLOEXEC | O_NOATIME);
	    if (fd < 0)
	    {
		close(fddst);
		throw std::exception();
	    }

	    struct btrfs_ioctl_vol_args_v2 args_v2;
	    memset(&args_v2, 0, sizeof(args_v2));

	    args_v2.fd = fd;
	    args_v2.flags = BTRFS_SUBVOL_RDONLY;
	    strncpy(args_v2.name, name.c_str(), sizeof(args_v2.name) - 1);

	    int ret = ioctl(fddst, BTRFS_IOC_SNAP_CREATE_V2, &args_v2);

	    if (ret && (errno == ENOTTY || errno == EINVAL))
	    {
		struct btrfs_ioctl_vol_args args;
		memset(&args, 0, sizeof(args));

		args.fd = fd;
		strncpy(args.name, name.c_str(), sizeof(args.name) - 1);

		ret = ioctl(fddst, BTRFS_IOC_SNAP_CREATE, &args);
	    }

	    close(fd);
	    close(fddst);

	    if (ret)
	    {
		throw std::exception();
	    }
	}

	bool btrfs_subvolume_exists(const string& path)
	{
	    struct stat buf;

	    int fd = open(path.c_str(), O_RDONLY | O_NOATIME | O_CLOEXEC | O_NOFOLLOW);
	    if (fd < 0)
		return false;

	    int ret = fstat(fd, &buf);
	    close(fd);

	    return ret ? false : S_ISDIR(buf.st_mode) && buf.st_ino == 256;
	}

	string deep_mkdirat(const string& root, const string& new_dirs, bool fail_at_eexists)
	{
	    if (new_dirs.empty())
	    {
		std::cerr << "empty new_dirs" << std::endl;
		throw std::exception();
	    }

	    int fd;

	    int rootfd = open(root.c_str(), O_RDONLY | O_NOATIME | O_CLOEXEC);
	    if (rootfd < 0)
	    {
		std::cerr << "can't open root: " << root << std::endl;
		throw std::exception();
	    }

	    std::cout << "new_dirs: " << new_dirs << std::endl;

	    vector<string> res;
	    boost::split(res, new_dirs, boost::is_any_of("/"), boost::token_compress_on);

	    for (vector<string>::const_iterator cit = res.begin();
		 cit != res.end();
		 cit++, close(rootfd), rootfd = fd)
	    {
		struct stat buf;
		
		std::cout << "going to mkdirat: " << *cit << std::endl;

		fd = ::openat(rootfd, (*cit).c_str(), O_RDONLY | O_NOATIME | O_CLOEXEC);
		if (fd >= 0)
		{
		    if (!::fstat(fd, &buf) && S_ISDIR(buf.st_mode) && !fail_at_eexists)
			continue;

		    close(rootfd);
		    close(fd);

		    std::cerr << *cit << " is not a dir or can't stat it" << std::endl;

		    throw std::exception();
		}

		if(::mkdirat(rootfd, (*cit).c_str(), 0777))
		{
		    // even in case of EEXIST, someone has outrunned us, throw exception
		    close(rootfd);
		    std::cerr << "mkdir " << *cit << " failed!" << std::endl;
		    throw std::exception();
		}

		fd = ::openat(rootfd, (*cit).c_str(), O_RDONLY | O_NOATIME | O_CLOEXEC);
		if (fd < 0)
		{
		    close(rootfd);
		    std::cerr << "last open " << *cit << " failed!" << std::endl;
		    throw std::exception();
		}
	    }

	    close(rootfd);

	    std::cout << "returing " << root << '/' << new_dirs << std::endl;

	    return new_dirs;
	}


	bool
	bool_deep_mkdirat(const string& root, const string& new_dirs)
	{
	    try
	    {
		deep_mkdirat(root, new_dirs, true);
		return true;
	    }
	    catch (...)
	    {
		return false;
	    }
	}

    } // btrfs

    char* SimpleSystemCmd::convert(const string& str)
    {
	char *tmp = new char[str.size() + 1];
	std::strcpy(tmp, str.c_str());
	return tmp;
    }

    SimpleSystemCmd::SimpleSystemCmd(const string& cmd, const vector<string>& args)
    {
	int out_pipefd[2], err_pipefd[2];

	if (pipe(out_pipefd) < 0 || pipe(err_pipefd) < 0)
	{
	    std::cerr << "pipe() failed" << std::endl;
	    throw std::exception();
	}

	// child will not need read part of pipe
	if (fcntl(out_pipefd[0], F_SETFD, FD_CLOEXEC) == -1 || fcntl(err_pipefd[0], F_SETFD, FD_CLOEXEC) == -1)
	{
	    std::cerr << "couldn't set FD_CLOEXEC on pipe[0]" << std::endl;
	    throw SimpleSystemCmdException();
	}

	pid_t cpid = fork();
	if (cpid)
	{
	    // parent
	    if (cpid < 0)
	    {
		std::cerr << "fork() failed" << std::endl;
		throw SimpleSystemCmdException();
	    }

	    // close write input
	    if (close(out_pipefd[1]) || close(err_pipefd[1]))
	    {
		std::cerr << "parent: can't close write input of pipe"
			  << std::endl << "going to kill my child: "
			  << cpid << std::endl;
		kill(cpid, SIGKILL);
		throw SimpleSystemCmdException();
	    }

	    FILE* outfile = fdopen(out_pipefd[0], "r");
	    FILE* errfile = fdopen(err_pipefd[0], "r");

	    if (!outfile || !errfile)
	    {
		std::cerr << "parent: can't open fopen() pipe for reading" << std::endl;
		kill(cpid, SIGKILL);
		throw SimpleSystemCmdException();
	    }

	    char *lineptr = NULL;
	    ssize_t n = 0;
	    ssize_t len;

	    while ((len = getline(&lineptr, (size_t *)&n, outfile)) > 0)
	    {
		if (n < (len + 1))
		    n = len + 1;
		out_vec.push_back(string(lineptr, len - 1));
	    }

	    while ((len = getline(&lineptr, (size_t *)&n, errfile)) > 0)
	    {
		if (n < (len + 1))
		    n = len + 1;
		err_vec.push_back(string(lineptr, len - 1));
	    }

	    fclose(outfile);
	    fclose(errfile);

	    free(lineptr);

	    int status;
	    waitpid(cpid, &status, 0);
	    ret_code = WIFEXITED(status) ? WEXITSTATUS(status) : -1;

	    close(out_pipefd[0]);
	    close(err_pipefd[0]);
	}
	else // child
	{
	    setenv( "LC_ALL", "C", 1 );
	    setenv( "LANGUAGE", "C", 1 );

	    close(STDIN_FILENO);

	    if (dup2(out_pipefd[1], STDOUT_FILENO) == -1 || dup2(err_pipefd[1], STDERR_FILENO) == -1)
	    {
		std::cerr << "child: couldn't dup pipe to stdout or stderr" << std::endl;
		exit(EXIT_FAILURE);
	    }

	    close(out_pipefd[1]);
	    close(err_pipefd[1]);

	    vector<char*> pchar_vec;

	    pchar_vec.push_back(convert(cmd));
	    std::transform(args.begin(), args.end(), std::back_inserter(pchar_vec), SimpleSystemCmd::convert);
	    pchar_vec.push_back(static_cast<char *>(NULL));

	    execv(cmd.c_str(), &pchar_vec.front());

	    for (vector<char *>::const_iterator cit = pchar_vec.begin(); cit != pchar_vec.end(); cit++)
		delete (*cit);

	    std::cerr << "execl failed!" << std::endl;
	    exit(EXIT_FAILURE);
	}
    }
    
    void
    deep_rmdirat(int dirfd)
    {
	DIR *dr = fdopendir(dirfd);
	if (!dr)
	{
	    std::cerr << "Can't opendir passed fd" << std::endl;
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
		struct stat buff;

		if (fstatat(dirfd, de->d_name, &buff, AT_SYMLINK_NOFOLLOW))
		{
// 			perror("fstatat");
// 			BOOST_TEST_MESSAGE( "Can't fstatat: " << f_snapshot_dir << de->d_name );
		    //throw std::exception();
		    std::cerr << "can't stat " << de->d_name << std::endl;
		}

		if (S_ISDIR(buff.st_mode))
		{
		    int fd = openat(dirfd, de->d_name, O_RDONLY | O_NOATIME | O_NOFOLLOW | O_CLOEXEC);
		    if (fd >= 0)
		    {
			deep_rmdirat(fd);
			close(fd);
		    }

		    if (unlinkat(dirfd, de->d_name, AT_REMOVEDIR))
		    {
			perror("unlinkat");
			std::cerr <<  "Can't remove (dir): " << de->d_name << std::endl;
		    }
		    continue;
		}

		if (S_ISREG(buff.st_mode) || S_ISLNK(buff.st_mode))
		{
		    if (unlinkat(dirfd, de->d_name, 0))
		    {
			perror("unlinkat");
			std::cerr << "Can't remove: " << de->d_name << std::endl;
		    }
		    continue;
		}
		std::cerr << "WARN: what file type is this? : " << de->d_name << std::endl;
	    }
	}

	closedir(dr);
	free(de);
    }

    void deep_rmdirat(const string& path)
    {
	int fd = open(path.c_str(), O_RDONLY | O_NOATIME | O_CLOEXEC | O_NOFOLLOW);
	if (fd >= 0)
	{
	    deep_rmdirat(fd);
	    close(fd);
	}
    }

}
