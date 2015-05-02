#ifndef _FILESYSTEMINFO_H
#define _FILESYSTEMINFO_H

#include <list>
#include <map>
#include <string>

#include "systeminfo.h"
#include "filesystem.h"

struct statvfs;
struct mntent;

class FileSystemInfo : public SystemInfo
{
	public:
		FileSystemInfo();
		FileSystemInfo(const FileSystemInfo &other);
		FileSystemInfo& operator=(const FileSystemInfo &other);
		virtual ~FileSystemInfo();

		inline bool hasMountChange()const;
		inline const std::list<FileSystem>&
			getMountFileSystems() const;
		inline const std::list<FileSystem>&
			getUnmountFileSystems()const;

		inline size_t getMountFileSystemCount()const;
		inline size_t getUnmountFileSystemCount()const;

		inline void beginMountFileSystems()const;
		inline void beginUnMountFileSystems()const;
		inline bool hasNextMountFileSystem()const;
		inline bool hasNextUnmountFileSystem()const;
		inline const FileSystem* nextMountFileSystem()const;
		inline const FileSystem* nextUnmountFileSystem()const;

		virtual void update();
		virtual std::string toString()const;
		virtual std::string toString(
			const std::vector<char> &filter)const;

	public: // data
		const static char FILTER_IS_MOUNTED;
		const static char FILTER_DEVICE_NAME;
		const static char FILTER_MOUNT_DIR;
		const static char FILTER_MOUNT_TYPE;
		const static char FILTER_BLOCK_SIZE;
		const static char FILTER_BLOCK_COUNT;
		const static char FILTER_BLOCK_FREE;
		const static char FILTER_TOTAL_BYTES;
		const static char FILTER_FREE_BYTES;
		const static char FILTER_USAGE;
		const static char FILTER_LABEL;
		const static char FILTER_UUID;

	private: // data
		const static std::string NAMES[];

	private: // function
		void refreshData();
		void refreshMountFSList(
			std::map<std::string, std::string> &deviceUuidMap,
			std::map<std::string, std::string> &deviceLabelMap);
		void refreshUnmountFSList(
			std::map<std::string, std::string> &deviceUuidMap,
			std::map<std::string, std::string> &deviceLabelMap,
			std::list<std::string> &swapList);

		bool isDevice(const char *sysName)const;
		void initFileSystem(const struct mntent *mnt,
			const struct statvfs *vfs, FileSystem *stat);
		void initDeviceUuidMap(
			std::map<std::string, std::string> &deviceUuidMap);
		void initDeviceLabelMap(
			std::map<std::string, std::string> &deviceLabelMap);
		void initMapByDir(const std::string &dirName,
			std::map<std::string, std::string> &keyValueMap);
		void initSwapList(std::list<std::string> &swapList);
		bool isSwap(const std::string &deviceName,
			const std::list<std::string> &swapList);
		bool isMounted(const std::string &deviceName);

		std::string makeTableHeader(
			const bool *filterFlags, int size)const;
		std::string makeTableItem(const FileSystem &fs,
			const bool *filterFlags, int size)const;
		std::string getValue(const FileSystem &fs, int filter)const;

	private: // data
		bool mHasMountChange;
		std::list<FileSystem> mMountFSList;
		std::list<FileSystem> mUnmountFSList;
		mutable std::list<FileSystem>::const_iterator mMountIterator;
		mutable std::list<FileSystem>::const_iterator mUnMountIterator;
};

bool FileSystemInfo::hasMountChange()const
{
	return mHasMountChange;
}

const std::list<FileSystem>&
FileSystemInfo::getMountFileSystems()const
{
	return mMountFSList;
}

const std::list<FileSystem>&
FileSystemInfo::getUnmountFileSystems()const
{
	return mUnmountFSList;
}

size_t FileSystemInfo::getMountFileSystemCount()const
{
	return mMountFSList.size();
}

size_t FileSystemInfo::getUnmountFileSystemCount()const
{
	return mUnmountFSList.size();
}

void FileSystemInfo::beginMountFileSystems()const
{
	mMountIterator = mMountFSList.begin();
}

void FileSystemInfo::beginUnMountFileSystems()const
{
	mUnMountIterator = mUnmountFSList.begin();
}

bool FileSystemInfo::hasNextMountFileSystem()const
{
	return mMountIterator != mMountFSList.end();
}

bool FileSystemInfo::hasNextUnmountFileSystem()const
{
	return mUnMountIterator != mUnmountFSList.end();
}

const FileSystem* FileSystemInfo::nextMountFileSystem()const
{
	const FileSystem &fs = *mMountIterator;
	++mMountIterator;
	return &fs;
}

const FileSystem* FileSystemInfo::nextUnmountFileSystem()const
{
	const FileSystem &fs = *mUnMountIterator;
	++mUnMountIterator;
	return &fs;
}

#endif

