#ifndef _FILESYSTEM_H
#define _FILESYSTEM_H

#include <string>

class FileSystem
{
	friend class FileSystemInfo;

	public:
		FileSystem();

		inline bool isMounted()const;
		inline const std::string& getDeviceName()const;
		inline const std::string& getMountDir()const;
		inline const std::string& getMountType()const;
		inline const std::string& getUuid()const;
		inline const std::string& getLabel()const;
		inline unsigned long getBlockSize()const;
		inline unsigned long getBlockCount()const;
		inline unsigned long getBlockFree()const;
		inline unsigned long getTotalBytes()const;
		inline unsigned long getFreeBytes()const;
		inline unsigned long getMaxFileNameLen()const;
		inline float getUsage()const;

	private: // function
		inline void setIsMounted(bool isMounted);
		inline void setDeviceName(const std::string &deviceName);
		inline void setMountDir(const std::string &mountDir);
		inline void setMountType(const std::string &mountType);
		inline void setUuid(const std::string &uuid);
		inline void setLabel(const std::string &label);
		inline void setBlockSize(unsigned long blockSize);
		inline void setBlockCount(unsigned long blockCount);
		inline void setBlockFree(unsigned long blockFree);
		inline void setMaxFileNameLen(unsigned long maxFileNameLen);
		
	private: // data
		bool mIsMounted;
		std::string mDeviceName;
		std::string mMountDir;
		std::string mMountType;
		std::string mUuid;
		std::string mLabel;
		unsigned long mBlockSize;
		unsigned long mBlockCount;
		unsigned long mBlockFree;
		unsigned long mMaxFileNameLen;
};

void FileSystem::setIsMounted(bool isMounted)
{
	mIsMounted = isMounted;
}

void FileSystem::setDeviceName(const std::string &deviceName)
{
	mDeviceName = deviceName;
}

void FileSystem::setMountDir(const std::string &mountDir)
{
	mMountDir = mountDir;
}

void FileSystem::setMountType(const std::string & mountType)
{
	mMountType = mountType;
}

void FileSystem::setUuid(const std::string &uuid)
{
	mUuid = uuid;
}

void FileSystem::setLabel(const std::string &label)
{
	mLabel = label;
}

void FileSystem::setBlockSize(unsigned long blockSize)
{
	mBlockSize = blockSize;
}

void FileSystem::setBlockCount(unsigned long blockCount)
{
	mBlockCount = blockCount;
}

void FileSystem::setBlockFree(unsigned long blockFree)
{
	mBlockFree = blockFree;
}

void FileSystem::setMaxFileNameLen(unsigned long maxFileNameLen)
{
	mMaxFileNameLen = maxFileNameLen;
}

bool FileSystem::isMounted()const
{
	return mIsMounted;
}

const std::string& FileSystem::getDeviceName()const
{
	return mDeviceName;
}

const std::string& FileSystem::getMountDir()const
{
	return mMountDir;
}

const std::string& FileSystem::getMountType()const
{
	return mMountType;
}

const std::string& FileSystem::getUuid()const
{
	return mUuid;
}

const std::string& FileSystem::getLabel()const
{
	return mLabel;
}

unsigned long FileSystem::getBlockSize()const
{
	return mBlockSize;
}

unsigned long FileSystem::getBlockCount()const
{
	return mBlockCount;
}

unsigned long FileSystem::getBlockFree()const
{
	return mBlockFree;
}

unsigned long FileSystem::getTotalBytes()const
{
	return mBlockSize * mBlockCount;
}

unsigned long FileSystem::getFreeBytes()const
{
	return mBlockSize * mBlockFree;
}

unsigned long FileSystem::getMaxFileNameLen()const
{
	return mMaxFileNameLen;
}

float FileSystem::getUsage()const
{
	if (mBlockCount == 0)
	{
		return 1.0;
	}
	return 1.0 - (float)mBlockFree / (float)mBlockCount;
}

#endif

