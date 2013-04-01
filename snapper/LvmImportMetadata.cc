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

#include <boost/algorithm/string.hpp>

#include "snapper/Log.h"
#include "snapper/AppUtil.h"
#include "snapper/ImportMetadata.h"
#include "snapper/LvmImportMetadata.h"
#include "snapper/SystemCmd.h"
#include "snapper/Log.h"
#include "snapper/Lvm.h"
#include "snapper/Exception.h"
#include "snapper/Regex.h"

namespace snapper
{
    LvmImportMetadata::LvmImportMetadata(const map<string,string> &input, const Lvm* lvm) : ImportMetadata()
    {
	y2deb("LvmImportMetadata constructor");

	map<string,string>::const_iterator cit;
	imd_map = map<string,string>();
	string vg_name, lv_name;

	cit = input.find("vg_name");
	if (cit == input.end())
	{
	    y2err("can't get vg_name from raw import metadata");
	    throw InvalidImportMetadataException();
	}
	imd_map[cit->first] = vg_name = cit->second;

	cit = input.find("lv_name");
	if (cit == input.end())
	{
	    y2err("can't get lv_name from raw import metadata");
	    throw InvalidImportMetadataException();
	}
	imd_map[cit->first] = lv_name = cit->second;

	SystemCmd cmd(LVSBIN " --noheading -o lv_time " + quote(vg_name + "/" + lv_name));
	if (cmd.retcode() != 0)
	{
	    // NOTE: This  feature has been added in later versions of LVM2
	    y2war("cannot get time from imported snapshot");
	}
	else
	{
	    // TODO: need to cut off TZ info at the end of LV time string (i.e. +0200)
	    string time_string = boost::trim_copy(cmd.getLine(0));
	    creation_time = scan_datetime(time_string, false);
	}

	this->lvm = lvm;
    }


    LvmImportMetadata::LvmImportMetadata(const LvmImportMetadata& p_idata): ImportMetadata(p_idata)
    {
	y2deb("LvmImportMetadata copy constructor");
	imd_map = p_idata.imd_map;
	lvm = p_idata.lvm;
    }


    ImportMetadata* LvmImportMetadata::clone() const
    {
	return new LvmImportMetadata(*this);
    }


    string
    LvmImportMetadata::getDevicePath() const
    {
	return "/dev/" + getVgName() + "/" + getLvName();
    }


    bool
    LvmImportMetadata::isEqual(const ImportMetadata& a) const
    {
	const LvmImportMetadata* p_a = dynamic_cast<const LvmImportMetadata*>(&a);
	if (!p_a)
	{
	    y2err("FATAL: tried to comapare LvmImportMetadata w/ incompatible type!");
	    throw InvalidImportMetadataException();
	}

	return (p_a->getVgName() == getVgName() && p_a->getLvName() == getLvName());
    }


    bool LvmImportMetadata::checkImportedSnapshot() const
    {
	// TODO: think about adding UUID check
	return lvm->detectThinVolumeNames(getVgName(), getLvName());
    }


    void
    LvmImportMetadata::cloneImportedSnapshot(unsigned int num) const
    {
	lvm->cloneSnapshot(num, getVgName(), getLvName());
    }


    void
    LvmImportMetadata::deleteImportedSnapshot(unsigned int num) const
    {
	lvm->deleteSnapshot(num, getVgName(), getLvName());
    }


    map<string,string>::const_iterator
    LvmImportMetadata::info_cbegin() const
    {
	return imd_map.begin();
    }


    map<string,string>::const_iterator
    LvmImportMetadata::info_cend() const
    {
	return imd_map.end();
    }

}
