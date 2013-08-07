#ifndef SNAPSHOTS_TEST_H
#define SNAPSHOTS_TEST_H


namespace testsuite { namespace lvm
{
    class ImportCheckerTestClass
    {
	void tc_check_valid();
	void tc_check_origin();
	void tc_check_existing_snapshot();
    };

}}
#endif //SNAPSHOTS_TEST_H
