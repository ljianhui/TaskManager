#include "filesystem.h"

FileSystem::FileSystem():
	mIsMounted(false),
	mDeviceName(),
	mMountDir(),
	mMountType(),
	mUuid(),
	mLabel(),
	mBlockSize(0),
	mBlockCount(0),
	mBlockFree(0),
	mMaxFileNameLen(0)
{
}


