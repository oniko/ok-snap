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

#ifndef SNAPPER_LVMIMPORTMETADATA_H
#define SNAPPER_LVMIMPORTMETADATA_H

#define KEY_VG_NAME "vg_name"
#define KEY_LV_NAME "lv_name"

#include "snapper/ImportMetadata.h"

namespace snapper
{
    using std::map;
    using std::string;

    class Lvm;

    class LvmImportMetadata : public ImportMetadata
    {
    public:

	//friend class Lvm; // ??

	LvmImportMetadata(const map<string,string> &input, const Lvm* fs);

	virtual string getDevicePath() const;

	virtual ImportTypeId getImportMetadataId() const { return type_id; }
	virtual bool isEqualImpl(const ImportMetadata &a) const;
	virtual bool isEqual(unsigned int num) const;

	virtual bool checkImportedSnapshot() const;
	virtual void cloneImportedSnapshot(unsigned int num) const;
	virtual void deleteImportedSnapshot(unsigned int num) const;

	virtual map<string, string> raw_metadata() const;

	virtual string getSnapshotDir(unsigned int num) const;
    private:
	LvmImportMetadata(const string &vg_name, const string &lv_name, const Lvm* fs);
	static const ImportTypeId type_id = ImportTypeId::LVM2;

	const Lvm* lvm;

	string vg_name;
	string lv_name;
    };

}
#endif
