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

#ifndef SNAPPER_IMPORTMETADATA_H
#define SNAPPER_IMPORTMETADATA_H

#include "config.h"

#include <stdint.h>
#include <sys/time.h>

#include <map>
#include <string>

#include "snapper/Log.h"

namespace snapper
{
    using std::map;
    using std::string;

    /*
     * Define these types of import policy:
     *
     * NONE:		A snapshot (created by snapper)
     *
     * CLONE:		Used only on path 'dbus->snapper->xmlFile'. it creates a clone of an appointed snapshot
     *			On a way backward (xmlfile->snapper), it will become NONE type, undistinguishable from
     *			regular snapshot.
     *
     * ADOPT:		Takeover an imported snapshot (it will become 'owned by' the snapper and deleted if
     * 			snapper decides so (cleanup policy, command, ...))
     *
     * ACKNOWLEDGE:	add snapshot into snapper's environement but do not manipulate the snapshot (e.g. delete it)
     */

    // keep it 1 byte as it's supposed to be transfered via dbus as the BYTE type
    enum ImportPolicy : unsigned char { NONE = 0, CLONE, ADOPT, ACKNOWLEDGE };
    enum ImportTypeId : unsigned int { BTRFS, EXT4, LVM2 };

    ImportPolicy createImportPolicy(unsigned char raw);

    class ImportMetadata
    {
    protected:

	time_t creation_time;

	virtual bool isEqualImpl(const ImportMetadata& idata) const = 0;
	virtual ImportTypeId getImportMetadataId() const = 0;

    public:

	ImportMetadata() { creation_time = (time_t)(-1); y2deb("ImportMetadata ctor"); }
	virtual ~ImportMetadata() { y2deb("ImportMetadata dtor"); }

	/*
	 * if fs snapshot doesn't containt information about creation time
	 * it'll return time_t(-1)
	 */
	virtual time_t getCreationTime() const { return creation_time; }

	// used only during mount operation on ADOPT/ACKNOWLEDGE types of import
	virtual string getDevicePath() const = 0;

	bool isEqual(const ImportMetadata &b) const;
	virtual bool isEqual(unsigned int num) const = 0;

	// check if the metadata desribes valid snapshot
	virtual bool checkImportedSnapshot() const = 0;
	// clone described snapshot into snapper's own snapshot
	virtual void cloneImportedSnapshot(unsigned int num) const = 0;
	// delete imported snapshot (ADOPT only import)
	virtual void deleteImportedSnapshot(unsigned int num) const = 0;

	virtual map<string,string> raw_metadata() const = 0;

	virtual string getSnapshotDir(unsigned int num) const = 0;
    };

}
#endif
