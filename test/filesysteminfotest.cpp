#include <iostream>
#include <vector>
#include "filesysteminfo.h"

using namespace std;

int main()
{
	FileSystemInfo info;
	vector<char> filter;
	filter.push_back(FileSystemInfo::FILTER_DEVICE_NAME);
	filter.push_back(FileSystemInfo::FILTER_MOUNT_DIR);
	filter.push_back(FileSystemInfo::FILTER_MOUNT_TYPE);
	filter.push_back(FileSystemInfo::FILTER_BLOCK_SIZE);
	filter.push_back(FileSystemInfo::FILTER_BLOCK_COUNT);
	filter.push_back(FileSystemInfo::FILTER_BLOCK_FREE);
	filter.push_back(FileSystemInfo::FILTER_TOTAL_BYTES);
	filter.push_back(FileSystemInfo::FILTER_FREE_BYTES);
	filter.push_back(FileSystemInfo::FILTER_USAGE);
	cout << info.toString(filter) << endl;

	cout << info.toString() << endl;
	return 0;
}
