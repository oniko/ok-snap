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

#include "config.h"

#include <vector>

#include <boost/algorithm/string.hpp>

#include "snapper/Log.h"
#include "snapper/Lvm.h"
#include "snapper/LvmCache.h"
#include "snapper/SystemCmd.h"

boost::shared_mutex lvm_cache_mutex;

namespace snapper
{
    using std::make_pair;

    bool
    LvAttrs::extractActive(const string& raw)
    {
	return (raw.size() > 4 && raw[4] == 'a');
    }


    bool
    LvAttrs::extractReadOnly(const string& raw)
    {
	return (raw.size() > 1 && raw[1] == 'r');
    }


    LvAttrs::LvAttrs(const vector<string>& raw)
	: active(raw.size() > 1 && extractActive(raw.front())),
	readonly(raw.size() > 1 && extractReadOnly(raw.front())),
	thin(raw.size() > 2 && raw[1] == "thin"),
	pool(raw.size() > 3 ? raw[2] : "")
    {
    }


    VolGroup::VolGroup(vg_content_raw& input, const LvmCapabilities* caps)
	: caps(caps)
    {
	for (vg_content_raw::const_iterator cit = input.begin(); cit != input.end(); cit++)
	    lv_info_map.insert(make_pair(cit->first, LvAttrs(cit->second)));
    }


    bool
    VolGroup::active(const string& lv_name) const
    {
	boost::shared_lock<boost::shared_mutex> sh_lock(vg_mutex);

	const_iterator cit = lv_info_map.find(lv_name);

	return cit != lv_info_map.end() && cit->second.active;
    }


    void
    VolGroup::activate(const string& vg_name, const string& lv_name)
    {
	boost::upgrade_lock<boost::shared_mutex> upg_lock(vg_mutex);

	iterator it = lv_info_map.find(lv_name);
	if (it == lv_info_map.end())
	{
	    // TODO: proper exception
	    throw std::exception();
	}

	if (!it->second.active)
	{
	    boost::upgrade_to_unique_lock<boost::shared_mutex> unique_lock(upg_lock);

	    SystemCmd cmd(LVCHANGEBIN + caps->get_ignoreactivationskip() + " -ay " + quote(vg_name + "/" + lv_name));
	    if (cmd.retcode() != 0)
	    {
		y2err("Couldn't activate snapshot " << vg_name << "/" << lv_name);
		throw LvmActivationException();
	    }

	    it->second.active = true;
	}
    }


    void
    VolGroup::deactivate(const string& vg_name, const string& lv_name)
    {
	boost::upgrade_lock<boost::shared_mutex> upg_lock(vg_mutex);

	iterator it = lv_info_map.find(lv_name);
	if (it == lv_info_map.end())
	{
	    // TODO: proper exception
	    throw std::exception();
	}

	if (it->second.active)
	{
	    boost::upgrade_to_unique_lock<boost::shared_mutex> unique_lock(upg_lock);

	    SystemCmd cmd(LVCHANGEBIN " -an " + quote(vg_name + "/" + lv_name));
	    if (cmd.retcode() != 0)
	    {
		y2err("Couldn't activate snapshot " << vg_name << "/" << lv_name);
		throw LvmDeactivatationException();
	    }

	    it->second.active = false;
	}
    }


    bool
    VolGroup::contains(const std::string& lv_name) const
    {
	boost::shared_lock<boost::shared_mutex> lock(vg_mutex);

	return lv_info_map.find(lv_name) != lv_info_map.end();
    }


    bool
    VolGroup::contains_thin(const string& lv_name) const
    {
	boost::shared_lock<boost::shared_mutex> sh_lock(vg_mutex);

	const_iterator cit = lv_info_map.find(lv_name);

	return cit != lv_info_map.end() && cit->second.thin;
    }


    bool
    VolGroup::read_only(const string& lv_name) const
    {
	boost::shared_lock<boost::shared_mutex> sh_lock(vg_mutex);

	const_iterator cit = lv_info_map.find(lv_name);

	return cit != lv_info_map.end() && cit->second.readonly;
    }


    void
    VolGroup::add(const string& lv_name, const LvAttrs& attrs)
    {
	boost::unique_lock<boost::shared_mutex> lock(vg_mutex);

	if (!lv_info_map.insert(make_pair(lv_name, attrs)).second)
	{
	    y2err(lv_name << " already in cache");
	    // TODO: proper exception
	    throw std::exception();
	}
    }


    void
    VolGroup::add_update(const string& vg_name, const string& lv_name)
    {
	SystemCmd cmd(LVSBIN " --noheadings -o lv_name,lv_attr,segtype,pool_lv " + quote(vg_name + "/" + lv_name));
	if (cmd.retcode() != 0 || cmd.numLines() < 1)
	{
	    // TODO: proper exception
	    y2err("Couldn't get cache data for: " << vg_name << "/" << lv_name);
	    throw std::exception();
	}

	vector<string> args;
	const string tmp(cmd.getLine(0));
	boost::split(args, tmp, boost::is_any_of(" \t\n"), boost::token_compress_on);
	if (args.size() < 1)
	{
	    // TODO: proper exception
	    y2err("Couldn't get cache data for: " << vg_name << "/" lv_name);
	    throw std::exception();
	}
	LvAttrs attrs((args.begin() + 1, args.end()));

	boost::unique_lock<boost::shared_mutex> lock(vg_mutex);
	lv_info_map[lv_name] = attrs;
    }


    void
    VolGroup::remove(const string& lv_name)
    {
	boost::unique_lock<boost::shared_mutex> lock(vg_mutex);

	const_iterator cit = lv_info_map.find(lv_name);

	if (cit == lv_info_map.end())
	{
	    y2err(lv_name << " was not present in a cache");
	    // TODO: proper exception
	    throw std::exception();
	}

	lv_info_map.erase(cit);
    }


    void
    VolGroup::rename(const string& old_name, const string& new_name)
    {
	boost::upgrade_lock<boost::shared_mutex> sh_lock(vg_mutex);

	const_iterator cit = lv_info_map.find(old_name);

	if (cit == lv_info_map.end() || lv_info_map.find(new_name) != lv_info_map.end())
	{
	    // TODO: proper exception
	    y2err("error in cache");
	    throw std::exception();
	}

	boost::upgrade_to_unique_lock<boost::shared_mutex> unique_lock(sh_lock);

	lv_info_map.insert(make_pair(new_name, cit->second));

	lv_info_map.erase(cit);
    }


    LvmCache*
    LvmCache::get_lvm_cache()
    {
	static LvmCache cache;
	return &cache;
    }


    LvmCache::LvmCache()
	: caps(LvmCapabilities::get_lvm_capabilities())
    {
    }


    LvmCache::~LvmCache()
    {
	for (const_iterator cit = vgroups.begin(); cit != vgroups.end(); cit++)
	   delete cit->second;
    }


    bool
    LvmCache::find_vg(const string& vg_name, const_iterator& cit)
    {
	boost::shared_lock<boost::shared_mutex> lock(lvm_cache_mutex);

	cit = vgroups.find(vg_name);

	return cit != vgroups.end();
    }


    bool
    LvmCache::active(const string& vg_name, const string& lv_name) const
    {
	const_iterator cit;

	find_vg(vg_name, cit) && cit->second->active(lv_name);
    }


    void
    LvmCache::activate(const string& vg_name, const string& lv_name) const
    {
	const_iterator cit;

	if (!find_vg(vg_name, cit))
	{
	    // TODO: proper exception
	    throw std::exception();
	}

	cit->second->activate(cit->first, lv_name);
    }


    void
    LvmCache::deactivate(const string& vg_name, const string& lv_name) const
    {
	const_iterator cit;

	if (!find_vg(vg_name, cit))
	{
	    // TODO: proper exception
	    throw std::exception();
	}

	cit->second->deactivate(cit->first, lv_name);
    }


    bool
    LvmCache::read_only(const string& vg_name, const string& lv_name) const
    {
	const_iterator cit;

	return find_vg(vg_name, cit) && cit->second->read_only(lv_name);
    }


    bool
    LvmCache::contains(const string& vg_name, const string& lv_name) const
    {
	const_iterator cit;

	return find_vg(vg_name, cit) && cit->second->contains(lv_name);
    }


    bool
    LvmCache::contains_thin(const string& vg_name, const string& lv_name) const
    {
	const_iterator cit;

	return find_vg(vg_name, cit) && cit->second->contains_thin(lv_name);
    }


    void
    LvmCache::add_update(const string& vg_name, const string& lv_name) const
    {
	boost::upgrade_lock<boost::shared_mutex> upgrade_lock(lvm_cache_mutex);

	const_iterator cit = vgroups.find(vg_name);
	if (cit == vgroups.end())
	    add(vg_name, upgrade_lock);
	else
	{
	    upgrade_lock.unlock();
	    cit->second->add_update(lv_name);
	}
    }


    void
    LvmCache::add(const string& vg_name, const string& lv_name) const
    {
	boost::upgrade_lock<boost::shared_mutex> upgrade_lock(lvm_cache_mutex);

	const_iterator cit = vgroups.find(vg_name);
	if (cit != vgroups.end())
	{
	    upgrade_lock.unlock();

	    LvAttrs lv_attrs;

	    lv_attrs.thin = true;
	    lv_attrs.active = caps->get_ignoreactivationskip().empty();
	    lv_attrs.readonly = true;

	    cit->second->add(lv_name, lv_attrs);
	}
	else
	{
	}
    }


    void
    LvmCache::add(const string& vg_name, boost::upgrade_lock<boost::shared_mutex>& upg_lock)
    {
	/*
	* FIXME: There is bug in LVM causing lvs and lvchange commands
	* 	  may fail in certain situations.
	* 	  Concurrent lvs only commands are fine.
	*
	*	  https://bugzilla.redhat.com/show_bug.cgi?id=922568
	*
	* TODO: Add LVS and LVCHANGE wrappers with read/write locks.
	*/
	SystemCmd cmd(LVSBIN " --noheadings -o lv_name,lv_attr,segtype,pool_lv " + quote(vg_name));
	if (cmd.retcode() != 0)
	{
	    y2err("Couldn't get cache data for VG: " << vg_name);
	    throw std::exception();
	}

	vg_content_raw new_content;

	for (unsigned int num = 0; num < cmd.numLines(); num++)
	{
	    vector<string> args;
	    const string tmp(cmd.getLine(num));
	    boost::split(args, tmp, boost::is_any_of(" \t\n"), boost::token_compress_on);
	    if (args.size() > 0)
		new_content.insert(make_pair(args.front(), vector<string>(args.begin() + 1, args.end())));
	}

	VolGroup *p_vg = new VolGroup(new_content, caps);

	boost::upgrade_to_unique_lock<boost::shared_mutex> uniq_lock(upg_lock);
	vgroups.insert(std::make_pair(vg_name, p_vg));
    }


    void
    LvmCache::remove(const string& vg_name, const string& lv_name)
    {
	const_iterator cit;

	if (find_vg(vg_name, cit))
	    cit->second->remove(lv_name);
    }

}
