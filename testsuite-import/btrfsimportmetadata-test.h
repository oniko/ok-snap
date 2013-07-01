#ifndef BTRFSIMPORTMETADATA_TEST_H
#define BTRFSIMPORTMETADATA_TEST_H


#include "testsuite-import/general-test.h"
#include "testsuite-import/btrfsimportmetadata-fixtures.h"

namespace testsuiteimport { namespace btrfs
{
    class BtrfsImportMetadataTest
    {
    public:
	BtrfsImportMetadataTest() {}

	void tc_import_ctor();

	// NOTE: depends on ctor
	void tc_import_compare_metadata();

	//void tc_import_compare_num_true();;

	// NOTE: depends on Btrfs.checkImportedSnapshot()
	void tc_import_check_imported_snapshot();

	// voit tc_import_clone_snapshot();
	// void tc_import_delete_snapshot();

	void tc_import_get_raw_metadata();

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

    struct FGetSnapshotDir : public GeneralFixture
    {
	virtual void test_method();
    };

}}
#endif //BTRFSIMPORTMETADATA_TEST_H
