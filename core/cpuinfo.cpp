#include <unistd.h>
#include <string.h>
#include "cpuinfo.h"
#include "filehelper.h"
#include "utils.h"

using std::string;
using std::vector;

const char CPUInfo::FILTER_CPU_COUNT(0);
const char CPUInfo::FILTER_CPU_TYPE(1);
const char CPUInfo::FILTER_USAGE(2);

const string CPUInfo::NAMES[] = {
	"cpu_count",
	"cpu_type",
	"usage"};

unsigned short CPUInfo::sCPUCount(0);
string CPUInfo::sCPUType("");
long CPUInfo::sHertz(0);

CPUInfo::CPUInfo():
	mIterator(0),
	mRecordsVec()
{
	refreshData();
}

CPUInfo::CPUInfo(const CPUInfo &other):
	SystemInfo(other),
	mIterator(0),
	mRecordsVec(other.mRecordsVec)
{
}

CPUInfo& CPUInfo::operator=(const CPUInfo &other)
{
	if (this != &other)
	{
		SystemInfo::operator=(other);
		mIterator = 0;
		mRecordsVec = other.mRecordsVec;
	}
	return *this;
}

CPUInfo::~CPUInfo()
{
	mRecordsVec.clear();
}

unsigned short CPUInfo::getCPUCount()
{
	if (sCPUCount == 0)
	{
		sCPUCount = sysconf(_SC_NPROCESSORS_ONLN);
	}
	return sCPUCount;
}

string CPUInfo::getCPUType()
{
	if (sCPUType == "")
	{
		readCPUType();
	}
	return sCPUType;
}

long CPUInfo::getHertz()
{
	if (sHertz == 0)
	{	
		sHertz = sysconf(_SC_CLK_TCK);
	}
	return sHertz;
}

const CPU* CPUInfo::getCPU(size_t index)const
{
	if (index >= mRecordsVec.size() - 1)
	{
		return NULL;
	}
	return &mRecordsVec[index];
}

const CPU* CPUInfo::getCPUSummary()const
{
	if (mRecordsVec.empty())
	{
		return NULL;
	}
	return &mRecordsVec[mRecordsVec.size()-1];
}

void CPUInfo::update()
{
	refreshData();
}

void CPUInfo::refreshData()
{
	if (sCPUCount == 0)
	{
		sCPUCount = sysconf(_SC_NPROCESSORS_ONLN);
	}

	if (sCPUType == "")
	{
		readCPUType();
	}

	if (sHertz == 0)
	{
		sHertz = sysconf(_SC_CLK_TCK);
	}

	if (mRecordsVec.size() != sCPUCount + 1)
	{
		initVectors(sCPUCount + 1);
	}
	readCPU();
}

void CPUInfo::initVectors(int size)
{
	mRecordsVec.clear();
	mRecordsVec.resize(size);
}

void CPUInfo::readCPUType()
{
	FileHelper file("/proc/cpuinfo", "r");

	char buffer[256] = {0};
	char cpuType[256] = {0};
	bool finished = false;
	int bufSize = sizeof(buffer);

	while (!finished && file.readLine(buffer, bufSize))
	{
		int num = sscanf(buffer, "model name\t: %255[^\n]", cpuType);
		if (num != 0)
			finished = true;
	}
	sCPUType = cpuType;
}

void CPUInfo::readCPU()
{
	FileHelper file("/proc/stat", "r");
	readCPUSummary(file);
	readEachCPU(file);
}

void CPUInfo::readCPUSummary(FileHelper &file)
{
	if (mRecordsVec.empty())
		return;

	char buffer[256] = {0};
	int bufSize = sizeof(buffer);
	unsigned long long userTime = 0; 
	unsigned long long niceTime = 0;
	unsigned long long systemTime = 0;
	unsigned long long idleTime = 0;
	unsigned long long iowaitTime = 0;
	unsigned long long irqTime = 0;
	unsigned long long softirqTime = 0;

	file.readLine(buffer, bufSize);
	sscanf(buffer, "cpu %Lu %Lu %Lu %Lu %Lu %Lu %Lu",
		&userTime, &niceTime, &systemTime, &idleTime,
		&iowaitTime, &irqTime, &softirqTime);
	CPU record(-1, userTime, niceTime, systemTime, idleTime,
		iowaitTime, irqTime, softirqTime);
	float usage = calculateUsage(record, 
		mRecordsVec[mRecordsVec.size() - 1]);
	record.setUsage(usage);
	mRecordsVec[mRecordsVec.size() - 1] = record;
}

void CPUInfo::readEachCPU(FileHelper &file)
{
	if (mRecordsVec.empty())
		return;

	char buffer[256] = {0};
	int bufSize = sizeof(buffer);
	int id = 0;
	unsigned long long userTime = 0; 
	unsigned long long niceTime = 0;
	unsigned long long systemTime = 0;
	unsigned long long idleTime = 0;
	unsigned long long iowaitTime = 0;
	unsigned long long irqTime = 0;
	unsigned long long softirqTime = 0;

	for (int i = 0; i < sCPUCount; ++i)
	{
		file.readLine(buffer, bufSize);
		sscanf(buffer, "cpu%d %Lu %Lu %Lu %Lu %Lu %Lu %Lu",
			&id, &userTime, &niceTime, &systemTime,
			&idleTime, &iowaitTime, &irqTime, &softirqTime);

		CPU record(id, userTime, niceTime, systemTime, 
			idleTime, iowaitTime, irqTime, softirqTime);
		float usage = 
			calculateUsage(record, mRecordsVec[i]);
		record.setUsage(usage);
		mRecordsVec[i] = record;
	}
}

float CPUInfo::calculateUsage(const CPU &newRecord,
	const CPU &oldRecord)
{
	unsigned long long diffTotalTime = newRecord.getTotalTime() -
		oldRecord.getTotalTime();
	unsigned long long diffIdleTime = newRecord.getIdleTime() -
		oldRecord.getIdleTime();

	if (diffTotalTime == 0)
	{
		return oldRecord.getUsage();
	}
	return 1.0 - (float)diffIdleTime / (float)diffTotalTime;
}

string CPUInfo::toString()const
{
	if (mRecordsVec.empty())
		return "";
	string txt;
	int i = (int)FILTER_CPU_COUNT;
	txt += concatNameValue(NAMES[i++], sCPUCount);
	txt += "\n";
	txt += concatNameValue(NAMES[i++], sCPUType);
	txt += "\n";
	txt += usedRatesToString();
	txt += "\n";
	return txt;
}

string CPUInfo::toString(const vector<char> &filter)const
{
	if (mRecordsVec.empty())
		return "";
	
	const int size = 3;
	bool filterFlags[size];
	initFilterFlags(filterFlags, size, filter);
	string txt;
	for (int i = 0; i < size; ++i)
	{
		if (filterFlags[i])
		{
			const string &nameValue = getNameValue((int)i);
			if (nameValue != INVALID_NAME)
			{
				txt += nameValue;
				txt += '\n';
			}
		}
	}
	return txt;
}

string CPUInfo::usedRatesToString()const
{
	string usedRates(NAMES[FILTER_USAGE] + ": ");
	usedRates += concatNameValue("CPU Summary", 
		FloatToPercent(mRecordsVec[mRecordsVec.size() - 1].getUsage()));
	for (size_t i = 0; i < mRecordsVec.size() - 1; ++i)
	{
		usedRates += ", ";
		usedRates += concatNameValue(
			"CPU" + SignedToString(mRecordsVec[i].getId()),
			FloatToPercent(mRecordsVec[i].getUsage()));
	}
	return usedRates;
}

string CPUInfo::getNameValue(int filter)const
{
	switch (filter)
	{
	case FILTER_CPU_COUNT:
		return concatNameValue(NAMES[filter], sCPUCount);
	case FILTER_CPU_TYPE:
		return concatNameValue(NAMES[filter], sCPUType);
	case FILTER_USAGE:
		return usedRatesToString();
	default:;
	}
	return INVALID_NAME;
}

