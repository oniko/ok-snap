#include "testsuite-import/general-fixtures.h"

namespace testsuiteimport {

    const string LvmGeneralFixture::f_snapshots_prefix =  "/testsuite-import/.snapshots/";

    LvmGlobalConfig::LvmGlobalConfig()
    {
	snapper::Snapper::createConfig("testsuite-import", "/testsuite-import", "lvm(ext4)", "default");
    }

    LvmGlobalConfig::~LvmGlobalConfig()
    {
	snapper::Snapper::deleteConfig("testsuite-import");
    }

    LvmGeneralFixture::LvmGeneralFixture()
    {
	f_sh = new snapper::Snapper("testsuite-import");
	f_lvm = static_cast<const snapper::Lvm *>(f_sh->getFilesystem());
    }

    LvmGeneralFixture::~LvmGeneralFixture()
    {
	delete f_sh;
    }
}
