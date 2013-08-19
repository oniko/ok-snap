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
#include <vector>

#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>


namespace snapper
{
    using std::map;
    using std::set;
    using std::string;
    using std::vector;

    class LvmCapabilities;

    typedef map<string, vector<string>> vg_content_raw;

    struct LvAttrs
    {
	static bool extractActive(const string& raw);
	static bool extractReadOnly(const string& raw);

	LvAttrs(const vector<string>& raw);
	LvAttrs() : active(false), readonly(false), thin(false) {}

	bool active;
	bool readonly;
	bool thin;
	string pool;
    };


    class VolGroup : boost::noncopyable
    {
    public:

	// store pointer: LvInfo can be modified
	typedef map<string, LvAttrs> vg_content_t;
	typedef vg_content_t::iterator iterator;
	typedef vg_content_t::const_iterator const_iterator;

	VolGroup(vg_content_raw& input, const LvmCapabilities* caps);

	bool active(const string& lv_name) const; // shared locking

	void activate(const string& vg_name, const string& lv_name); // upg lock -> excl
	void deactivate(const string& vg_name, const string& lv_name); // upg lock -> excl

	bool contains(const string& lv_name) const; // shared lock
	bool contains_thin(const string& lv_name) const; // shared lock

	bool read_only(const string& lv_name) const; // shared lock

	void add(const string& lv_name, const LvAttrs& attrs); // excl lock
	void add_update(const string& vg_name, const string& lv_name); // excl lock

	void remove(const string& lv_name); // excl lock
	void rename(const string& old_name, const string& new_name); // upg lock -> excl

    private:
	const LvmCapabilities* caps;

	mutable boost::shared_mutex vg_mutex;

	vg_content_t lv_info_map;
    };


    class LvmCache : public boost::noncopyable
    {
    public:
	static LvmCache* get_lvm_cache();

	~LvmCache();

	// storing pointers in case we will need locking (mutex is noncopyable)
	typedef map<string, VolGroup*>::const_iterator const_iterator;
	typedef map<string, VolGroup*>::iterator iterator;

	bool active(const string& vg_name, const string& lv_name) const;

	void activate(const string& vg_name, const string& lv_name) const;
	void deactivate(const string& vg_name, const string& lv_name) const;

	bool contains(const string& vg_name, const string& lv_name) const;
	bool contains_thin(const string& vg_name, const string& lv_name) const;
	bool read_only(const string& vg_name, const string& lv_name) const;

	// add snapshot owned by snapper
	void add(const string& vg_name, const string& lv_name) const;
	void add_update(const string& vg_name, const string& lv_name);
	// load all lvs in vg
	
	// remove snapshot owned by snapper
	void remove(const string& vg_name, const string& lv_name);

    private:
	LvmCache();

	bool find_vg(const string& vg_name, const_iterator &cit) const;
	void add(const string& vg_name, boost::upgrade_lock<boost::shared_mutex>& upg_lock);

	const LvmCapabilities* caps;
	map<string, VolGroup*> vgroups;
    };
}
#endif // SNAPPER_LVM_CACHE_H
