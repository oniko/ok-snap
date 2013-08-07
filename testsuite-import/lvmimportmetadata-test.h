#ifndef LVMIMPORTMETADATA_TEST_H
#define LVMIMPORTMETADATA_TEST_H

#include <ostream>

#include "testsuite-import/general-test.h"
#include "lvmimportmetadata-fixtures.h"

namespace snapper
{
    std::ostream& operator<<(std::ostream& out, const snapper::LvmImportMetadata& idata);
}

namespace testsuiteimport { namespace lvm
{
    class LvmImportMetadataTestClass
    {
    public:
	LvmImportMetadataTestClass() {}

	void tc_import_ctor();

	/*
	 * NOTE: depends on:
	 *		- ctor
	 */
	void tc_import_compare_metadata();

	/*
	 * NOTE: depends on:
	 *		- ctor
	 *		- Lvm::checkImportdSnapshot()
	 */
	void tc_import_check_imported_snapshot();
	
	/*
	 * NOTE: depends on:
	 *		- ctor
	 *		- Lvm::cloneSnapshot()
	 */
	void tc_clone_imported_snapshot();

	/*
	 * NOTE: depends on:
	 *		- ctor
	 *		- Lvm::deleteSnapshot()
	 */
	void tc_delete_imported_snapshot();

	/*
	 * NOTE: depends on:
	 *		- ctor
	 */
	void tc_import_raw_metadata();

	/*
	 * NOTE: depends on:
	 *		- ctor
	 *		- Lvm::snapshotDir()
	 */
	void tc_import_get_snapshot_dir();
    };


    struct FLvmImportConstructor : public GeneralFixture, LvmImportConstructor
    {
	virtual void test_method();
    };


    struct FLvmCompareCheck : public GeneralFixture, LvmCompareImportMetadata
    {
	virtual void test_method();
    };


    struct FLvmIMDataCheckImportedSnapshot : public GeneralFixture, CheckImportedSnapshot
    {
	virtual void test_method();
    };


    struct FCloneImportedSnapshot : public GeneralFixture, CloneSnapshot
    {
	virtual void test_method();
    };


    struct FDeleteImportedSnapshot : public GeneralFixture, CloneSnapshot
    {
	virtual void test_method();
    };


    struct FGetRawData : public GeneralFixture, GetRawData
    {
	virtual void test_method();
    };


    struct FGetSnapshotDir : public GeneralFixture, GetSnapshotDir
    {
	virtual void test_method();
    };

    
}}
#endif //LVMIMPORTMETADATA_TEST_H
