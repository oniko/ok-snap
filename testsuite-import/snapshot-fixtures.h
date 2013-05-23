#ifndef SNAPSHOT_FIXTURES_H
#define SNAPSHOT_FIXTURES_H

#include <sys/stat.h>

// ouch !!!
#define private public
    #include "snapper/Snapshot.h"
#define private private

#include "snapper/Snapper.h"

#include "testsuite-import/general-fixtures.h"

namespace testsuiteimport { namespace lvm
{
    using std::map;
    using std::string;

    struct SimpleConstructorValid
    {
	SimpleConstructorValid();
	~SimpleConstructorValid() {}

	const snapper::Snapper* f_snapper;
	const snapper::SnapshotType f_type;
	const unsigned int f_num;
	const time_t f_date;
    };

    struct ImportConstructorValid : SimpleConstructorValid
    {
	ImportConstructorValid();
	~ImportConstructorValid() {}

	const snapper::ImportPolicy f_import_policy;
	const snapper::ImportMetadata* f_p_idata;
    };

    struct CopyConstructorNoImport : SimpleConstructorValid
    {
	CopyConstructorNoImport();
	~CopyConstructorNoImport();

	const uid_t f_uid;
	const unsigned int f_pre_num;
	const string f_description;
	const string f_cleanup;
	const map<string,string> f_userdata;
	const bool f_info_modified;
	const bool f_mount_checked;
	const bool f_mount_user_request;
	const unsigned int f_mount_use_count;

	snapper::Snapshot f_origin;
    };

}}
#endif //SNAPSHOT_FIXTURES_H
