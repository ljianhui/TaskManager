#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/statvfs.h>
#include <mntent.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

#include "filesysteminfo.h"
#include "filehelper.h"
#include "utils.h"

using std::vector;
using std::list;
using std::map;
using std::string;

const char FileSystemInfo::FILTER_IS_MOUNTED(0);
const char FileSystemInfo::FILTER_DEVICE_NAME(1);
const char FileSystemInfo::FILTER_MOUNT_DIR(2);
const char FileSystemInfo::FILTER_MOUNT_TYPE(3);
const char FileSystemInfo::FILTER_BLOCK_SIZE(4);
const char FileSystemInfo::FILTER_BLOCK_COUNT(5);
const char FileSystemInfo::FILTER_BLOCK_FREE(6);
const char FileSystemInfo::FILTER_TOTAL_BYTES(7);
const char FileSystemInfo::FILTER_FREE_BYTES(8);
const char FileSystemInfo::FILTER_USAGE(9);
const char FileSystemInfo::FILTER_LABEL(10);
const char FileSystemInfo::FILTER_UUID(11);

const string FileSystemInfo::NAMES[] = {
	"mounted\t",
	"device\t\t",
	"dir\t",
	"type\t",
	"bsize\t",
	"bcount\t",
	"bfree\t",
	"total\t",
	"free\t",
	"usage\t",
	"lable\t",
	"UUID\t\t"
};

FileSystemInfo::FileSystemInfo():
	mHasMountChange(false),
	mMountFSList(),
	mUnmountFSList()
{
	refreshData();
}

FileSystemInfo::FileSystemInfo(const FileSystemInfo &other):
	SystemInfo(other),
	mHasMountChange(other.mHasMountChange),
	mMountFSList(other.mMountFSList),
	mUnmountFSList(other.mUnmountFSList)
{
}

FileSystemInfo& FileSystemInfo::operator=(const FileSystemInfo &other)
{
	if (this != &other)
	{
		SystemInfo::operator=(other);
		mHasMountChange = other.mHasMountChange;
		mMountFSList = other.mMountFSList;
		mUnmountFSList = other.mUnmountFSList;
	}
	return *this;
}

FileSystemInfo::~FileSystemInfo()
{
	mMountFSList.clear();
	mUnmountFSList.clear();
}

void FileSystemInfo::update()
{
	refreshData();
}

void FileSystemInfo::refreshData()
{

	size_t mountSize = mMountFSList.size();
	size_t unmountSize = mUnmountFSList.size();

	map<string, string> deviceUuidMap;
	map<string, string> deviceLabelMap;
	list<string> swapList;
	initDeviceUuidMap(deviceUuidMap);
	initDeviceLabelMap(deviceLabelMap);
	initSwapList(swapList);

	refreshMountFSList(deviceUuidMap, deviceLabelMap);
	refreshUnmountFSList(deviceUuidMap, deviceLabelMap, swapList);

	if (mountSize != mMountFSList.size()
		|| unmountSize != mUnmountFSList.size())
	{
		mHasMountChange = true;
	}
	else
	{
		mHasMountChange = false;
	}
}

void FileSystemInfo::initDeviceUuidMap(
	map<string, string> &deviceUuidMap)
{
	const string uuidDir("/dev/disk/by-uuid/");
	initMapByDir(uuidDir, deviceUuidMap);
}

void FileSystemInfo::initMapByDir(
	const string &dirName, map<string, string> &keyValueMap)
{
	DIR *dir = opendir(dirName.c_str());
	if (dir == NULL)
	{
		return;
	}

	keyValueMap.clear();
	const string dev("/dev");
	struct dirent *valueDir = NULL;
	struct stat st;
	while ((valueDir = readdir(dir)) != NULL)
	{
		const string fileName = dirName + valueDir->d_name;
		lstat(fileName.c_str(), &st);
		if (S_ISLNK(st.st_mode))
		{
			char buffer[128] = {0};
			ssize_t size = readlink(fileName.c_str(), 
				buffer, sizeof(buffer));
			if (size > 0)
			{
				string key(dev + strrchr(buffer, '/'));
				string value(valueDir->d_name);
				keyValueMap[key] = value;
			}
		}
	}
	closedir(dir);
}

void FileSystemInfo::initDeviceLabelMap(
	map<string, string> &deviceLabelMap)
{
	string labelDir("/dev/disk/by-label/");
	initMapByDir(labelDir, deviceLabelMap);
}

void FileSystemInfo::initSwapList(list<string> &swapList)
{
	FileHelper file("/proc/swaps", "r");
	if (!file.isOpened())
	{
		return;
	}

	char buffer[256] = {0};
	while (file.readLine(buffer, sizeof(buffer) - 1))
	{
		char device[128] = {0};
		char type[128] = {0};
		long tmpL = 0;
		int num = sscanf(buffer, "%s%s%ld%ld%ld", 
			device, type, &tmpL, &tmpL, &tmpL);
		if (num == 5)
		{
			swapList.push_back(string(device));
		}
	}
}

bool FileSystemInfo::isSwap(const string &deviceName,
	const list<string> &swapList)
{
	for (list<string>::const_iterator it = swapList.begin();
		it != swapList.end(); ++it)
	{
		if (*it == deviceName)
		{
			return true;
		}
	}
	return false;
}

bool FileSystemInfo::isMounted(const string &deviceName)
{
	for (list<FileSystem>::const_iterator it = mMountFSList.begin();
		it != mMountFSList.end(); ++it)
	{
		if (it->getDeviceName() == deviceName)
		{
			return true;
		}
	}
	return false;
}

void FileSystemInfo::refreshMountFSList(
	map<string, string> &deviceUuidMap,
	map<string, string> &deviceLabelMap)
{
	FILE *file = setmntent("/etc/mtab", "r");
	if (file == NULL)
	{
		return;
	}

	mMountFSList.clear();

	struct mntent *mnt = NULL;
	struct statvfs vfs;
	FileSystem fs;
	while ((mnt = getmntent(file)))
	{
		if (isDevice(mnt->mnt_fsname))
		{
			if (!statvfs(mnt->mnt_dir, &vfs))
			{
				fs.setIsMounted(true);
				fs.setDeviceName(mnt->mnt_fsname);
				fs.setUuid(deviceUuidMap[fs.getDeviceName()]);
				fs.setLabel(deviceLabelMap[fs.getDeviceName()]);
				fs.setMountDir(mnt->mnt_dir);
				fs.setMountType(mnt->mnt_type);
				fs.setBlockSize(vfs.f_bsize);
				fs.setBlockCount(vfs.f_blocks);
				fs.setBlockFree(vfs.f_bfree);
				fs.setMaxFileNameLen(vfs.f_namemax);

				mMountFSList.push_back(fs);
			}
		}
	}
	endmntent(file);
}

bool FileSystemInfo::isDevice(const char *fileSysName)const
{
	struct stat st;
	int res = stat(fileSysName, &st);
	if (!res)
	{
		if (S_ISBLK(st.st_mode))
		{
			return true;
		}
	}
	return false;
}

void FileSystemInfo::refreshUnmountFSList(
	map<string, string> &deviceUuidMap,
	map<string, string> &deviceLabelMap,
	list<string> &swapList)
{
	FileHelper file("/proc/partitions", "r");
	if (!file.isOpened())
	{
		return;
	}

	mUnmountFSList.clear();
	int major = 0;
	int minor = 0;
	unsigned long blocks = 0;
	char device[128] = {0};
	char buffer[128] = {0};
	FileSystem fs;

	const string dev("/dev/");
	while (file.readLine(buffer, sizeof(buffer) - 1))
	{
		int num = sscanf(buffer, "%d %d %lu %s",
			&major, &minor, &blocks, device);
		if (num == 4)
		{
			string devName = dev + device;
			map<string, string>::iterator it =
				deviceUuidMap.find(devName);
			if (!isSwap(devName, swapList) &&
				it != deviceUuidMap.end() && !isMounted(devName))
			{
				fs.setIsMounted(false);
				fs.setDeviceName(devName);
				fs.setUuid(it->second);
				fs.setLabel(deviceLabelMap[devName]);
				fs.setBlockSize(1024);
				fs.setBlockCount(blocks);

				mUnmountFSList.push_back(fs);
			}
		}
	}
}

string FileSystemInfo::toString()const
{
	vector<char> filter(12, 0);
	for (size_t i = 0; i < filter.size(); ++i)
	{
		filter[i] = i;
	}

	return toString(filter);
}

string FileSystemInfo::toString(const vector<char> &filter)const
{
	const int size = 12;
	bool filterFlags[size];
	initFilterFlags(filterFlags, size, filter);

	string txt(makeTableHeader(filterFlags, size));
	for (list<FileSystem>::const_iterator it = mMountFSList.begin();
		it != mMountFSList.end(); ++it)
	{
		txt += makeTableItem(*it, filterFlags, size);
	}

	for (list<FileSystem>::const_iterator it = mUnmountFSList.begin();
		it != mUnmountFSList.end(); ++it)
	{
		txt += makeTableItem(*it, filterFlags, size);
	}
	txt += "\n";
	return txt;
}

string FileSystemInfo::makeTableHeader(
	const bool *filterFlags, int size)const
{
	string header;
	for (int i = 0; i < size; ++i)
	{
		if (filterFlags[i])
		{
			header += NAMES[i];
		}
	}
	header += "\n";
	return header;
}

string FileSystemInfo::makeTableItem(const FileSystem &fs,
	const bool *filterFlags, int size)const
{
	string item;
	for (int i = 0; i < size; ++i)
	{
		if (filterFlags[i])
		{
			const string &value = getValue(fs, i);
			if (value != INVALID_NAME)
			{
				item += value;
			}
		}
	}
	item += "\n";
	return item;
}

string FileSystemInfo::getValue(const FileSystem &fs, 
	int filter)const
{
	switch (filter)
	{
	case FILTER_IS_MOUNTED:
		return fs.isMounted() ? "Yes\t" : "No\t";
	case FILTER_DEVICE_NAME:
		return fs.getDeviceName() + "\t";
	case FILTER_MOUNT_DIR:
		return fs.getMountDir() + "\t";
	case FILTER_MOUNT_TYPE:
		return fs.getMountType() + "\t";
	case FILTER_BLOCK_SIZE:
		return BytesToString(fs.getBlockSize()) + "\t";
	case FILTER_BLOCK_COUNT:
		return UnsignedToString(fs.getBlockCount()) + "\t";
	case FILTER_BLOCK_FREE:
		return UnsignedToString(fs.getBlockFree()) + "\t";
	case FILTER_USAGE:
		return FloatToPercent(fs.getUsage()) + "\t";
	case FILTER_TOTAL_BYTES:
		return BytesToString(fs.getTotalBytes()) + "\t";
	case FILTER_FREE_BYTES:
		return BytesToString(fs.getFreeBytes()) + "\t";
	case FILTER_LABEL:
		return fs.getLabel() == "" ?
			"?\t" : fs.getLabel() + "\t";
	case FILTER_UUID:
		return fs.getUuid() + "\t";
	default:;
	}
	return INVALID_NAME;
}

