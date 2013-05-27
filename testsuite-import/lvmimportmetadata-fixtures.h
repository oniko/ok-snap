#ifndef LVMIMPORTMETADATA_FIXTURES_H
#define LVMIMPORTMETADATA_FIXTURES_H

#include <map>
#include <string>

#include "snapper/LvmImportMetadata.h"

namespace testsuiteimport { namespace lvm {

    using std::map;
    using std::string;

    struct ValidMetadata
    {
	ValidMetadata();
	~ValidMetadata() {}

	map<string,string> f_raw_data;
    };

    struct MissingVgName
    {
	MissingVgName();
	~MissingVgName() {}

	map<string,string> f_raw_data;
    };

    struct MissingLvName
    {
	MissingLvName();
	~MissingLvName() {}

	map<string,string> f_raw_data;
    };

    struct CopyConstructor : public ValidMetadata
    {
	CopyConstructor();
	~CopyConstructor() {}

	const snapper::LvmImportMetadata f_origin;
    };

    struct EqualMethodTrue : public CopyConstructor
    {
	EqualMethodTrue();
	~EqualMethodTrue() {}

	const map<string,string> f_raw_data_copy;
	const snapper::LvmImportMetadata f_origin_copy;
    };

    struct EqualMethodFalse : public CopyConstructor
    {
	EqualMethodFalse();
	~EqualMethodFalse() {}

	snapper::LvmImportMetadata f_data_diff_in_vg;
	snapper::LvmImportMetadata f_data_diff_in_lv;
	snapper::LvmImportMetadata f_data_different;

	map<string,string> f_raw_data_diff_in_vg;
	map<string,string> f_raw_data_diff_in_lv;
	map<string,string> f_raw_data_different;
    };

}}
#endif //LVMIMPORTMETADATA_FIXTURES_H
