#include <unistd.h>
#include <sys/sysinfo.h>
#include <stdio.h>
#include <pwd.h>
#include "process.h"
#include "filehelper.h"

using std::string;

Process::Process():
	mPid(0),
	mState('?'),
	mPriority(0),
	mNice(0),
	mMemory(0),
	mUTime(0),
	mSTime(0),
	mCPUOccupyRate(0.0),
	mUserName(),
	mProcessName(),
	mCmdLine()
{
}

Process::Process(pid_t pid):
	mPid(pid),
	mState('?'),
	mPriority(0),
	mNice(0),
	mMemory(0),
	mUTime(0),
	mSTime(0),
	mCPUOccupyRate(0.0),
	mUserName(),
	mProcessName(),
	mCmdLine()
{
}

void Process::refreshData()
{
	string dir = pidToDir();
	getInfoFromStat(dir);
	getUserIdName(dir);
	getCmdLine(dir);
}

string Process::pidToDir()const
{
	char dir[128] = {0};
	snprintf(dir, sizeof(dir) - 1, "/proc/%d/", mPid);
	return string(dir);
}

void Process::getInfoFromStat(const string &dir)
{
	string fileName = dir + "stat";
	FileHelper file(fileName.c_str(), "r");
	if (!file.isOpened())
		return;

	char processName[128] = {0};
	int tmpN = 0;
	long tmpL = 0;
	unsigned long tmpUL = 0;
	unsigned long long tmpULL = 0;

	fscanf(file.filePtr(),
		"%d (%127[^)]) %c " // 3 1-3
		"%d %d %d %d %d " // ignore 5 4-8
		"%lu %lu %lu %lu %lu " // ignore 5 9-13
		"%Lu %Lu " // 2 14-15
		"%Lu %Lu " // ignore 2 16-17
		"%ld %ld " // 2 18-19
		"%d %ld %Lu %lu " // ignore 4 20-23
		"%lu ", // 1 24
		&tmpN, processName, &mState, // 3 1-3
		&tmpN, &tmpN, &tmpN, &tmpN, &tmpN, // ignore 5, 4-8
		&tmpUL, &tmpUL, &tmpUL, &tmpUL, &tmpUL, // ignore 5, 9-13
		&mUTime, &mSTime, // 2 14-15
		&tmpULL, &tmpULL, // 2 16-17
		&mPriority, &mNice, // 2 18-19
		&tmpN, &tmpL, &tmpULL, &tmpUL, // 4 20-23
		&mMemory); // 1 24
	
	mProcessName = processName;
	mMemory *= getPageSize();
}

void Process::getUserIdName(const string &dir)
{
	string fileName = dir + "status";
	FileHelper file(fileName.c_str(), "r");
	if (!file.isOpened())
		return;

	char buffer[512] = {0};

	while (file.readLine(buffer, sizeof(buffer) - 1))
	{
		uid_t uid = 0;
		uid_t tmp = 0;
		int res = sscanf(buffer, "Uid: %u%u%u", &uid, &tmp, &tmp);
		if (res == 3)
		{
			mUserId = uid;
			struct passwd *pwd = getpwuid(uid);
			if (pwd != NULL)
			{
				mUserName = pwd->pw_name;
			}
		}
	}
}

void Process::getCmdLine(const std::string &dir)
{
	string fileName = dir + "cmdline";
	FileHelper file(fileName.c_str(), "r");
	if (!file.isOpened())
		return;
	
	char buffer[512] = {0};

	fscanf(file.filePtr(), "%511s", buffer);
	mCmdLine = buffer;
}

int Process::getPageSize()const
{
	static int pageSize = 0;
	if (pageSize == 0)
	{
		pageSize = getpagesize();
	}
	return pageSize;
}

unsigned long Process::getTotalMemory()const
{
	static unsigned long totalMemory = 0;
	if (totalMemory == 0)
	{
		struct sysinfo info;
		int res = sysinfo(&info);
		if (!res)
		{
			totalMemory = info.totalram;
		}
	}
	return totalMemory;
}

