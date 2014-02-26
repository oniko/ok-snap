/*
 * Copyright (c) [2016] Red Hat, Inc.
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


#include <map>

#include <boost/algorithm/string.hpp>

#include "snapper/AsciiFile.h"
#include "snapper/Log.h"
#include "snapper/Selinux.h"

namespace snapper
{

    SnapperContexts::SnapperContexts()
	: subvolume_ctx(NULL)
    {
	std::map<string,string> snapperd_contexts;

	try
	{
	    AsciiFileReader asciifile(selinux_snapperd_contexts_path());

	    string line;
	    while (asciifile.getline(line))
	    {
		// commented line
		if (line[0] == '#')
		    continue;

		// do not parse lines w/o '=' symbol
		string::size_type pos = line.find('=');
		if (pos == string::npos)
		    continue;

		if (!snapperd_contexts.insert(make_pair(boost::trim_copy(line.substr(0, pos)), boost::trim_copy(line.substr(pos + 1)))).second)
		{
		    y2err("Duplicate key found in snapperd contexts file");
		    throw SelinuxException();
		}
	    }
	}
	catch (const FileNotFoundException& e)
	{
	    y2err("Failed to parse snapperd contexts file");
	    throw SelinuxException();
	}

	std::map<string,string>::const_iterator cit = snapperd_contexts.find(selinux_snapperd_data);
	if (cit == snapperd_contexts.end())
	{
	    y2err("Snapperd data context not found");
	    throw SelinuxException();
	}

	subvolume_ctx = context_new(cit->second.c_str());
	if (!subvolume_ctx)
	{
	    throw SelinuxException();
	}
    }

}
