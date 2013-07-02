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


#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <fnmatch.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>

#include "snapper/Snapshot.h"
#include "snapper/Snapper.h"
#include "snapper/AppUtil.h"
#include "snapper/XmlFile.h"
#include "snapper/Filesystem.h"
#include "snapper/Enum.h"
#include "snapper/SnapperTmpl.h"
#include "snapper/SnapperDefines.h"
#include "snapper/Exception.h"
#include "snapper/ImportMetadata.h"


namespace snapper
{
    using std::list;


    std::ostream& operator<<(std::ostream& s, const Snapshot& snapshot)
    {
	s << "type:" << toString(snapshot.type) << " num:" << snapshot.num;

	if (snapshot.pre_num != 0)
	    s << " pre-num:" << snapshot.pre_num;

	s << " date:\"" << datetime(snapshot.date, true, true) << "\"";

	s << "uid:" << snapshot.uid;

	if (!snapshot.description.empty())
	    s << " description:\"" << snapshot.description << "\"";

	if (!snapshot.cleanup.empty())
	    s << " cleanup:\"" << snapshot.cleanup << "\"";

	if (!snapshot.userdata.empty())
	    s << " userdata:\"" << snapshot.userdata << "\"";

	s << " import_policy:" << toString(snapshot.getImportPolicy());

	return s;
    }

/*
    Snapshot::Snapshot(const Snapper* snapper, SnapshotType type, unsigned int num, time_t date)
	: snapper(snapper), type(type), num(num), date(date), uid(0), pre_num(0),
	  info_modified(false), mount_checked(false), mount_user_request(false),
	  mount_use_count(0), import_policy(NONE), p_idata(NULL)
    {
    }
*/

    Snapshot::Snapshot(const Snapper* snapper, SnapshotType type, unsigned int num, time_t date,
		       const ImportMetadata* p_imdata)
	: snapper(snapper), type(type), num(num), date(date), uid(0), pre_num(0),
	  info_modified(false), mount_checked(false), mount_user_request(false),
	  mount_use_count(0), p_idata(p_imdata)
    {
    }


    // Directory containing the actual content of the snapshot.
    // For btrfs e.g. "/" or "/home" for current and "/.snapshots/1/snapshot"
    // or "/home/.snapshots/1/snapshot" otherwise.
    // For ext4 e.g. "/" or "/home" for current and "/@1" or "/home@1"
    // otherwise.
    string
    Snapshot::snapshotDir() const

    {
	if (isCurrent())
	    return snapper->subvolumeDir();

	if (getImportPolicy() == ADOPT || getImportPolicy() == ACKNOWLEDGE)
	    return p_idata.get()->getSnapshotDir(num);
	else
	    return snapper->getFilesystem()->snapshotDir(num);
    }


    SDir
    Snapshot::openInfoDir() const
    {
	if (isCurrent())
	    throw IllegalSnapshotException();

	SDir infos_dir = snapper->openInfosDir();
	return SDir(infos_dir, decString(num));
    }


    SDir
    Snapshot::openSnapshotDir() const
    {
	if (isCurrent())
	    return snapper->openSubvolumeDir();

	return snapper->getFilesystem()->openSnapshotDir(num);
    }


    void
    Snapshot::setUid(uid_t val)
    {
	if (isCurrent())
	    throw IllegalSnapshotException();

	uid = val;
	info_modified = true;
    }


    void
    Snapshot::setDescription(const string& val)
    {
	if (isCurrent())
	    throw IllegalSnapshotException();

	description = val;
	info_modified = true;
    }


    void
    Snapshot::setCleanup(const string& val)
    {
	if (isCurrent())
	    throw IllegalSnapshotException();

	cleanup = val;
	info_modified = true;
    }


    void
    Snapshot::setUserdata(const map<string, string>& val)
    {
	if (isCurrent())
	    throw IllegalSnapshotException();

	for (map<string, string>::const_iterator it = val.begin(); it != val.end(); ++it)
	{
	    if (it->first.empty() || it->first.find_first_of(",=") != string::npos)
		throw InvalidUserdataException();

	    if (it->second.find_first_of(",=") != string::npos)
		throw InvalidUserdataException();
	}

	userdata = val;
	info_modified = true;
    }


    void
    Snapshots::read()
    {
	SDir infos_dir = snapper->openInfosDir();

	vector<string> infos = infos_dir.entries();
	for (vector<string>::const_iterator it1 = infos.begin(); it1 != infos.end(); ++it1)
	{
	    try
	    {
		SDir info_dir(infos_dir, *it1);
		int fd = info_dir.open("info.xml", O_NOFOLLOW | O_CLOEXEC);
		XmlFile file(fd, "");

		const xmlNode* root = file.getRootElement();
		const xmlNode* node = getChildNode(root, "snapshot");

		string tmp;

		SnapshotType type;
		if (!getChildValue(node, "type", tmp) || !toValue(tmp, type, true))
		{
		    y2err("type missing or invalid. not adding snapshot " << *it1);
		    continue;
		}

		unsigned int num;
		if (!getChildValue(node, "num", num) || num == 0)
		{
		    y2err("num missing or invalid. not adding snapshot " << *it1);
		    continue;
		}

		time_t date;
		if (!getChildValue(node, "date", tmp) || (date = scan_datetime(tmp, true)) == (time_t)(-1))
		{
		    y2err("date missing or invalid. not adding snapshot " << *it1);
		    continue;
		}

		ImportPolicy policy = NONE;
		ImportMetadata* p_imdata = NULL;

		const xmlNode *import_node = getElementNode(node, "import");
		if (import_node)
		{
		    if (!getNodePropValue(import_node, "policy", tmp) || !toValue(tmp, policy, true))
		    {
			y2err("import policy is missing or invalid. not adding imported snapshot " << *it1);
			continue;
		    }

		    // on path xmlFile -> snapper, CLONE import policy is useless.
		    // Later, CLONE policy may be respected for informational purposes
		    if (policy == CLONE)
			policy = NONE;

		    if (policy != NONE)
		    {
			map<string,string> raw_import_metadata;

			const list<const xmlNode *> nodes = getChildNodes(getChildNode(node, "import"), "import_metadata");
			for (list<const xmlNode*>::const_iterator nit = nodes.begin(); nit != nodes.end(); nit++)
			{
			    string key, value;
			    getChildValue(*nit, "key", key);
			    getChildValue(*nit, "value", value);
			    if (!key.empty())
				raw_import_metadata[key] = value;
			}

			y2deb("raw_import_metadata: " << raw_import_metadata);

			try {
			    p_imdata = snapper->getFilesystem()->createImportMetadata(raw_import_metadata, policy);
			}
			catch (const InvalidImportMetadataException &e)
			{
			    y2err("corrupted import metadata. not adding imported snapshot " << *it1);
			    continue;
			}
		    }
		}
		Snapshot snapshot(snapper, type, num, date, p_imdata);

		*it1 >> num;
		if (num != snapshot.num)
		{
		    y2err("num mismatch. not adding snapshot " << *it1);
		    continue;
		}

		getChildValue(node, "uid", snapshot.uid);

		getChildValue(node, "pre_num", snapshot.pre_num);

		getChildValue(node, "description", snapshot.description);

		getChildValue(node, "cleanup", snapshot.cleanup);

		const list<const xmlNode*> l = getChildNodes(node, "userdata");
		for (list<const xmlNode*>::const_iterator it2 = l.begin(); it2 != l.end(); ++it2)
		{
		    string key, value;
		    getChildValue(*it2, "key", key);
		    getChildValue(*it2, "value", value);
		    if (!key.empty())
			snapshot.userdata[key] = value;
		}
		y2deb("user_data:" << snapshot.userdata);

		if (snapshot.getImportPolicy() == NONE || snapshot.getImportPolicy() == CLONE)
		{
		    if (!snapper->getFilesystem()->checkSnapshot(snapshot.num))
		    {
			y2err("snapshot check failed. not adding snapshot " << *it1);
			continue;
		    }
		}
		else
		{
		    if (!snapshot.p_idata->checkImportedSnapshot())
		    {
			y2err("snapshot check failed. not adding imported snapshot " << *it1);
			continue;
		    }
		}

		entries.push_back(snapshot);
	    }
	    catch (const IOErrorException& e)
	    {
		y2err("loading " << *it1 << " failed");
	    }
	}

	entries.sort();

	y2mil("found " << entries.size() << " snapshots");
    }


    void
    Snapshots::check() const
    {
	time_t t0 = time(NULL);
	time_t t1 = (time_t)(-1);

	for (const_iterator i1 = begin(); i1 != end(); ++i1)
	{
	    switch (i1->type)
	    {
		case SINGLE:
		{
		}
		break;

		case PRE:
		{
		    int n = 0;
		    for (const_iterator i2 = begin(); i2 != end(); ++i2)
			if (i2->pre_num == i1->num)
			    n++;
		    if (n > 1)
			y2err("pre-num " << i1->num << " has " << n << " post-nums");
		}
		break;

		case POST:
		{
		    if (i1->pre_num > i1->num)
			y2err("pre-num " << i1->pre_num << " larger than post-num " << i1->num);

		    const_iterator i2 = find(i1->pre_num);
		    if (i2 == end())
			y2err("pre-num " << i1->pre_num << " for post-num " << i1->num <<
			      " does not exist");
		    else
			if (i2->type != PRE)
			    y2err("pre-num " << i1->pre_num << " for post-num " << i1->num <<
				  " is of type " << toString(i2->type));
		}
		break;
	    }

	    if (!i1->isCurrent())
	    {
		if (i1->date > t0)
		    y2err("snapshot num " << i1->num << " in future");

		if (t1 != (time_t)(-1) && i1->date < t1)
		    y2err("time shift detected at snapshot num " << i1->num);

		t1 = i1->date;
	    }
	}
    }


    void
    Snapshots::initialize()
    {
	entries.clear();

	Snapshot snapshot(snapper, SINGLE, 0, (time_t)(-1), NULL);
	snapshot.description = "current";
	entries.push_back(snapshot);

	try
	{
	    read();
	}
	catch (const IOErrorException& e)
	{
	    y2err("reading failed");
	}

	check();
    }


    Snapshots::iterator
    Snapshots::findPost(const_iterator pre)
    {
	if (pre == entries.end() || pre->isCurrent() || pre->getType() != PRE)
	    throw IllegalSnapshotException();

	for (iterator it = begin(); it != end(); ++it)
	{
	    if (it->getType() == POST && it->getPreNum() == pre->getNum())
		return it;
	}

	return end();
    }


    Snapshots::const_iterator
    Snapshots::findPost(const_iterator pre) const
    {
	if (pre == entries.end() || pre->isCurrent() || pre->getType() != PRE)
	    throw IllegalSnapshotException();

	for (const_iterator it = begin(); it != end(); ++it)
	{
	    if (it->getType() == POST && it->getPreNum() == pre->getNum())
		return it;
	}

	return end();
    }


    Snapshots::iterator
    Snapshots::findPre(const_iterator post)
    {
	if (post == entries.end() || post->isCurrent() || post->getType() != POST)
	    throw IllegalSnapshotException();

	return find(post->pre_num);
    }


    Snapshots::const_iterator
    Snapshots::findPre(const_iterator post) const
    {
	if (post == entries.end() || post->isCurrent() || post->getType() != POST)
	    throw IllegalSnapshotException();

	return find(post->pre_num);
    }


    unsigned int
    Snapshots::nextNumber()
    {
	unsigned int num = entries.empty() ? 0 : entries.rbegin()->num;

	SDir infos_dir = snapper->openInfosDir();

	while (true)
	{
	    ++num;

	    if (snapper->getFilesystem()->checkSnapshot(num))
		continue;

	    if (infos_dir.mkdir(decString(num), 0777) == 0)
		break;

	    if (errno == EEXIST)
		continue;

	    y2err("mkdir failed errno:" << errno << " (" << stringerror(errno) << ")");
	    throw IOErrorException();
	}

	infos_dir.chmod(decString(num), 0755, 0);

	return num;
    }


    void
    Snapshot::flushInfo()
    {
	if (!info_modified)
	    return;

	writeInfo();
	info_modified = false;
    }


    void
    Snapshot::writeInfo() const
    {
	XmlFile xml;
	xmlNode* node = xmlNewNode("snapshot");
	xml.setRootElement(node);

	setChildValue(node, "type", toString(type));

	setChildValue(node, "num", num);

	setChildValue(node, "date", datetime(date, true, true));

	setChildValue(node, "uid", uid);

	if (type == POST)
	    setChildValue(node, "pre_num", pre_num);

	if (!description.empty())
	    setChildValue(node, "description", description);

	if (!cleanup.empty())
	    setChildValue(node, "cleanup", cleanup);

	for (map<string, string>::const_iterator it = userdata.begin(); it != userdata.end(); ++it)
	{
	    xmlNode* userdata_node = xmlNewChild(node, "userdata");
	    setChildValue(userdata_node, "key", it->first);
	    setChildValue(userdata_node, "value", it->second);
	}

	if (getImportPolicy() != NONE)
	{
	    xmlNode* import_node = xmlNewChild(node, "import");
	    xmlNewAttr(import_node, "policy", toString(getImportPolicy()).c_str());

	    const map<string,string> raw = p_idata.get()->raw_metadata();

	    for (map<string,string>::const_iterator cit = raw.begin(); cit != raw.end(); cit++)
	    {
		xmlNode* imdata_node = xmlNewChild(import_node, "import_metadata");
		setChildValue(imdata_node, "key", cit->first);
		setChildValue(imdata_node, "value", cit->second);
	    }
	}

	string file_name = "info.xml";
	string tmp_name = file_name + ".tmp-XXXXXX";

	SDir info_dir = openInfoDir();

	xml.save(info_dir.mktemp(tmp_name));

	if (info_dir.rename(tmp_name, file_name) != 0)
	{
	    y2err("rename info.xml failed infoDir: " << info_dir.fullname() << " errno: " <<
		  errno << " (" << stringerror(errno) << ")");
	    throw IOErrorException();
	}
    }


    void
    Snapshot::mountFilesystemSnapshot(bool user_request) const
    {
	if (isCurrent())
	    throw IllegalSnapshotException();

	if (!mount_checked)
	{
	    mount_user_request = snapper->getFilesystem()->isSnapshotMounted(num);
	    mount_checked = true;
	}

	if (user_request)
	    mount_user_request = true;
	else
	    mount_use_count++;

	if (getImportPolicy() == ACKNOWLEDGE  || getImportPolicy() == ADOPT)
	    snapper->getFilesystem()->mountSnapshot(num, p_idata.get()->getDevicePath());
	else
	    snapper->getFilesystem()->mountSnapshot(num);
    }


    void
    Snapshot::umountFilesystemSnapshot(bool user_request) const
    {
	if (isCurrent())
	    throw IllegalSnapshotException();

	if (!mount_checked)
	{
	    mount_user_request = snapper->getFilesystem()->isSnapshotMounted(num);
	    mount_checked = true;
	}

	if (user_request)
	    mount_user_request = false;
	else
	    mount_use_count--;

	if (user_request && mount_use_count == 0)
	    snapper->getFilesystem()->umountSnapshot(num);
    }


    void
    Snapshot::handleUmountFilesystemSnapshot() const
    {
	if (!mount_checked)
	    return;

	if (!mount_user_request && mount_use_count == 0)
	    snapper->getFilesystem()->umountSnapshot(num);
    }


    void
    Snapshot::createFilesystemSnapshot() const
    {
	if (isCurrent())
	    throw IllegalSnapshotException();

	snapper->getFilesystem()->createSnapshot(num);
    }


    void
    Snapshot::deleteFilesystemSnapshot() const
    {
	if (isCurrent())
	    throw IllegalSnapshotException();

	snapper->getFilesystem()->umountSnapshot(num);

	switch (getImportPolicy())
	{
	    case NONE:
		y2deb(toString(getImportPolicy()) + " delete milestone");
	    case CLONE:
		y2deb(toString(getImportPolicy()) + " delete milestone: entering deleteSnapshot(num)");
		snapper->getFilesystem()->deleteSnapshot(num);
		break;
	    case ADOPT:
		y2deb(toString(getImportPolicy()) + " delete milestone: entering deleteImportedSnapshot(num)");
		p_idata.get()->deleteImportedSnapshot();
	    case ACKNOWLEDGE:
		y2deb(toString(getImportPolicy()) + " delete milestone: entering removeEnvironment()");
		removeEnvironemt();
		break;
	}
    }


    ImportPolicy Snapshot::getImportPolicy() const
    {
	return p_idata.get() ? p_idata.get()->getImportPolicy() : ImportPolicy::NONE;
    }


    void
    Snapshot::removeEnvironemt() const
    {
	snapper->getFilesystem()->removeSnapshotEnvironment(num);
    }


    void
    Snapshot::createEnvironment() const
    {
	snapper->getFilesystem()->createSnapshotEnvironment(num);
    }


    void
    Snapshot::cloneFilesystemSnapshot() const
    {
	// TODO: Why? What's wrong with that? hmm... sure. it's same like createSnapshot...
	if (isCurrent())
	    throw IllegalSnapshotException();

	p_idata.get()->cloneImportedSnapshot(num);
    }


    Snapshots::iterator
    Snapshots::createSingleSnapshot(string description)
    {
	Snapshot snapshot(snapper, SINGLE, nextNumber(), time(NULL), NULL);
	snapshot.description = description;
	snapshot.info_modified = true;

	return createHelper(snapshot);
    }


    Snapshots::iterator
    Snapshots::createPreSnapshot(string description)
    {
	Snapshot snapshot(snapper, PRE, nextNumber(), time(NULL), NULL);
	snapshot.description = description;
	snapshot.info_modified = true;

	return createHelper(snapshot);
    }


    Snapshots::iterator
    Snapshots::createPostSnapshot(string description, Snapshots::const_iterator pre)
    {
	if (pre == entries.end() || pre->isCurrent() || pre->getType() != PRE ||
	    findPost(pre) != entries.end())
	    throw IllegalSnapshotException();

	Snapshot snapshot(snapper, POST, nextNumber(), time(NULL), NULL);
	snapshot.description = description;
	snapshot.pre_num = pre->getNum();
	snapshot.info_modified = true;

	return createHelper(snapshot);
    }


    Snapshots::iterator
    Snapshots::importSingleSnapshot(const string &description, unsigned char raw_import_policy, const map<string,string> &raw_import_metadata)
    {
	ImportPolicy ipolicy = createImportPolicy(raw_import_policy);

	ImportMetadata *p_idata = snapper->getFilesystem()->createImportMetadata(raw_import_metadata, ipolicy);

	time_t tmp_date = p_idata->getCreationTime();

	if (ipolicy == CLONE || tmp_date == (time_t) (-1))
	    tmp_date = time(NULL);

	Snapshot snapshot(snapper, SINGLE, nextNumber(), tmp_date, p_idata);
	snapshot.description = description;
	snapshot.info_modified = true;

	return importHelper(snapshot);
    }


    Snapshots::iterator
    Snapshots::importPreSnapshot(const string &description, unsigned char raw_import_policy, const map<string,string> &raw_import_metadata)
    {
	ImportPolicy ipolicy = createImportPolicy(raw_import_policy);

	ImportMetadata *p_idata = snapper->getFilesystem()->createImportMetadata(raw_import_metadata, ipolicy);

	time_t tmp_date = p_idata->getCreationTime();

	if (ipolicy == CLONE || tmp_date == (time_t) (-1))
	    tmp_date = time(NULL);

	Snapshot snapshot(snapper, PRE, nextNumber(), tmp_date, p_idata);
	snapshot.description = description;
	snapshot.info_modified = true;

	return importHelper(snapshot);
    }


    Snapshots::iterator
    Snapshots::importPostSnapshot(const string &description, Snapshots::const_iterator pre, unsigned char raw_import_policy, const map<string,string> &raw_import_metadata)
    {
	if (pre == entries.end() || pre->isCurrent() || pre->getType() != PRE ||
	    findPost(pre) != entries.end())
	    throw IllegalSnapshotException();

	ImportPolicy ipolicy = createImportPolicy(raw_import_policy);

	ImportMetadata *p_idata = snapper->getFilesystem()->createImportMetadata(raw_import_metadata, ipolicy);

	time_t tmp_date = p_idata->getCreationTime();

	if (ipolicy == CLONE || tmp_date == (time_t) (-1))
	    tmp_date = time(NULL);

	Snapshot snapshot(snapper, POST, nextNumber(), tmp_date, p_idata);
	snapshot.description = description;
	snapshot.pre_num = pre->getNum();
	snapshot.info_modified = true;

	return importHelper(snapshot);
    }


    Snapshots::iterator
    Snapshots::createHelper(Snapshot& snapshot)
    {
	try
	{
	    snapshot.createFilesystemSnapshot();
	}
	catch (const CreateSnapshotFailedException& e)
	{
	    SDir infos_dir = snapper->openInfosDir();
	    infos_dir.unlink(decString(snapshot.getNum()), AT_REMOVEDIR);
	    throw;
	}

	try
	{
	    snapshot.flushInfo();
	}
	catch (const IOErrorException& e)
	{
	    snapshot.deleteFilesystemSnapshot();
	    SDir infos_dir = snapper->openInfosDir();
	    infos_dir.unlink(decString(snapshot.getNum()), AT_REMOVEDIR);
	    throw;
	}

	return entries.insert(entries.end(), snapshot);
    }


    Snapshots::iterator
    Snapshots::importHelper(Snapshot& snapshot)
    {
	ImportHelper ihelper(snapshot, *this);

	ihelper.importSnapshot();

	try
	{
	    snapshot.flushInfo();
	}
	catch (const IOErrorException& e)
	{
	    snapshot.deleteFilesystemSnapshot();

	    SDir infos_dir = snapshot.snapper->openInfosDir();
	    infos_dir.unlink(decString(snapshot.getNum()), AT_REMOVEDIR);
	    throw;
	}

	return entries.insert(entries.end(), snapshot);
    }


    static bool
    is_filelist_file(unsigned char type, const char* name)
    {
	return (type == DT_UNKNOWN || type == DT_REG) && (fnmatch("filelist-*.txt", name, 0) == 0);
    }


    void
    Snapshots::deleteSnapshot(iterator snapshot)
    {
	if (snapshot == entries.end() || snapshot->isCurrent())
	    throw IllegalSnapshotException();

	snapshot->deleteFilesystemSnapshot();

	SDir info_dir = snapshot->openInfoDir();

	info_dir.unlink("info.xml", 0);

	vector<string> tmp1 = info_dir.entries(is_filelist_file);
	for (vector<string>::const_iterator it = tmp1.begin(); it != tmp1.end(); ++it)
	{
	    info_dir.unlink(*it, 0);
	}

	for (Snapshots::iterator it = begin(); it != end(); ++it)
	{
	    if (!it->isCurrent())
	    {
		SDir tmp2 = it->openInfoDir();
		tmp2.unlink("filelist-" + decString(snapshot->getNum()) + ".txt", 0);
	    }
	}

	SDir infos_dir = snapper->openInfosDir();
	infos_dir.unlink(decString(snapshot->getNum()), AT_REMOVEDIR);

	entries.erase(snapshot);
    }


    struct num_is
    {
	num_is(unsigned int num) : num(num) {}
	bool operator()(const Snapshot& s) const { return s.getNum() == num; }
	const unsigned int num;
    };


    Snapshots::iterator
    Snapshots::find(unsigned int num)
    {
	return find_if(entries.begin(), entries.end(), num_is(num));
    }


    Snapshots::const_iterator
    Snapshots::find(unsigned int num) const
    {
	return find_if(entries.begin(), entries.end(), num_is(num));
    }


    void ImportHelper::importSnapshot() const
    {
	try
	{
	    switch (snapshot.getImportPolicy())
	    {
		case NONE:
		    y2err("Illegal import policy");
		    throw IllegalSnapshotException();

		case CLONE:
		    if (!snapshot.p_idata.get()->checkImportedSnapshot())
		    {
			y2err("Check imported snapshot failed");
			throw IllegalSnapshotException();
		    }

		    snapshot.cloneFilesystemSnapshot();
		    break;

		case ADOPT:
		case ACKNOWLEDGE:
		    if (!snapshot.p_idata.get()->checkImportedSnapshot())
		    {
			y2err("Check imported snapshot failed");
			throw IllegalSnapshotException();
		    }

		    for (Snapshots::const_iterator cit = snapshots.begin(); cit != snapshots.end(); cit++)
		    {
			if (cit->isCurrent())
			    continue;

			switch(cit->getImportPolicy())
			{
			    case NONE:
			    case CLONE:
				if (*snapshot.p_idata.get() == cit->getNum())
				{
				    y2err("Snapshot already owned by snapper exists. Number: " << cit->getNum());
				    throw ImportSnapshotFailedException();
				}
				continue;

			    case ADOPT:
			    case ACKNOWLEDGE:
				if (*snapshot.p_idata.get() == *cit->p_idata.get())
				{
				    y2err("Snapshot already imported in snapshot No. " << cit->getNum());
				    throw ImportSnapshotFailedException();
				}
				break;
			}
		    }

		    snapshot.createEnvironment();
		    break;
	    }
	}
	catch (const IllegalSnapshotException &e)
	{
	    SDir info_dir = snapshot.openInfoDir();
	    info_dir.unlink("snapshot", AT_REMOVEDIR);

	    SDir infos_dir = snapshot.snapper->openInfosDir();
	    infos_dir.unlink(decString(snapshot.getNum()), AT_REMOVEDIR);
	    throw;
	}
	catch (const ImportSnapshotFailedException &e)
	{
	    SDir info_dir = snapshot.openInfoDir();
	    info_dir.unlink("snapshot", AT_REMOVEDIR);

	    SDir infos_dir = snapshot.snapper->openInfosDir();
	    infos_dir.unlink(decString(snapshot.getNum()), AT_REMOVEDIR);
	    throw;
	}
    }

}
