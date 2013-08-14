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
#include <boost/thread.hpp>

#include "snapper/Log.h"
#include "snapper/LvmCache.h"
#include "snapper/SystemCmd.h"

boost::shared_mutex cache_mutex;

namespace snapper
{
    void
    LvmCache::add(const string& vg_name, const string& lv_name)
    {
	boost::shared_lock<boost::shared_mutex> lock(cache_mutex);

	iterator it = m_caches.find(vg_name);
	if (it == m_caches.end())
	{
	    y2err(vg_name << " VG not in a cache!");
	    throw std::exception();
	}

	it->second.add(lv_name);
    }


    void
    LvmCache::add(const std::string& vg_name)
    {
	boost::upgrade_lock<boost::shared_mutex> sh_lock(cache_mutex);

	iterator it = m_caches.find(vg_name);
	if (it == m_caches.end())
	{
	    boost::upgrade_to_unique_lock<boost::shared_mutex> uniq_lock(sh_lock);

	    SystemCmd cmd(LVSBIN " --noheadings -o lv_name " + quote(vg_name));
	    if (cmd.retcode() != 0)
	    {
		y2err("Couldn't get cache data for VG: " << vg_name);
		throw std::exception();
	    }

	    VolGroup::vg_content_set new_vg;

	    for (vector<string>::const_iterator cit = cmd.stdout().begin(); cit != cmd.stdout().end(); cit++)
		new_vg.insert(boost::trim_copy(*cit));

	    m_caches.insert(make_pair(vg_name, VolGroup(new_vg)));
	}
    }


    bool LvmCache::in_cache(const string& vg_name, const string& lv_name) const
    {
	boost::shared_lock<boost::shared_mutex> lock(cache_mutex);

	const_iterator cit = m_caches.find(vg_name);
	return cit != m_caches.end() && cit->second.contains(lv_name);
    }


    void LvmCache::remove(const string& vg_name, const string& lv_name)
    {
	boost::shared_lock<boost::shared_mutex> lock(cache_mutex);

	iterator it = m_caches.find(vg_name);
	if (it != m_caches.end())
	    it->second.remove(lv_name);
    }


    LvmCache*
    LvmCache::get_lvm_cache()
    {
	static LvmCache cache;

	return &cache;
    }


    void
    VolGroup::add(const std::string& lv_name)
    {
	boost::unique_lock<boost::mutex> lock(_shrd_lock);

	if (!lvs_in_vg.insert(lv_name).second)
	{
	    y2err(lv_name << " already in cache");
	    // TODO: proper exception
	    throw std::exception();
	}
    }


    void
    VolGroup::remove(const std::string& lv_name)
    {
	boost::unique_lock<boost::mutex> lock(_shrd_lock);

	if (lvs_in_vg.erase(lv_name) == 0)
	{
	    y2err(lv_name << " was not present in a cache");
	    // TODO: proper exception
	    throw std::exception();
	}
    }


    bool
    VolGroup::contains(const std::string& lv_name) const
    {
	boost::shared_lock<boost::mutex> lock(_shrd_lock);

	return lvs_in_vg.find(lv_name) != lvs_in_vg.end();
    }


    iterator
    LvmCache::create_new_vg(const string& vg_name)
    {
	return m_caches.insert(make_pair(vg_name, VolGroup)).first;
    }

}
