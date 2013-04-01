/*
 * Copyright (c) 2011 Novell, Inc.
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


#ifndef SNAPPER_SNAPSHOT_H
#define SNAPPER_SNAPSHOT_H


#include <time.h>
#include <string>
#include <list>
#include <map>

#include "snapper/Exception.h"
#include "snapper/ImportMetadata.h"


namespace snapper
{
    using std::string;
    using std::list;
    using std::map;


    class Snapper;
    class SDir;


    enum SnapshotType { SINGLE, PRE, POST };


    struct CreateSnapshotFailedException : public SnapperException
    {
	explicit CreateSnapshotFailedException() throw() {}
	virtual const char* what() const throw() { return "create snapshot failed"; }
    };

    struct DeleteSnapshotFailedException : public SnapperException
    {
	explicit DeleteSnapshotFailedException() throw() {}
	virtual const char* what() const throw() { return "delete snapshot failed"; }
    };


    struct IsSnapshotMountedFailedException : public SnapperException
    {
	explicit IsSnapshotMountedFailedException() throw() {}
	virtual const char* what() const throw() { return "is snapshot mounted failed"; }
    };

    struct MountSnapshotFailedException : public SnapperException
    {
	explicit MountSnapshotFailedException() throw() {}
	virtual const char* what() const throw() { return "mount snapshot failed"; }
    };

    struct UmountSnapshotFailedException : public SnapperException
    {
	explicit UmountSnapshotFailedException() throw() {}
	virtual const char* what() const throw() { return "umount snapshot failed"; }
    };

    // NOTE: temporary exception about to be removed when import feature is complete
    struct UnsupportedSnapshotOperation : public SnapperException
    {
	explicit UnsupportedSnapshotOperation() throw() {}
	virtual const char* what() const throw() { return "unsupported snapshot operation"; }
    };


    struct ImportSnapshotFailedException : public SnapperException
    {
	explicit ImportSnapshotFailedException() throw() {}
	virtual const char* what() const throw() { return "import snapshot failed"; }
    };


    class Snapshot
    {
    public:

	friend class Snapshots;

	Snapshot(const Snapshot& snapshot);
	Snapshot(const Snapper* snapper, SnapshotType type, unsigned int num, time_t date);
	Snapshot(const Snapper* snapper, SnapshotType type, unsigned int num, time_t date, ImportPolicy ipolicy, const ImportMetadata* p_idata);
	~Snapshot();

	SnapshotType getType() const { return type; }
	ImportPolicy getImportPolicy() const { return import_policy; }

	unsigned int getNum() const { return num; }
	bool isCurrent() const { return num == 0; }

	time_t getDate() const { return date; }

	void setUid(uid_t uid);
	uid_t getUid() const { return uid; }

	unsigned int getPreNum() const { return pre_num; }

	void setDescription(const string& description);
	string getDescription() const { return description; }

	void setCleanup(const string& cleanup);
	string getCleanup() const { return cleanup; }

	void setUserdata(const map<string, string>& userdata);
	map<string, string> getUserdata() const { return userdata; }

	void flushInfo();

	string snapshotDir() const;

	SDir openInfoDir() const;
	SDir openSnapshotDir() const;

	void mountFilesystemSnapshot(bool user_request) const;
	void umountFilesystemSnapshot(bool user_request) const;
	void handleUmountFilesystemSnapshot() const;

	friend std::ostream& operator<<(std::ostream& s, const Snapshot& snapshot);

    private:
	const Snapper* snapper;

	SnapshotType type;

	unsigned int num;

	time_t date;

	uid_t uid;

	unsigned int pre_num;	// valid only for type=POST

	string description;	// likely empty for type=POST

	string cleanup;

	map<string, string> userdata;

	bool info_modified;

	mutable bool mount_checked;
	mutable bool mount_user_request;
	mutable unsigned int mount_use_count;

	ImportPolicy import_policy;

	const ImportMetadata* p_idata;

	void writeInfo() const;

	void cloneFilesystemSnapshot() const;
	void createFilesystemSnapshot() const;
	void deleteFilesystemSnapshot() const;

    };


    inline bool operator<(const Snapshot& a, const Snapshot& b)
    {
	return a.getNum() < b.getNum();
    }


    class Snapshots
    {
    public:

	friend class Snapper;

	Snapshots(const Snapper* snapper) : snapper(snapper) {}

	typedef list<Snapshot>::iterator iterator;
	typedef list<Snapshot>::const_iterator const_iterator;
	typedef list<Snapshot>::size_type size_type;

	iterator begin() { return entries.begin(); }
	const_iterator begin() const { return entries.begin(); }

	iterator end() { return entries.end(); }
	const_iterator end() const { return entries.end(); }

	size_type size() const { return entries.size(); }

	iterator find(unsigned int num);
	const_iterator find(unsigned int num) const;

	iterator findPre(const_iterator post);
	const_iterator findPre(const_iterator post) const;

	iterator findPost(const_iterator pre);
	const_iterator findPost(const_iterator pre) const;

	const_iterator getSnapshotCurrent() const { return entries.begin(); }

    private:

	void initialize();

	void read();

	void check() const;

	iterator createSingleSnapshot(string description);
	iterator createPreSnapshot(string description);
	iterator createPostSnapshot(string description, const_iterator pre);

	iterator importSingleSnapshot(const string &description, unsigned char raw_import_policy, const map<string,string> &raw_import_metadata);
	iterator importPreSnapshot(const string &description, unsigned char raw_import_policy, const map<string,string> &raw_import_metadata);
	iterator importPostSnapshot(const string &description, Snapshots::const_iterator pre, unsigned char raw_import_policy, const map<string,string> &raw_import_metadata);

	iterator createHelper(Snapshot& snapshot);
	iterator importHelper(Snapshot& snapshot);

	void deleteSnapshot(iterator snapshot);

	unsigned int nextNumber();

	const Snapper* snapper;

	list<Snapshot> entries;

    };

}


#endif
