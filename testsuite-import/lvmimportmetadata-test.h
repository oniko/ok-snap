#ifndef LVMIMPORTMETADATA_TEST_H
#define LVMIMPORTMETADATA_TEST_H

#include "testsuite-import/general-test.h"
#include "lvmimportmetadata-fixtures.h"

namespace testsuiteimport { namespace lvm
{
    class LvmImportMetadataTestClass
    {
    public:
	LvmImportMetadataTestClass() {}

	void tc_import_ctor();

	void tc_import_compare_metadata();

	// repeat fs test
	void tc_import_check_imported_snapshot();
	
	// trivial test
	void tc_clone_imported_snapshot();

	// trivial test
	void tc_delete_imported_snapshot();

	void tc_import_raw_metadata();
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


}}
#endif //LVMIMPORTMETADATA_TEST_H
