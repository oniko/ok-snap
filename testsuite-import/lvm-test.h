#ifndef LVM_TEST_H
#define LVM_TEST_H

#include "testsuite-import/general-test.h"
#include "testsuite-import/lvm-fixtures.h"

namespace testsuiteimport { namespace lvm
{
    class LvmTestClass
    {
    public:
	void tc_create_snapshot_environment();
 	void tc_create_snapshot_fail_on_environment();
	void tc_remove_snapshot_environment();
	void tc_clone_snapshot();
	void tc_mount_snapshot_by_subvolume();
	void tc_check_imported_snapshot();
	void tc_lvm_delete_snapshot_by_vg_lv();
    };


    struct FCreateSnapshotEnvironment : public GeneralFixture, CreateSnapshotEnvironment
    {
	virtual void test_method();
    };


    struct FCreateSnapshotFailOnEnvironment : public GeneralFixture, CreateSnapshotFailOnEnvironment
    {
	virtual void test_method();
    };


    struct FRemoveSnapshotEnvironment : public GeneralFixture, RemoveSnapshotEnvironment
    {
	virtual void test_method();
    };


    struct FCloneSnapshot : public GeneralFixture, LvmCloneSnapshot
    {
	virtual void test_method();
    };


    struct FMountSnapshotBySubvolume : public GeneralFixture, LvmMountSnapshotBySubvolume
    {
	virtual void test_method();
    };


    struct FCheckImportedSnapshot : public GeneralFixture, LvmCheckImportedSnapshot
    {
	virtual void test_method();
    };


    struct FDeleteSnapshotByVgLv : public GeneralFixture, DeleteSnapshotByVgLv
    {
	virtual void test_method();
    };

}}
#endif
