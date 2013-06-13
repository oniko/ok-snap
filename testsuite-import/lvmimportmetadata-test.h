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
	//void tc_import_copy_ctor();
	void tc_import_equal_method_true();
	void tc_import_equal_method_false();
    };

    struct FLvmImportConstructor : public GeneralFixture, LvmImportConstructor
    {
	virtual void test_method();
    };

/*
    struct FCopyConstructor : public GeneralFixture, CopyConstructor
    {
	virtual void test_method();
    };
*/

    struct FEqualMethodTrue : public GeneralFixture, EqualMethodTrue
    {
	virtual void test_method();
    };

    struct FEqualMethodFalse : public GeneralFixture, EqualMethodFalse
    {
	virtual void test_method();
    };
}}
#endif //LVMIMPORTMETADATA_TEST_H
