#include <dirent.h>
#include <sys/types.h>
#include <sys/time.h>
#include <algorithm>
#include "processinfo.h"
#include "cpuinfo.h"
#include "utils.h"

using std::vector;
using std::map;
using std::string;
using std::sort;

static bool ProcessNameCompare(const Process *x, const Process *y);
static bool UserNameCompare(const Process *x, const Process *y);
static bool PriorityCompare(const Process *x, const Process *y);
static bool PidCompare(const Process *x, const Process *y);
static bool MemoryCompare(const Process *x, const Process *y);
static bool CPUOccupyRateCompare(const Process *x, const Process *y);

const char ProcessInfo::FILTER_PID(0);
const char ProcessInfo::FILTER_PROCESS_NAME(1);
const char ProcessInfo::FILTER_STATE(2);
const char ProcessInfo::FILTER_MEMORY(3);
const char ProcessInfo::FILTER_MEM_OCCUPY_RATE(4);
const char ProcessInfo::FILTER_CPU_OCCUPY_RATE(5);
const char ProcessInfo::FILTER_USER_NAME(6);
const char ProcessInfo::FILTER_PRIORITY(7);
const char ProcessInfo::FILTER_NICE(8);
const char ProcessInfo::FILTER_CMDLINE(9);

const string ProcessInfo::NAMES[] = {
	"pid\t",
	"name\t",
	"state\t",
	"memory\t",
	"%mem\t",
	"%cpu\t",
	"user\t",
	"priority\t",
	"nice\t",
	"cmdline"
};
const size_t ProcessInfo::CACHESIZE(50);

unsigned long long ProcessInfo::HistoryTime::sSeqNum(0);

ProcessInfo::HistoryTime::HistoryTime():
	time(0),
	seqNum(sSeqNum)
{
}

ProcessInfo::ProcessInfo():
	mToStringCount(20),
	mComparator(MemoryCompare),
	mUserIdFilter(-1),
	mRecordTime(0.0),
	mIterator(0),
	mProcessVec(),
	mCacheVec(),
	mHistoryMap()
{
	refreshData();
}

ProcessInfo::ProcessInfo(const ProcessInfo &other):
	SystemInfo(other),
	mToStringCount(other.mToStringCount),
	mComparator(other.mComparator),
	mUserIdFilter(other.mUserIdFilter),
	mRecordTime(other.mRecordTime),
	mIterator(0),
	mProcessVec(other.mProcessVec.size(), NULL),
	mCacheVec(),
	mHistoryMap(other.mHistoryMap)
{
	depthCopyProcessVec(other.mProcessVec);
}

ProcessInfo& ProcessInfo::operator=(const ProcessInfo &other)
{
	if (this != &other)
	{
		SystemInfo::operator=(other);
		mToStringCount = other.mToStringCount;
		mComparator = other.mComparator;
		mUserIdFilter = other.mUserIdFilter;
		mRecordTime = other.mRecordTime;
		mIterator = 0;
		depthCopyProcessVec(other.mProcessVec);
		mHistoryMap = other.mHistoryMap;
	}
	return *this;
}

ProcessInfo::~ProcessInfo()
{
	clearPtrVec(mProcessVec);
	clearPtrVec(mCacheVec);
	mHistoryMap.clear();
}

void ProcessInfo::depthCopyProcessVec(const vector<Process*> &srcVec)
{
	if (mProcessVec.size() != srcVec.size())
	{
		resizeProcessVec(srcVec.size());
	}

	for (size_t i = 0; i < srcVec.size(); ++i)
	{
		if (mProcessVec[i] == NULL)
		{
			mProcessVec[i] = new Process(*srcVec[i]);
		}
		else
		{
			*mProcessVec[i] = *srcVec[i];
		}
	}
}

void ProcessInfo::resizeProcessVec(size_t size)
{
	if (mProcessVec.size() == size)
	{
		return;
	}

	while (mProcessVec.size() > size)
	{
		Process *p = mProcessVec[mProcessVec.size() - 1];
		pushToCache(p);
		mProcessVec.pop_back();
	}

	while (mProcessVec.size() < size)
	{
		Process *p = popFromCache();
		mProcessVec.push_back(p);
	}
}

void ProcessInfo::pushToCache(Process *p)
{
	if (p == NULL)
		return;

	if (mCacheVec.size() <= CACHESIZE)
	{
		mCacheVec.push_back(p);
	}
	else
	{
		delete p;
	}
}

Process* ProcessInfo::popFromCache()
{
	if (mCacheVec.empty())
		return NULL;
	
	Process *p = mCacheVec[mCacheVec.size() - 1];
	mCacheVec.pop_back();
	return p;
}

void ProcessInfo::clearPtrVec(vector<Process*> &ptrVec)
{
	for (vector<Process*>::iterator it = ptrVec.begin();
		it != ptrVec.end(); ++it)
	{
		if (*it != NULL)
		{
			delete *it;
			*it = NULL;
		}
	}
	ptrVec.clear();
}

void ProcessInfo::sortProcesses(ProcessComparator comparator)
{
	if (mComparator == comparator)
		return;

	mComparator = comparator;
	sort(mProcessVec.begin(), mProcessVec.end(), mComparator);
}

void ProcessInfo::sortByProcessName()
{
	sortProcesses(ProcessNameCompare);
}

void ProcessInfo::sortByUserName()
{
	sortProcesses(UserNameCompare);
}

void ProcessInfo::sortByPriority()
{
	sortProcesses(PriorityCompare);
}

void ProcessInfo::sortByPid()
{
	sortProcesses(PidCompare);
}

void ProcessInfo::sortByMemory()
{
	sortProcesses(MemoryCompare);
}

void ProcessInfo::sortByCPUOccupyRate()
{
	sortProcesses(CPUOccupyRateCompare);
}

void ProcessInfo::update()
{
	refreshData();
}

void ProcessInfo::refreshData()
{
	DIR *procDir = opendir("/proc");
	if (procDir == NULL)
		return;

	++HistoryTime::sSeqNum;

	double curTime = getCurrentTime();
	size_t i = 0;
	struct dirent *dir = NULL;
	while ((dir = readdir(procDir)) != NULL)
	{
		long long pid = atoll(dir->d_name);
		if (pid != 0)
		{
			if (addToProcessVec(pid, i, curTime))
			{
				++i;
			}
		}
	}
	closedir(procDir);
	sort(mProcessVec.begin(), mProcessVec.end(), mComparator);
	resizeProcessVec(i);
	clearClosedProcessHistory();
	mRecordTime = curTime;
}

double ProcessInfo::getCurrentTime()
{
	struct timeval timeVal;
	struct timezone timeZone;
	gettimeofday(&timeVal, &timeZone);
	double curTime  = (double)timeVal.tv_sec + 
		(double)timeVal.tv_usec / (double)1e6;
	return curTime;
}

bool ProcessInfo::addToProcessVec(pid_t pid, size_t i,
	double curTime)
{
	if (i >= mProcessVec.size())
	{
		resizeProcessVec(i + 1);
	}

	if (mProcessVec[i] == NULL)
	{
		mProcessVec[i] = new Process(pid);
	}
	else
	{
		mProcessVec[i]->setPid(pid);
	}
	mProcessVec[i]->refreshData();
	
	if (mUserIdFilter == -1 ||
		mProcessVec[i]->getUserId() == mUserIdFilter)
	{
		calculateCPUOccRate(mProcessVec[i], curTime);
		return true;
	}
	return false;
}

void ProcessInfo::calculateCPUOccRate(Process *p, double curTime)
{
	HistoryTime &history = mHistoryMap[p->getPid()];
	unsigned long long cpuTime = p->getUTime() + p->getSTime();
	double diffCpuTime = cpuTime - history.time;
	double totalDiffCpuTime = (double)(CPUInfo::getHertz() * 
		(curTime - mRecordTime) * CPUInfo::getCPUCount());
	double cpuOccRate = (double)(diffCpuTime) / totalDiffCpuTime;
	p->setCPUOccupyRate(cpuOccRate);
	history.time = cpuTime;
	history.seqNum = HistoryTime::sSeqNum;
}

void ProcessInfo::clearClosedProcessHistory()
{
	// if closed processes more than 50, clear it. 
	if (mHistoryMap.size() < mProcessVec.size() + 50)
		return;

	map<pid_t, HistoryTime>::iterator it = mHistoryMap.begin();
	while (it != mHistoryMap.end())
	{
		if (it->second.seqNum != HistoryTime::sSeqNum)
		{
			mHistoryMap.erase(it++);
		}
		else
		{
			++it;
		}
	}
}

string ProcessInfo::toString()const
{
	vector<char> filter(10, 0);
	for (char i = 0; i < 10; ++i)
	{
		filter[i] = i;
	}

	return toString(filter);
}

string ProcessInfo::toString(const vector<char> &filter)const
{
	const int size = 10;
	bool filterFlags[size];
	initFilterFlags(filterFlags, size, filter);

	string txt(makeTableHeader(filterFlags, size));
	for (size_t i = 0; i < mToStringCount && i < mProcessVec.size();
		++i)
	{
		txt += makeTableItem(mProcessVec[i], filterFlags, size);
	}
	txt += "\n";
	return txt;
}

string ProcessInfo::makeTableHeader(const bool *filterFlags, int size)const
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

string ProcessInfo::makeTableItem(const Process *process,
	const bool *filterFlags, int size)const
{
	string item;
	for (int i = 0; i < size; ++i)
	{
		if (filterFlags[i])
		{
			item += getItemValue(process, i);
		}
	}
	item += "\n";
	return item;
}

string ProcessInfo::getItemValue(const Process *process, 
	int filter)const
{
	if (process == NULL)
		return string("\n");

	switch (filter)
	{
	case FILTER_PID:
		return SignedToString(process->getPid()) + "\t";
	case FILTER_PROCESS_NAME:
		return process->getProcessName() + "\t";
	case FILTER_STATE:
		return string(1, process->getState()) + "\t";
	case FILTER_MEMORY:
		return BytesToString(process->getMemory()) + "\t";
	case FILTER_MEM_OCCUPY_RATE:
		return FloatToString(
			process->getMemoryOccupyRate() * 100) + "\t";
	case FILTER_CPU_OCCUPY_RATE:
		return FloatToString(
			process->getCPUOccupyRate() * 100) + "\t";
	case FILTER_USER_NAME:
		return process->getUserName() + "\t";
	case FILTER_PRIORITY:
		return SignedToString(process->getPriority()) + "\t";
	case FILTER_NICE:
		return SignedToString(process->getNice()) + "\t";
	case FILTER_CMDLINE:
		return process->getCmdLine() + "\t";
	}
	return "\t";
}

static bool ProcessNameCompare(const Process *x, const Process *y)
{
	if (!x || !y)
		return x > y;
	return x->getProcessName() < y->getProcessName();
}

static bool UserNameCompare(const Process *x, const Process *y)
{
	if (!x || !y)
		return x > y;
	return x->getUserName() < y->getUserName();
}

static bool PriorityCompare(const Process *x, const Process *y)
{
	if (!x || !y)
		return x > y;
	return x->getPriority() < y->getPriority();
}

static bool PidCompare(const Process *x, const Process *y)
{
	if (!x || !y)
		return x > y;
	return x->getPid() > y->getPid();
}

static bool MemoryCompare(const Process *x, const Process *y)
{
	if (!x || !y)
		return x > y;
	return x->getMemory() > y->getMemory();
}

static bool CPUOccupyRateCompare(const Process *x, const Process *y)
{
	if (!x || !y)
		return x > y;
	return x->getCPUOccupyRate() > y->getCPUOccupyRate();
}

