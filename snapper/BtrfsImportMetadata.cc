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

namespace snapper
{
    BtrfsImportMetadata::BtrfsImportMetadata(const map<string,string> &input, ImportPolicy policy, const Btrfs* btrfs)
	: ImportMetadata(policy), btrfs(btrfs)
    {
	map<string,string>::const_iterator cit_subv = input.find(KEY_SUBVOLUME);

	if (cit_subv != input.end())
	{
	    //import_subvolume = cit_subv->second;

	    SDir root(btrfs->openSubvolumeDir());

	    // remove trailing "/"
	    import_subvolume = boost::trim_right_copy_if(cit_subv->second, boost::is_any_of("/ \t"));

	    // remove origin subvolume from import subvolume
	    string::size_type pos = import_subvolume.find(root.fullname());
	    if (pos == 0)
		import_subvolume = import_subvolume.substr(root.fullname().length());

	    // remove heading "/"
	    import_subvolume = boost::trim_left_copy_if(import_subvolume, boost::is_any_of("/ \t"));

	    if (import_subvolume.empty())
	    {
		y2err("Illegal import metadata");
		throw InvalidImportMetadataException();
	    }

	    y2deb("import btrfs subvolume: " << import_subvolume);

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
	: ImportMetadata(ImportPolicy::NONE), btrfs(btrfs), import_subvolume(subvolume)
    {
	import_subvol_id = Btrfs::subvolume_id(SDir::deepopen(btrfs->openSubvolumeDir(), import_subvolume));
    }


    string BtrfsImportMetadata::getDevicePath() const
    {
	// NOTE: not used
	return string();
    }

    bool BtrfsImportMetadata::isEqual(const ImportMetadata& a) const
    {
	const BtrfsImportMetadata &p_a = static_cast<const BtrfsImportMetadata &>(a);

	return import_subvol_id == p_a.import_subvol_id;
    }

    bool BtrfsImportMetadata::isEqual(unsigned int num) const
    {
	try
	{
	    string subvolume = btrfs->snapshotDir(num).substr(btrfs->openSubvolumeDir().fullname().length());
	    subvolume = boost::trim_left_copy_if(subvolume, boost::is_any_of("/"));

	    // trim all leading / chars
	    //subvolume = boost::trim_left_if(subvolume, );
	    //boost::starts_with();

	    y2deb("isEqual subvolume=" << subvolume);

	    return isEqual(BtrfsImportMetadata(subvolume, btrfs));
	}
	catch (const IOErrorException &e)
	{
	    y2war("Failure in BtrfsImportMetadata compare");
	    return false;
	}
    }


    bool BtrfsImportMetadata::checkImportedSnapshot() const
    {
	return btrfs->checkImportedSnapshot(import_subvolume, (import_policy != CLONE));
    }


    void BtrfsImportMetadata::deleteImportedSnapshot() const
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
	SDir root_volume = btrfs->openSubvolumeDir();
	return (root_volume.fullname() == "/" ? "" : root_volume.fullname()) + "/" + import_subvolume;
    }

    map<string,string> BtrfsImportMetadata::raw_metadata() const
    {
	map<string,string> raw;

	raw.insert(make_pair(KEY_SUBVOLUME, import_subvolume));

	return raw;
    }
}
