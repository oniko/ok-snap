/*
 * Copyright (c) [2004-2012] Novell, Inc.
 *
 * All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, contact Novell, Inc.
 *
 * To contact Novell about this file by physical or electronic mail, you may
 * find current contact information at www.novell.com.
 */


#ifndef SNAPPER_APP_UTIL_H
#define SNAPPER_APP_UTIL_H

#include <sys/time.h>
#include <sys/types.h>
#include <sstream>
#include <locale>
#include <string>
#include <list>
#include <map>
#include <vector>


namespace snapper
{
    using std::string;
    using std::list;
    using std::map;
    using std::vector;


    bool checkDir(const string& Path_Cv);

    list<string> glob(const string& path, int flags);

    bool clonefile(int src_fd, int dest_fd);
    bool copyfile(int src_fd, int dest_fd);

    int readlink(const string& path, string& buf);
    int symlink(const string& oldpath, const string& newpath);

    string realpath(const string& path);

    string stringerror(int errnum);

    string dirname(const string& name);
    string basename(const string& name);


    struct MtabData
    {
	string device;
	string dir;
	string type;
	vector<string> options;
    };

    bool getMtabData(const string& mount_point, bool& found, MtabData& mtab_data);


    template<class StreamType>
    void classic(StreamType& stream)
    {
	stream.imbue(std::locale::classic());
    }


    string hostname();

    string datetime(time_t time, bool utc, bool classic);
    time_t scan_datetime(const string& str, bool utc);

    string username(uid_t uid);


    class StopWatch
    {
    public:

	StopWatch();

	double read() const;

	friend std::ostream& operator<<(std::ostream& s, const StopWatch& sw);

    protected:

	struct timeval start_tv;

    };


    string sformat(const string& format, ...);

}

#endif
