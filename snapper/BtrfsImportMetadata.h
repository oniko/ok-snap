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

#ifndef SNAPPER_BTRFSIMPORTMETADATA_H
#define SNAPPER_BTRFSIMPORTMETADATA_H

#define KEY_SUBVOL_ID "subvol_id"
#define KEY_SUBVOLUME "subvolume"

#include "snapper/FileUtils.h"
#include "snapper/ImportMetadata.h"


namespace snapper
{

    class BtrfsImportMetadata : public ImportMetadata
    {
    public:

	BtrfsImportMetadata(const map<string,string> &input, ImportPolicy policy, const Btrfs* btrfs);

	virtual string getDevicePath() const;

	virtual bool checkImportedSnapshot() const;
	virtual void cloneImportedSnapshot(unsigned int num) const;
	virtual void deleteImportedSnapshot(unsigned int num) const;

	virtual map<string, string> raw_metadata() const;

	virtual string getSnapshotDir(unsigned int num) const;
    private:
	BtrfsImportMetadata(const string &subvolume, const Btrfs* btrfs);
	static const ImportTypeId type_id = ImportTypeId::BTRFS;

	const Btrfs* btrfs;
	uint64_t import_subvol_id; // used in comparing imported snapshots

	string import_subvolume; // no starting or trailing "/" allowed!

	virtual ImportTypeId getImportMetadataId() const { return type_id; }

	virtual bool isEqual(const ImportMetadata &a) const;
	virtual bool isEqual(unsigned int num) const;
    };

}
#endif
