/*
 * Copyright (c) [2013] Red Hat, Inc.
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
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#ifndef SNAPPER_LVM_CACHE_H
#define SNAPPER_LVM_CACHE_H

#include <map>
#include <set>
#include <string>

#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>

namespace snapper
{
     using std::map;
     using std::set;
     using std::string;

    // FIXME: !!! this is utterly broken. Only for testing purposes !!!

    class VolGroup : boost::noncopyable
    {
    public:
	typedef set<string> vg_content_set;

	VolGroup(const vg_content_set& input) : lvs_in_vg(input) {}

	bool contains(const string& lv_name) const; // req. shared lock
	void add(const string& lv_name); // req. exclusive lock
	void remove(const string& lv_name); // req. exclusive lock

    private:
	// TODO: locking maybe useless
	boost::mutex _mutex;

	vg_content_set lvs_in_vg;
    };


    class LvmCache : public boost::noncopyable
    {
    public:
	~LvmCache();

	static LvmCache* get_lvm_cache();

	typedef map<string, VolGroup>::const_iterator const_iterator;
	typedef map<string, VolGroup>::iterator iterator;

	bool in_cache(const string& vg_name, const string& lv_name) const;
	void add(const string& vg_name, const string& lv_name);
	void add(const string& vg_name);
	void remove(const string& vg_name, const string& lv_name);

    private:
	LvmCache() {}

	void create_new_vg(const string& vg_name);

	map<string, VolGroup*> m_caches;
    };
}


#endif // SNAPPER_LVM_CACHE_H
