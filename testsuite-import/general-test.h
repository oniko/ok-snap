#ifndef GENERAL_TEST_H
#define GENERAL_TEST_H

namespace testsuiteimport {

    struct GeneralFixture
    {
	virtual ~GeneralFixture() {}

	virtual void test_method() = 0;
    };

}
#endif
