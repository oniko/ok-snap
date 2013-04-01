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
    class Lvm;

    class LvmImportMetadata : public ImportMetadata
    {
    public:

	friend class Lvm;

	LvmImportMetadata(const LvmImportMetadata& p_idata);
	LvmImportMetadata(const map<string,string> &input, const Lvm* fs);

	virtual ImportMetadata* clone() const;

	virtual string getDevicePath() const;

	virtual bool isEqual(const ImportMetadata &a) const;

	virtual bool checkImportedSnapshot() const;
	virtual void cloneImportedSnapshot(unsigned int num) const;
	virtual void deleteImportedSnapshot(unsigned int num) const;

	virtual map<string,string>::const_iterator info_cbegin() const;
	virtual map<string,string>::const_iterator info_cend() const;
    private:
	const Lvm* lvm;
	map<string,string> imd_map;

	string getVgName() const { return imd_map.find("vg_name")->second; }
	string getLvName() const { return imd_map.find("lv_name")->second; }
    };

}
#endif
