#ifndef SNAPSHOT_TEST_H
#define SNAPSHOT_TEST_H

#include "testsuite-import/general-test.h"
#include "testsuite-import/snapshot-fixtures.h"

namespace testsuiteimport
{
    class SnapshotTestClass
    {
    public:
	SnapshotTestClass(const GeneralSnapshotFixtures* fixtures) : fixtures(fixtures) {}
	~SnapshotTestClass() { delete fixtures; }

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

	const GeneralMountFilesystemFixture f;

	const ::snapper::Snapshot f_sh_none;
	const ::snapper::Snapshot f_sh_none_user;

	const ::snapper::Snapshot f_sh_clone;
	const ::snapper::Snapshot f_sh_clone_user;

	const ::snapper::Snapshot f_sh_adopt;
	const ::snapper::Snapshot f_sh_adopt_user;

	const ::snapper::Snapshot f_sh_ack;
	const ::snapper::Snapshot f_sh_ack_user;

	virtual void test_method();
    };

}
#endif // SNAPSHOT_TEST_H
