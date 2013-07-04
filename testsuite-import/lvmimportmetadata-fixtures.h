#ifndef LVMIMPORTMETADATA_FIXTURES_H
#define LVMIMPORTMETADATA_FIXTURES_H

#include <map>
#include <string>

#include "snapper/Lvm.h"
#include "snapper/LvmImportMetadata.h"


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


    struct LvmImportConstructor : public ValidMetadata
    {
	LvmImportConstructor();

	map<string,string> f_raw_data_missing_vg;
	map<string,string> f_raw_data_missing_lv;
    };


    struct EqualFalseTestMetadata : public ValidMetadata
    {
	EqualFalseTestMetadata();
	virtual ~EqualFalseTestMetadata() {}

	map<string,string> f_raw_data_diff_in_vg;
	map<string,string> f_raw_data_diff_in_lv;
	map<string,string> f_raw_data_different;
    };


    struct CopyConstructor : public ValidMetadata
    {
	CopyConstructor();

	const snapper::LvmImportMetadata f_origin;
    };


    struct EqualMethodTrue : public CopyConstructor
    {
	EqualMethodTrue();
	~EqualMethodTrue();

	map<string,string> f_raw_data_copy;
	snapper::LvmImportMetadata* f_p_origin_copy;
    };


    struct EqualMethodFalse : public EqualFalseTestMetadata
    {
	EqualMethodFalse();

	const snapper::LvmImportMetadata f_origin;

	const snapper::LvmImportMetadata f_data_diff_in_vg;
	const snapper::LvmImportMetadata f_data_diff_in_lv;
	const snapper::LvmImportMetadata f_data_different;
    };
}}
#endif //LVMIMPORTMETADATA_FIXTURES_H
