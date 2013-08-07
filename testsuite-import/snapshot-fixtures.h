#ifndef SNAPSHOT_FIXTURES_H
#define SNAPSHOT_FIXTURES_H

#include "snapper/ImportMetadata.h"

#include "testsuite-import/general-fixtures.h"

namespace testsuiteimport
{
    using std::string;

    struct GeneralSnapshotCtorFixture
    {
	GeneralSnapshotCtorFixture(const ::snapper::Snapper* snapper) : snapper(snapper) {}

	const ::snapper::Snapper* snapper;

	::snapper::ImportMetadata* clone_data;
	::snapper::ImportMetadata* adopt_data;
	::snapper::ImportMetadata* ack_data;
    };


    struct GeneralGetImportPolicyFixture : public GeneralSnapshotCtorFixture
    {
	GeneralGetImportPolicyFixture(const ::snapper::Snapper* snapper)
	    : GeneralSnapshotCtorFixture(snapper) {}
    };
    
    struct GeneralGetSnapshotDirFixture : public GeneralSnapshotCtorFixture
    {
	GeneralGetSnapshotDirFixture(const ::snapper::Snapper* snapper)
	    : GeneralSnapshotCtorFixture(snapper) {}

	unsigned int num_none;
	unsigned int num_clone;
	unsigned int num_adopt;
	unsigned int num_ack;

	string expected_none;
	string expected_clone;
	string expected_adopt;
	string expected_ack;
    };
    
    
    struct GeneralMountFilesystemFixture
    {
	GeneralMountFilesystemFixture(const ::snapper::Snapper* snapper)
	    : snapper(snapper) {}

	const ::snapper::Snapper* snapper;

	SubvolumeWrapper* subvol_none;
	SubvolumeWrapper* subvol_none_user;

	SubvolumeWrapper* subvol_clone_orig;
	SubvolumeWrapper* subvol_clone;
	SubvolumeWrapper* subvol_clone_orig_user;
	SubvolumeWrapper* subvol_clone_user;

	SubvolumeWrapper* subvol_adopt;
	SubvolumeWrapper* subvol_adopt_user;

	SubvolumeWrapper* subvol_ack;
	SubvolumeWrapper* subvol_ack_user;

	/*
	 * NOTE: following memory will be freed by Snapshot dtor.
	 * 	 Do not pass pter to more than one Snapshot ctor!
	 */
	::snapper::ImportMetadata* im_clone;
	::snapper::ImportMetadata* im_clone_user;

	::snapper::ImportMetadata* im_adopt;
	::snapper::ImportMetadata* im_adopt_user;

	::snapper::ImportMetadata* im_ack;
	::snapper::ImportMetadata* im_ack_user;
    };


    class GeneralSnapshotFixtures
    {
    public:
	GeneralSnapshotFixtures(const snapper::Snapper* snapper) : snapper(snapper) {}
	virtual ~GeneralSnapshotFixtures() {}

	virtual GeneralSnapshotCtorFixture ctor_fixture() const = 0;
	virtual GeneralGetImportPolicyFixture get_import_policy_fixture() const = 0;
	virtual GeneralGetSnapshotDirFixture get_snapshot_dir_fixture() const = 0;
	virtual GeneralMountFilesystemFixture mount_filesystem_fixture() const = 0;

    protected:
	const ::snapper::Snapper* snapper;
    };

//     struct DeleteFilesystemSnapshotImportTypeNone : public InfoDirWithSnapshotDir
//     {
// 	DeleteFilesystemSnapshotImportTypeNone();
// 	~DeleteFilesystemSnapshotImportTypeNone();
// 
// 	const snapper::Snapshot f_sh;
// 	const string f_snapshot_lv_name;
//     };
// 
//     struct DeleteFilesystemSnapshotImportTypeClone : public InfoDirWithSnapshotDir
//     {
// 	DeleteFilesystemSnapshotImportTypeClone();
// 	~DeleteFilesystemSnapshotImportTypeClone();
// 
// 	const string f_snapshot_lv_name;
// 	const CreateRawLvmImportMetata rm;
// 
// 	const snapper::ImportMetadata* f_p_idata;
// 	const snapper::Snapshot f_sh;
//     };
// 
//     struct DeleteFilesystemSnapshotImportTypeAdopt : public InfoDirWithSnapshotDir
//     {
// 	DeleteFilesystemSnapshotImportTypeAdopt();
// 	~DeleteFilesystemSnapshotImportTypeAdopt();
// 
// 	const string f_snapshot_lv_name;
// 	const CreateRawLvmImportMetata rm;
// 
// 	const snapper::ImportMetadata* f_p_idata;
// 	const snapper::Snapshot f_sh;
//     };
// 
//     struct DeleteFilesystemSnapshotImportTypeAcknowledge : public InfoDirWithSnapshotDir
//     {
// 	DeleteFilesystemSnapshotImportTypeAcknowledge();
// 	~DeleteFilesystemSnapshotImportTypeAcknowledge();
// 
// 	const string f_snapshot_lv_name;
// 	const CreateRawLvmImportMetata rm;
// 
// 	const snapper::ImportMetadata* f_p_idata;
// 	const snapper::Snapshot f_sh;
//     };
// 
//     struct DeleteFileSystemSnapshotOrigin : public LvmGeneralFixture
//     {
// 	DeleteFileSystemSnapshotOrigin();
// 	~DeleteFileSystemSnapshotOrigin() {}
// 
// 	const snapper::Snapshot f_sh;
//     };
}
#endif //SNAPSHOT_FIXTURES_H
