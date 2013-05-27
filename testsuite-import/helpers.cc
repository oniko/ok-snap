#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>

#include <boost/algorithm/string.hpp>

#include "testsuite-import/helpers.h"

namespace testsuiteimport { namespace lvm

{
    using std::string;
    using std::vector;


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
	args.push_back("/dev/mapper/" + vg_name + "-" + lv_name);

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
}}
