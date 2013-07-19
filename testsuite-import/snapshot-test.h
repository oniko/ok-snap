#ifndef SNAPSHOT_TEST_H
#define SNAPSHOT_TEST_H

#include "snapper/Snapper.h"
#include "snapper/ImportMetadata.h"

#include "testsuite-import/general-test.h"
#include "testsuite-import/general-fixtures.h"
//#include "testsuite-import/snapshot-fixtures.h"


namespace testsuiteimport
{    
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
	string infos_dir;

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

	const ::snapper::Snapper* snapper;
    };


    class SnapshotTestClass
    {
    public:
	SnapshotTestClass(const GeneralSnapshotFixtures* fixtures) : fixtures(fixtures) {}

	void tc_snapshot_ctor();
	void tc_snapshot_get_import_policy();
	void tc_snapshot_get_snapshot_dir();
	void tc_snapshot_mount_filesystem();

    private:
	const GeneralSnapshotFixtures* fixtures;
    };


    struct FSnapshotCtor : public GeneralFixture
    {
	FSnapshotCtor(const GeneralSnapshotCtorFixture& gfix);
	~FSnapshotCtor();

	const GeneralSnapshotCtorFixture f;

	const snapper::SnapshotType f_type;
	const unsigned int f_num;
	const time_t f_date;

	bool f_ctor_clone_passed;
	bool f_ctor_adopt_passed;
	bool f_ctor_ack_passed;

	virtual void test_method();
    };


    struct FSnapshotGetImportPolicy : public GeneralFixture
    {
	FSnapshotGetImportPolicy(const GeneralGetImportPolicyFixture& gfix);
	const GeneralGetImportPolicyFixture f;

	const ::snapper::Snapshot f_sh_none;
	const ::snapper::Snapshot f_sh_clone;
	const ::snapper::Snapshot f_sh_adopt;
	const ::snapper::Snapshot f_sh_ack;

	virtual void test_method();
    };


    struct FSnapshotGetSnapshotDir : public GeneralFixture
    {
	FSnapshotGetSnapshotDir(const GeneralGetSnapshotDirFixture& gfix);
	const GeneralGetSnapshotDirFixture f;

	const ::snapper::Snapshot f_sh_none;
	const ::snapper::Snapshot f_sh_clone;
	const ::snapper::Snapshot f_sh_adopt;
	const ::snapper::Snapshot f_sh_ack;

	virtual void test_method();
    };


    struct FSnapshotMountFilesystem : public GeneralFixture
    {
	FSnapshotMountFilesystem(const GeneralMountFilesystemFixture& gfix);
	~FSnapshotMountFilesystem();

	virtual void test_method();

	const GeneralMountFilesystemFixture f;

	const InfoDirWithSnapshotDir f_info_none;
	const InfoDirWithSnapshotDir f_info_none_user;

	const InfoDirWithSnapshotDir f_info_clone;
	const InfoDirWithSnapshotDir f_info_clone_user;

	const InfoDirWithSnapshotDir f_info_adopt;
	const InfoDirWithSnapshotDir f_info_adopt_user;

	const InfoDirWithSnapshotDir f_info_ack;
	const InfoDirWithSnapshotDir f_info_ack_user;

	const ::snapper::Snapshot f_sh_none;
	const ::snapper::Snapshot f_sh_none_user;

	const ::snapper::Snapshot f_sh_clone;
	const ::snapper::Snapshot f_sh_clone_user;

	const ::snapper::Snapshot f_sh_adopt;
	const ::snapper::Snapshot f_sh_adopt_user;

	const ::snapper::Snapshot f_sh_ack;
	const ::snapper::Snapshot f_sh_ack_user;
    };

}
#endif // SNAPSHOT_TEST_H
