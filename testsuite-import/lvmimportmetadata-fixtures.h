#ifndef LVMIMPORTMETADATA_FIXTURES_H
#define LVMIMPORTMETADATA_FIXTURES_H

#include <map>
#include <string>

#define private public
    #include "snapper/Lvm.h"
    #include "snapper/LvmImportMetadata.h"
#undef private

namespace testsuiteimport { namespace lvm {

    using std::map;
    using std::string;

    struct GeneralFixture
    {
	virtual ~GeneralFixture() {}

	virtual void test_method() = 0;
    };

    struct ValidMetadata : public GeneralFixture
    {
	ValidMetadata();

	const snapper::Lvm* f_dummy_lvm;
	map<string,string> f_raw_data;

	virtual void test_method();
    };

    struct MissingVgName : public GeneralFixture
    {
	MissingVgName();

	const snapper::Lvm* f_dummy_lvm;
	map<string,string> f_raw_data;

	virtual void test_method();
    };

    struct MissingLvName : public GeneralFixture
    {
	MissingLvName();

	const snapper::Lvm* f_dummy_lvm;
	map<string,string> f_raw_data;

	virtual void test_method();
    };

    struct EqualFalseTestMetadata : public ValidMetadata
    {
	EqualFalseTestMetadata();

	map<string,string> f_raw_data_diff_in_vg;
	map<string,string> f_raw_data_diff_in_lv;
	map<string,string> f_raw_data_different;

	virtual void test_method();
    };

    struct CopyConstructor : public ValidMetadata
    {
	CopyConstructor();

	const snapper::LvmImportMetadata f_origin;

	virtual void test_method();
    };

    struct EqualMethodTrue : public CopyConstructor
    {
	EqualMethodTrue();

	const map<string,string> f_raw_data_copy;
	const snapper::LvmImportMetadata f_origin_copy;

	virtual void test_method();
    };

    struct EqualMethodFalse : public EqualFalseTestMetadata, CopyConstructor
    {
    private:
	const snapper::Lvm* dummy_addr;
    public:
	EqualMethodFalse();

	const snapper::LvmImportMetadata f_data_diff_in_vg;
	const snapper::LvmImportMetadata f_data_diff_in_lv;
	const snapper::LvmImportMetadata f_data_different;

	virtual void test_method();
    };

}}
#endif //LVMIMPORTMETADATA_FIXTURES_H
