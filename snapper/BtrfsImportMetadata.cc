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

#ifdef HAVE_LIBBTRFS
#include <btrfs/btrfs-list.h>
#endif

#include "snapper/Btrfs.h"
#include "snapper/BtrfsImportMetadata.h"
#include "snapper/Exception.h"
#include "snapper/FileUtils.h"
#include "snapper/Log.h"
#include "snapper/SnapperTmpl.h"

namespace snapper
{
    BtrfsImportMetadata::BtrfsImportMetadata(const map< string, string >& input, const Btrfs* btrfs)
	: ImportMetadata(), btrfs(btrfs)
    {
	map<string,string>::const_iterator cit_subv = input.find(KEY_SUBVOLUME);

	if (cit_subv != input.end())
	{
	    import_subvolume = cit_subv->second();
	    SDir root(btrfs->openSubvolumeDir());

	    // remove origin subvolume from import subvolume
	    string::size_type pos = import_subvolume.find(root.fullname());
	    if (pos != string::npos && pos == 0)
		import_subvolume = import_subvolume.substr(root.fullname().length());

	    // import subvolume == origin subvolume
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

	    imd_map[KEY_SUBVOLUME] = import_subvolume;
	}
	else
	{
	    y2err("Invalid btrfs import metadata");
	    throw InvalidImportMetadataException();
	}
    }

    string BtrfsImportMetadata::getDevicePath() const
    {
	// hmm not needed. we'll mount subvolume by either id or by its path
	return "not_needed";
    }

    bool BtrfsImportMetadata::isEqualImpl(const ImportMetadata& a) const
    {
	if (a.getImportMetadataId() != getImportMetadataId())
	    return false;

	const BtrfsImportMetadata &p_a = static_cast<const BtrfsImportMetadata &>(a);

	return import_subvol_id == p_a.import_subvol_id;
    }

    bool BtrfsImportMetadata::checkImportedSnapshot() const
    {
	//TODO: create private btrfs check method in fs class
	return true;
    }
}
