#ifndef LVM_SNAPSHOT_FIXTURES_H
#define LVM_SNAPSHOT_FIXTURES_H

#include "snapper/Snapper.h"

#include "testsuite-import/snapshot-fixtures.h"


namespace testsuiteimport { namespace lvm
{
    struct LvmSnapshotFixtures : public LvmGeneralFixture, GeneralSnapshotFixtures
    {
	LvmSnapshotFixtures() : GeneralSnapshotFixtures(f_snapper) {}

	virtual GeneralSnapshotCtorFixture ctor_fixture() const;
	virtual GeneralGetImportPolicyFixture get_import_policy_fixture() const;
	virtual GeneralGetSnapshotDirFixture get_snapshot_dir_fixture() const;
	virtual GeneralMountFilesystemFixture mount_filesystem_fixture() const;
    };

    
}}
#endif // LVM_SNAPSHOT_FIXTURES_H
