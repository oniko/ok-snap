#ifndef BTRFS_TEST_H
#define BTRFS_TEST_H

#include "testsuite-import/btrfs-fixtures.h"
#include "testsuite-import/general-test.h"

namespace testsuiteimport { namespace btrfs
{
    class BtrfsTestClass
    {
	void tc_btrfs_ctor();
	void tc_btrfs_clone_snapshot();
	void tc_btrfs_check_imported_snapshot();
	void tc_btrfs_delete_imported_snapshot();
    };

    struct FBtrfsConstructor : public GeneralFixture, BtrfsConstructor
    {
	virtual void test_method();
    };

    struct FCloneSnapshot : public GeneralFixture, CloneSnapshot
    {
	virtual void test_method();
    };

    struct FCheckImportedSnapshot : public GeneralFixture
    {
	virtual void test_method();
    };

    struct FDeleteImportedSnapshot : public GeneralFixture
    {
	virtual void test_method();
    };

}}
#endif //BTRFS_TEST_H
