/*
 * Copyright (c) [2011-2013] Novell, Inc.
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
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, contact Novell, Inc.
 *
 * To contact Novell about this file by physical or electronic mail, you may
 * find current contact information at www.novell.com.
 */


#ifndef SNAPPER_LVM_H
#define SNAPPER_LVM_H

#include <boost/noncopyable.hpp>

#include "snapper/Filesystem.h"


namespace snapper
{
    class LvmImportMetadata;
    class ImportMetadata;

    struct LvmActivationException : public std::exception
    {
	explicit LvmActivationException() throw() {}
	virtual const char* what() const throw() { return "lvm snapshot activation exception"; }
    };


    struct LvmDeactivatationException : public std::exception
    {
	explicit LvmDeactivatationException() throw() {}
	virtual const char* what() const throw() { return "lvm snapshot deactivation exception"; }
    };


    struct lvm_version
    {
	lvm_version(uint16_t maj, uint16_t min, uint16_t rev)
	    : version(rev | ((uint32_t)min << 16)  | ((uint64_t)maj << 32)) {}

	const uint64_t version;
    };

    bool operator>=(const lvm_version& a, const lvm_version& b);

    class LvmCapabilities : public boost::noncopyable
    {
    public:
	static LvmCapabilities* get_lvm_capabilities();

	bool get_time_support() const;
	string get_ignoreactivationskip() const;

    private:
	LvmCapabilities();

	// empty or " -K" if lvm supports ignore activation skip flag
	string ignoreactivationskip;
	// true if lvm2 supports time info stored in metadata
	bool time_support;
    };


    class Lvm : public Filesystem
    {
    public:

	friend class LvmImportMetadata;

	static Filesystem* create(const string& fstype, const string& subvolume);
	virtual ImportMetadata* createImportMetadata(const map<string,string> &raw_data) const;

	Lvm(const string& subvolume, const string& mount_type);

	virtual string fstype() const { return "lvm(" + mount_type + ")"; }

	virtual void createConfig() const;
	virtual void deleteConfig() const;

	virtual string snapshotDir(unsigned int num) const;
	virtual string snapshotLvName(unsigned int num) const;

	virtual SDir openInfosDir() const;
	virtual SDir openSnapshotDir(unsigned int num) const;

	virtual void createSnapshotEnvironment(unsigned int num) const;
	virtual void removeSnapshotEnvironment(unsigned int num) const;

	virtual void createSnapshot(unsigned int num) const;
	virtual void deleteSnapshot(unsigned int num) const;

	virtual bool isSnapshotMounted(unsigned int num) const;
	virtual void mountSnapshot(unsigned int num) const;
	virtual void mountSnapshot(unsigned int num, const string &device_path) const;
	virtual void umountSnapshot(unsigned int num) const;

	virtual bool checkSnapshot(unsigned int num) const;

    private:

	const string mount_type;
	const LvmCapabilities* caps;

	bool checkImportedSnapshot(const string &vg_name, const string& lv_name) const;
	bool detectThinVolumeNames(const MtabData& mtab_data);
	bool detectThinVolumeNames(const string& vg_name, const string& lv_name) const;
	void activateSnapshot(const string& vg_name, const string& lv_name) const;
	void deactivateSnapshot(const string& vg_name, const string& lv_name) const;
	bool detectInactiveSnapshot(const string& vg_name, const string& lv_name) const;

	void cloneSnapshot(unsigned int num, const string &vg_name, const string &lv_name) const;
	void deleteSnapshot(const string &vg_name, const string &lv_name) const;
	string getFsUuid() const { return fs_uuid; }

	string getDevice(unsigned int num) const;

	string vg_name;
	string lv_name;
	string fs_uuid;

	vector<string> mount_options;

    };

}


#endif
