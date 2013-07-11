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
    LvmImportMetadata::LvmImportMetadata(const string &input, ImportPolicy ipolicy, const Lvm* lvm)
	: ImportMetadata(ipolicy), lvm(lvm)
    {
	y2deb("LvmImportMetadata constructor");

	string::size_type pos = input.find("/");
	if (pos == string::npos)
	{
	    y2err("Invalid LVM import metadata format, request: " << input);
	    throw InvalidImportMetadataException();
	}

	vg_name = input.substr(0, pos);
	lv_name = input.substr(pos + 1);

	if (vg_name.empty() || lv_name.empty() || lv_name.find("/") != string::npos)
	{
	    y2err("Invalid LVM import metadata format, request: " << input);
	    throw InvalidImportMetadataException();
	}


	// TODO: LVM will support format options for time string later.
	//	 use "%F %T" output format instead of hardcoded "%F %T %z"
	SystemCmd cmd(LVSBIN " --noheading -o lv_time " + quote(vg_name + "/" + lv_name));
	if (cmd.retcode() != 0)
	{
	    // NOTE: Time option is not available before LVM2.02.89
	    y2war("can't get time from imported snapshot");
	}
	else
	{
	    string time_string = boost::trim_copy(cmd.getLine(0));
	    /*
	     * TODO: Temporary hack removing time zone info.
	     *	     See previous Todo.
	     */
	    Regex rx("(.*)[[:space:]]+[+-][0-9]{4}$");
	    if (!rx.match(time_string))
		y2war("Unexpected time output from lvs command: " << time_string);
	    else
	    {
		string time_string = rx.cap(1);
		y2deb("time_string with striped time zone info: " << time_string);
		creation_time = scan_datetime(time_string, false);
	    }
	}
    }

    LvmImportMetadata::LvmImportMetadata(const string& vg_name, const string& lv_name, const Lvm* lvm)
	: ImportMetadata(ImportPolicy::NONE), lvm(lvm), vg_name(vg_name), lv_name(lv_name)
    {
    }

    string
    LvmImportMetadata::getDevicePath() const
    {
	return "/dev/" + vg_name + "/" + lv_name;
    }


    bool
    LvmImportMetadata::isEqual(const ImportMetadata& a) const
    {
	const LvmImportMetadata& p_a = static_cast<const LvmImportMetadata&>(a);

	return (this->vg_name == p_a.vg_name &&  this->lv_name == p_a.lv_name);
    }

    bool LvmImportMetadata::isEqual(unsigned int num) const
    {
	return isEqual(LvmImportMetadata(lvm->getVgName(), lvm->snapshotLvName(num), lvm));
    }


    bool
    LvmImportMetadata::checkImportedSnapshot() const
    {
	return lvm->checkImportedSnapshot(vg_name, lv_name, get_fs_uuid(getDevicePath()), (import_policy != CLONE));
    }


    void
    LvmImportMetadata::cloneImportedSnapshot(unsigned int num) const
    {
	lvm->cloneSnapshot(num, vg_name, lv_name);
    }


    void
    LvmImportMetadata::deleteImportedSnapshot() const
    {
	lvm->deleteSnapshot(vg_name, lv_name);
    }

    string
    LvmImportMetadata::get_raw_metadata() const
    {
	return vg_name + "/" + lv_name;
    }

    string LvmImportMetadata::getSnapshotDir(unsigned int num) const
    {
	return lvm->snapshotDir(num);
    }
}
