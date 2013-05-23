#include "testsuite-import/snapshot-fixtures.h"

#include "snapper/ImportMetadata.h"

namespace testsuiteimport { namespace lvm
{
    using std::make_pair;

    SimpleConstructorValid::SimpleConstructorValid()
	: f_snapper(static_cast<const snapper::Snapper *>(123456789)),
	f_type(snapper::SnapshotType::PRE), f_num(42), f_date(1234554321)
    {
    }

    ImportConstructorValid::ImportConstructorValid()
	: SimpleConstructorValid(), f_import_policy(snapper::ImportPolicy::ADOPT),
	f_p_idata(static_cast<const snapper::ImportMetadata *>(123456789))
    {
    }

    CopyConstructorNoImport::CopyConstructorNoImport()
	: SimpleConstructorValid(), f_uid(1024), f_pre_num(42), f_description("test_desc"),
	f_cleanup("test_cleanup"), f_userdata(make_pair("t_key", "t_value")),
	f_info_modified(true), f_mount_checked(false), f_mount_user_request(true),
	f_mount_use_count(42), f_origin(f_snapper, f_type, f_num, f_date)
    {
	f_origin.setUid(f_uid);
	f_origin.setDescription(f_description);
	f_origin.setCleanup(f_cleanup);
	f_origin.setUserdata(f_userdata);

	// set remaining private attributes for testing purposes
	f_origin.pre_num = f_pre_num;
	f_origin.info_modified = f_info_modified;
	f_origin.mount_user_request = f_mount_user_request;
	f_origin.mount_use_count = f_mount_use_count;
    }
}
