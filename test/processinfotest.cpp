#include <unistd.h>
#include <iostream>
#include "processinfo.h"

using namespace std;

int main()
{
	
	/*
	FileHelper file("/proc/4231/stat", "r");
	char processName[128] = {0};
	char buffer[1024] = {0};
	char mState = ' ';
	unsigned long long mCurUTime = 0;
	unsigned long long mCurSTime = 0;
	long mPriority = 0;
	long mNice = 0;
	unsigned long mMemory = 0;

	
	fscanf(file.filePtr(),
		"%*lu (%[^)]) %c "
		"%*d %*d %*d %*d %*d "
		"%*lu %*lu %*lu %*lu %*lu "
		"%Lu %Lu %*Lu %*Lu "
		"%ld %ld "
		"%*d %*ld %*Lu %*lu "
		"%lu ",
		processName, &mState,
		&mCurUTime, &mCurSTime,
		&mPriority, &mNice,
		&mMemory);
	cout << processName << endl;
	cout << mState << endl;
	cout << mCurUTime << endl;
	cout << mCurSTime << endl;
	cout << mPriority << endl;
	cout << mNice << endl;
	cout << mMemory << endl;
	*/

	/*
	string fileName = "/proc/2841/status";
	FileHelper file(fileName.c_str(), "r");
	char buffer[512] = {0};
	string mUserName;

	while (file.readLine(buffer, sizeof(buffer) - 1))
	{
		uid_t uid = 0;
		uid_t tmp = 0;
		int res = sscanf(buffer, "Uid: %u%u%u", &uid, &tmp, &tmp);
		if (res == 3)
		{
			struct passwd *pwd = getpwuid(uid);
			if (pwd != NULL)
			{
				mUserName = pwd->pw_name;
			}
			break;
		}
	}
	cout << mUserName << endl;
	*/

	/*
	string dir = "/proc/2841/";
	string fileName = dir + "cmdline";
	FileHelper file(fileName.c_str(), "r");
	char buffer[512] = {0};

	fscanf(file.filePtr(), "%511s", buffer);
	string mCmdLine = buffer;
	cout << mCmdLine << endl;
	*/

	ProcessInfo info;
	for (;;)
	{
		info.update();
		cout << "process count: " << info.getProcesses().size() << endl;
		cout << info.toString() << endl;
		sleep(1);
	}
	return 0;
}
