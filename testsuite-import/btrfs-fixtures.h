#ifndef BTRFS_FIXTURES_H
#define BTRFS_FIXTURES_H

#include "snapper/Btrfs.h"

namespace testsuiteimport { namespace btrfs
{
    class DummyBtrfs : public snapper::Btrfs
    {
	DummyBtrfs(const string& root_volume);

	virtual snapper::SDir openSubvolumeDir() const;
    };
}}
#endif // BTRFS_FIXTURES_H
