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



#include "snapper/Btrfs.h"
#include "snapper/BtrfsImportMetadata.h"
#include "snapper/Exception.h"
#include "snapper/FileUtils.h"
#include "snapper/Log.h"
#include "snapper/SnapperTmpl.h"
#include "snapper/Snapshot.h"

namespace snapper
{
    BtrfsImportMetadata::BtrfsImportMetadata(const map<string,string> &input, const Btrfs* btrfs)
	: ImportMetadata(), btrfs(btrfs)
    {
	map<string,string>::const_iterator cit_subv = input.find(KEY_SUBVOLUME);

	if (cit_subv != input.end())
	{
	    import_subvolume = cit_subv->second;

	    SDir root(btrfs->openSubvolumeDir());

	    // remove trailing "/"
	    while (boost::ends_with(import_subvolume, "/"))
		boost::erase_tail(import_subvolume, 1);

	    // remove origin subvolume from import subvolume
	    string::size_type pos = import_subvolume.find(root.fullname());
	    if (pos != string::npos && pos == 0)
		import_subvolume = import_subvolume.substr(root.fullname().length());

	    // remove starting "/"
	    while (boost::starts_with(import_subvolume, "/"))
		boost::erase_head(import_subvolume, 1);

	    if (import_subvolume.empty())
	    {
		y2err("Illegal import metadata");
		throw InvalidImportMetadataException();
	    }

	    try
	    {
		import_subvol_id = Btrfs::subvolume_id(SDir::deepopen(root, import_subvolume));
	    }
	    catch (const IOErrorException &e)
	    {
		y2err("couldn't open subvolume or resolve subvolume_id");
		throw InvalidImportMetadataException();
	    }
	}
	else
	{
	    y2err("Invalid btrfs import metadata");
	    throw InvalidImportMetadataException();
	}
    }

    BtrfsImportMetadata::BtrfsImportMetadata(const string& subvolume, const Btrfs* btrfs)
	: btrfs(btrfs), import_subvolume(subvolume)
    {
	import_subvol_id = Btrfs::subvolume_id(SDir::deepopen(btrfs->openSubvolumeDir(), import_subvolume));
    }


    string BtrfsImportMetadata::getDevicePath() const
    {
	// hmm not needed. we'll mount subvolume by either id or by its path
	return "not_needed";
    }

    bool BtrfsImportMetadata::isEqual(const ImportMetadata& a) const
    {
	const BtrfsImportMetadata &p_a = static_cast<const BtrfsImportMetadata &>(a);

	return import_subvol_id == p_a.import_subvol_id;
    }

    bool BtrfsImportMetadata::isEqual(unsigned int num) const
    {
	string subvolume = btrfs->snapshotDir(num).substr(btrfs->subvolume.length());

	try
	{
	    return isEqual(BtrfsImportMetadata(subvolume, btrfs));
	}
	catch (const IOErrorException &e)
	{
	    return false;
	}
    }


    bool BtrfsImportMetadata::checkImportedSnapshot() const
    {
	return btrfs->checkImportedSnapshot(import_subvolume);
    }

    void BtrfsImportMetadata::deleteImportedSnapshot(unsigned int num) const
    {
	string::size_type pos = import_subvolume.rfind("/");

	if (pos == string::npos)
	    btrfs->deleteSnapshot("", import_subvolume);
	else
	    btrfs->deleteSnapshot(import_subvolume.substr(0, pos), import_subvolume.substr(pos + 1));
    }

    void BtrfsImportMetadata::cloneImportedSnapshot(unsigned int num) const
    {
	btrfs->cloneSnapshot(num, import_subvolume);
    }

    string BtrfsImportMetadata::getSnapshotDir(unsigned int num) const
    {
	return (btrfs->subvolume == "/" ? "" : btrfs->subvolume) + "/" + import_subvolume;
    }

    map<string,string> BtrfsImportMetadata::raw_metadata() const
    {
	map<string,string> raw;

	raw.insert(make_pair(KEY_SUBVOLUME, import_subvolume));

	return raw;
    }
}
