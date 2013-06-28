#ifndef BTRFSIMPORTMETADATA_TEST_H
#define BTRFSIMPORTMETADATA_TEST_H

namespace testsuiteimport { namespace btrfs
{
    class BtrfsImportMetadataTest
    {
    public:
	BtrfsImportMetadataTest() {}

	void tc_import_ctor();

	void tc_import_compare_metadata_true();
	void tc_import_compare_metadata_false();

	void tc_import_compare_num_true();
	void tc_import_compare_num_false();

	void tc_import_check_imported_snapshot();

	// voit tc_import_clone_snapshot();
	// void tc_import_delete_snapshot();

	void tc_import_get_raw_metadata();

	// void tc_import_get_snapshot_dir();
    };
	
}}
#endif //BTRFSIMPORTMETADATA_TEST_H
