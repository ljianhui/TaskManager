#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <algorithm>
#include "log.h"
#include "systeminfo.h"
#include "filehelper.h"
#include "utils.h"

using std::vector;
using std::map;
using std::string;

Log::FileFilter::FileFilter():
	fileName(),
	filter()
{
}

Log::FileFilter::FileFilter(const string &fname,
	const vector<char> &filterVec):
	fileName(fname),
	filter(filterVec)
{
}

Log::Log():
	mInfoFilterMap()
{
}

Log::Log(const Log &other):
	mInfoFilterMap(other.mInfoFilterMap)
{
}

Log& Log::operator=(const Log &other)
{
	return *this;
}

Log::~Log()
{
	mInfoFilterMap.clear();
}

void Log::registerSystemInfo(const SystemInfo *info,
	const string &fileName, const vector<char> &filter)
{
	FileFilter fileFilter(fileName, filter);
	mInfoFilterMap[info] = fileFilter;
}

void Log::unregisterSystemInfo(const SystemInfo *info)
{
	mInfoFilterMap.erase(mInfoFilterMap.find(info));
}

void Log::addFilter(const SystemInfo *info, char filter)
{
	map<const SystemInfo*, FileFilter>::iterator it = 
		mInfoFilterMap.find(info);
	if (it != mInfoFilterMap.end())
	{
		vector<char> &filterVec = it->second.filter;
		vector<char>::iterator filterIt = 
			find(filterVec.begin(), filterVec.end(), filter);
		if (filterIt == filterVec.end())
		{
			filterVec.push_back(filter);
		}
	}
}

void Log::removeFilter(const SystemInfo *info, char filter)
{
	map<const SystemInfo*, FileFilter>::iterator it = 
		mInfoFilterMap.find(info);
	if (it != mInfoFilterMap.end())
	{
		vector<char> &filterVec = it->second.filter;
		vector<char>::iterator filterIt =
			find(filterVec.begin(), filterVec.end(), filter);
		if (filterIt != filterVec.end())
		{
			filterVec.erase(filterIt);
		}
	}
}

void Log::log(const SystemInfo *info)const
{
	log(mInfoFilterMap.find(info), getDir());
}

void Log::log(map<const SystemInfo*, FileFilter>::const_iterator it,
	const string &dir)const
{
	if (it == mInfoFilterMap.end())
		return;

	const FileFilter &fileFilter = it->second;
	string fileName = dir + fileFilter.fileName;
	FileHelper file(fileName.c_str(), "a", "w");
	if (!file.isOpened())
	{
		return;
	}

	file.write(LocalTime() + "\n");
	file.write(it->first->toString(fileFilter.filter));
}

void Log::logAll()const
{
	const string &dir = getDir();
	map<const SystemInfo*, FileFilter>::const_iterator it = 
		mInfoFilterMap.begin();
	for (; it != mInfoFilterMap.end(); ++it)
	{
		log(it, dir);
	}
}

string Log::getDir()const
{
	mkdirLog();
	
	static string dir("");
	if (dir != "" && isDirExist(dir.c_str()))
	{
		return dir;
	}

	time_t curTime = time(NULL);
	struct tm *time = localtime(&curTime);
	char buffer[256] = {0};
	snprintf(buffer, sizeof(buffer) - 1, "./log/%d%02d%02d/",
		time->tm_year + 1900, time->tm_mon + 1, time->tm_mday);

	if (isDirExist(buffer))
	{
		dir = buffer;
		return dir;
	}

	int res = mkdir(buffer,S_IRUSR|S_IWUSR|S_IXUSR|
		S_IRGRP|S_IWGRP|S_IXGRP|S_IROTH|S_IXOTH);
	if (!res)
	{
		dir = buffer;
	}
	return dir;
}

bool Log::isDirExist(const char *dir)const
{
	return access(dir, 0) == 0;
}

void Log::mkdirLog()const
{
	if (!isDirExist("./log/"))
	{
		mkdir("./log/",S_IRUSR|S_IWUSR|S_IXUSR|
			S_IRGRP|S_IWGRP|S_IXGRP|S_IROTH|S_IXOTH);
	}
}
