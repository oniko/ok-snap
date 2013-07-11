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

#include "snapper/ImportMetadata.h"

namespace snapper
{
    using std::string;

    class Lvm;

    class LvmImportMetadata : public ImportMetadata
    {
    public:

	LvmImportMetadata(const string &input, ImportPolicy ipolicy, const Lvm* fs);

	virtual string getDevicePath() const;

	virtual bool checkImportedSnapshot() const;
	virtual void cloneImportedSnapshot(unsigned int num) const;
	virtual void deleteImportedSnapshot() const;

	virtual string get_raw_metadata() const;

	virtual string getSnapshotDir(unsigned int num) const;
    private:
	LvmImportMetadata(const string &vg_name, const string &lv_name, const Lvm* fs);

	virtual bool isEqual(const ImportMetadata &a) const;
	virtual bool isEqual(unsigned int num) const;

	const Lvm* lvm;

	string vg_name;
	string lv_name;
    };

}
#endif
