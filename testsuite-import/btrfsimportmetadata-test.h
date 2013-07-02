#ifndef BTRFSIMPORTMETADATA_TEST_H
#define BTRFSIMPORTMETADATA_TEST_H


#include "testsuite-import/general-test.h"
#include "testsuite-import/btrfsimportmetadata-fixtures.h"

namespace testsuiteimport { namespace btrfs
{
    class BtrfsImportMetadataTestClass
    {
    public:
	BtrfsImportMetadataTestClass() {}

	void tc_import_ctor();

	// NOTE: depends on:	- ctor
	void tc_import_compare_metadata();

	/*
	 * NOTE: depends on:
	 *			- ctor
	 * 			- Btrfs::checkImportedSnapshot()
	 */
	void tc_import_check_imported_snapshot();

	// NOTE: test not needed right now. simply calls Btrfs::cloneSnapshot()
	//void tc_import_clone_snapshot();

	/*
	 * NOTE: depends on:
	 * 			- ctor
	 * 			- Btrfs::deleteImportedSnapshot()
	 */
	void tc_import_delete_snapshot();

	// NOTE: depends on:	- ctor
	void tc_import_get_raw_metadata();

	// NOTE: depends on:	- ctor
	void tc_import_get_snapshot_dir();
    };

    struct FBtrfsImportConstructor : public GeneralFixture, BtrfsImportConstructor
    {
	virtual void test_method();
    };

    struct FImportCompareMetadata : public GeneralFixture, CompareImportMetadata
    {
	virtual void test_method();
    };

    struct FImportCheckImportedMetadata : public GeneralFixture, CheckImportMetadata
    {
	virtual void test_method();
    };

    struct FGetSnapshotDir : public GeneralFixture, GetSnapshotDir
    {
	virtual void test_method();
    };

    struct FGetRawData : public GeneralFixture, GetRawMetadata
    {
	virtual void test_method();
    };

    struct FDeleteImportedSnapshot : public GeneralFixture, DeleteImportedSnapshot
    {
	virtual void test_method();
    };

}}
#endif //BTRFSIMPORTMETADATA_TEST_H
