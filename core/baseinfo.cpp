#include <unistd.h>
#include <stdio.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <sys/sysinfo.h>
#include <time.h>
#include "baseinfo.h"
#include "utils.h"

using std::vector;
using std::string;

const char BaseInfo::FILTER_USER_NAME(0);
const char BaseInfo::FILTER_USER_ID(1);
const char BaseInfo::FILTER_SYSTEM_NAME(2);
const char BaseInfo::FILTER_HOST_NAME(3);
const char BaseInfo::FILTER_SYSTEM_RELEASE(4);
const char BaseInfo::FILTER_SYSTEM_VERSION(5);
const char BaseInfo::FILTER_MACHINE_TYPE(6);

const string BaseInfo::NAMES[] = {
	"user_name",
	"user_id",
	"system_name",
	"host_name",
	"system_release",
	"system_version",
	"machine_type"};

BaseInfo::BaseInfo():
	mUserName(),
	mUserId(0),
	mSystemName(),
	mHostName(),
	mSystemRelease(),
	mSystemVersion(),
	mMachineType()
{
	refreshData();
}

BaseInfo::BaseInfo(const BaseInfo &other):
	SystemInfo(other),
	mUserName(other.mUserName),
	mUserId(other.mUserId),
	mSystemName(other.mSystemName),
	mHostName(other.mHostName),
	mSystemRelease(other.mSystemRelease),
	mSystemVersion(other.mSystemVersion),
	mMachineType(other.mMachineType)
{
}

BaseInfo& BaseInfo::operator=(const BaseInfo &other)
{
	if (this != &other)
	{
		SystemInfo::operator=(other);
		mUserName = other.mUserName;
		mUserId = other.mUserId;
		mSystemName = other.mSystemName;
		mHostName = other.mHostName;
		mSystemRelease = other.mSystemRelease;
		mSystemVersion = other.mSystemVersion;
		mMachineType = other.mMachineType;
	}
	return *this;
}

BaseInfo::~BaseInfo()
{
}

time_t BaseInfo::getBootTime()
{
	static time_t bootTime = 0;
	if (bootTime == 0)
	{
		struct sysinfo info;
		int res = sysinfo(&info);
		if (!res)
		{
			bootTime = time(NULL) - info.uptime;
		}
	}
	return bootTime;
}

string BaseInfo::getUserName()const
{
	return mUserName;
}

uid_t BaseInfo::getUserId()const
{
	return mUserId;
}

string BaseInfo::getSystemName()const
{
	return mSystemName;
}

string BaseInfo::getHostName()const
{
	return mHostName;
}

string BaseInfo::getSystemRelease()const
{
	return mSystemRelease;
}

string BaseInfo::getSystemVersion()const
{
	return mSystemVersion;
}

string BaseInfo::getMachineType()const
{
	return mMachineType;
}

void BaseInfo::update()
{
	refreshData();
}

void BaseInfo::refreshData()
{
	char *userName = cuserid(NULL);
	if (userName != NULL)
	{
		mUserName = userName;
	}
	mUserId = getuid();
	struct utsname un;
	int res = uname(&un);
	if (!res)
	{
		mSystemName = un.sysname;
		mHostName = un.nodename;
		mSystemRelease = un.release;
		mSystemVersion = un.version;
		mMachineType = un.machine;
	}
}

string BaseInfo::toString()const
{
	string txt;
	int i = (int)FILTER_USER_NAME;
	txt += (concatNameValue(NAMES[i++], mUserName) + ", ");
	txt += (concatNameValue(NAMES[i++], mUserId) + ", ");
	txt += (concatNameValue(NAMES[i++], mSystemName) + ", ");
	txt += (concatNameValue(NAMES[i++], mHostName) + ", ");
	txt += (concatNameValue(NAMES[i++], mSystemRelease) + ", ");
	txt += (concatNameValue(NAMES[i++], mSystemVersion) + ", ");
	txt += (concatNameValue(NAMES[i++], mMachineType) + "\n");
	return txt;
}

string BaseInfo::toString(const vector<char> &filter)const
{
	const int size = 7;
	bool filterFlags[size];
	initFilterFlags(filterFlags, size, filter);

	string txt;
	bool isFirst = true;
	for (int i = 0; i < size; ++i)
	{   
		if (!filterFlags[i])
		{
			continue;
		}

		if (!isFirst)
		{
			txt += ", ";
		}
		const string &pair(getPairByName(i));
		if (pair != INVALID_NAME)
		{
			txt += pair;
			isFirst = false;
		}
	}
	txt += "\n";
	return txt;
}

string BaseInfo::getPairByName(int filter)const
{
	switch (filter)
	{
	case FILTER_USER_NAME:
		return concatNameValue(NAMES[filter], mUserName);
	case FILTER_USER_ID:
		return concatNameValue(NAMES[filter], mUserId);
	case FILTER_SYSTEM_NAME:
		return concatNameValue(NAMES[filter], mSystemName);
	case FILTER_HOST_NAME:
		return concatNameValue(NAMES[filter], mHostName);
	case FILTER_SYSTEM_RELEASE:
		return concatNameValue(NAMES[filter], mSystemRelease);
	case FILTER_SYSTEM_VERSION:
		return concatNameValue(NAMES[filter], mSystemVersion);
	case FILTER_MACHINE_TYPE:
		return concatNameValue(NAMES[filter], mMachineType);
	default:;
	}
	return INVALID_NAME;
}

