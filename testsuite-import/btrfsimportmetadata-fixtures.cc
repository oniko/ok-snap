#include <fcntl.h>
#include <sys/stat.h>

#include <sstream>

#include "testsuite-import/btrfsimportmetadata-fixtures.h"

namespace testsuiteimport { namespace btrfs
{
    ValidMetadata::ValidMetadata()
	: BtrfsGeneralFixture()
    {
	int rootfd = open(f_conf_root_volume.c_str(), O_RDONLY | O_CLOEXEC | O_NOATIME);
	if (rootfd < 0)
	{
	    throw std::exception;
	}

	unsigned int count = 0;

	std::ostringstream oss;
	oss << f_subv_prefix << count;
	
	struct stat buf;

	while (fstatat(rootfd, oss.str().c_str(), &buf, AT_SYMLINK_NOFOLLOW) && count < 100)
	{
	    count++;

	    if (errno != ENOENT)
	    {
		// throw something
	    }

	    oss.str(f_subv_prefix);
	    oss.clear();
	    oss << count;
	}
    }
}}
