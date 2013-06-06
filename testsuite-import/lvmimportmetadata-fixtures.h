#ifndef LVMIMPORTMETADATA_FIXTURES_H
#define LVMIMPORTMETADATA_FIXTURES_H

#include <map>
#include <string>

#define private public
    #include "snapper/Lvm.h"
    #include "snapper/LvmImportMetadata.h"
#undef private

#include "testsuite-import/general-test.h"

namespace testsuiteimport { namespace lvm {

    using std::map;
    using std::string;

    struct ValidMetadata
    {
	ValidMetadata();
	virtual ~ValidMetadata() {}

	const snapper::Lvm* f_dummy_lvm;
	map<string,string> f_raw_data;
    };

    struct FLvmImportConstructor : public GeneralFixture, ValidMetadata
    {
	FLvmImportConstructor();

	map<string,string> f_raw_data_missing_vg;
	map<string,string> f_raw_data_missing_lv;

	virtual void test_method();
    };

    struct EqualFalseTestMetadata : public ValidMetadata
    {
	EqualFalseTestMetadata();
	virtual ~EqualFalseTestMetadata() {}

	map<string,string> f_raw_data_diff_in_vg;
	map<string,string> f_raw_data_diff_in_lv;
	map<string,string> f_raw_data_different;
    };

    struct FCopyConstructor : public GeneralFixture, ValidMetadata
    {
	FCopyConstructor();

	const snapper::LvmImportMetadata f_origin;

	virtual void test_method();
    };

    struct FEqualMethodTrue : public FCopyConstructor
    {
	FEqualMethodTrue();
	~FEqualMethodTrue();

	map<string,string> f_raw_data_copy;
	snapper::LvmImportMetadata* f_p_origin_copy;

	virtual void test_method();
    };

    struct FCloneMethod : public FCopyConstructor
    {
	virtual void test_method();
    };

    struct FEqualMethodFalse : public GeneralFixture, EqualFalseTestMetadata
    {
	FEqualMethodFalse();

	const snapper::LvmImportMetadata f_origin;

	const snapper::LvmImportMetadata f_data_diff_in_vg;
	const snapper::LvmImportMetadata f_data_diff_in_lv;
	const snapper::LvmImportMetadata f_data_different;

	virtual void test_method();
    };

}}
#endif //LVMIMPORTMETADATA_FIXTURES_H
