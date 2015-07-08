#ifndef _PROCESSINFO_H
#define _PROCESSINFO_H

#include <sys/types.h>
#include <vector>
#include <map>
#include "systeminfo.h"
#include "process.h"

typedef bool (*ProcessComparator)(const Process *x, const Process *y);

class ProcessInfo : public SystemInfo
{
	public:
		ProcessInfo();
		ProcessInfo(const ProcessInfo &other);
		ProcessInfo& operator=(const ProcessInfo &other);
		virtual ~ProcessInfo();

		inline void setToStringProcessCount(size_t count);
		inline void setUserIdFilter(uid_t uid);
		inline size_t getProcessCount()const;
		inline const Process* getProcess(size_t index)const;
		inline const std::vector<Process*>& getProcesses()const;

		inline void begin()const;
		inline Process* next()const;
		inline bool hasNext()const;

		void sortByProcessName();
		void sortByUserName();
		void sortByPriority();
		void sortByPid();
		void sortByMemory();
		void sortByCPUOccupyRate();

		virtual void update();
		virtual std::string toString()const;
		virtual std::string toString(
			const std::vector<char> &filter)const;
	
	public: // data
		const static char FILTER_PID;
		const static char FILTER_PROCESS_NAME;
		const static char FILTER_STATE;
		const static char FILTER_MEMORY;
		const static char FILTER_MEM_OCCUPY_RATE;
		const static char FILTER_CPU_OCCUPY_RATE;
		const static char FILTER_USER_NAME;
		const static char FILTER_PRIORITY;
		const static char FILTER_NICE;
		const static char FILTER_CMDLINE;

	private: // data
		const static std::string NAMES[];
		const static size_t CACHESIZE;
	private: // struct
		struct HistoryTime
		{
			HistoryTime();
			unsigned long long time;
			unsigned long long seqNum;
		};
		
	private: // function
		void refreshData();
		void depthCopyProcessVec(const std::vector<Process*> &srcVec);
		void resizeProcessVec(size_t size);
		Process* popFromCache();
		void pushToCache(Process *p);
		void clearPtrVec(std::vector<Process*> &ptrVec);

		bool addToProcessVec(pid_t pid, size_t i, double curTime);
		void calculateCPUOccRate(Process *p, double curTime);
		void clearClosedProcessHistory();
		
		void sortProcesses(ProcessComparator comparator);
		double getCurrentTime();

		std::string makeTableHeader(
			const bool *filterFlags, int size)const;
		std::string makeTableItem(const Process *process,
			const bool *filterFlags, int size)const;
		std::string getItemValue(const Process *process, 
			int filter)const;

	private: // data
		size_t mToStringCount;
		ProcessComparator mComparator;
		uid_t mUserIdFilter;
		double mRecordTime;
		mutable size_t mIterator;
		unsigned long long mSeqNum;
		std::vector<Process*> mProcessVec;
		std::vector<Process*> mCacheVec;
		std::map<pid_t, HistoryTime> mHistoryMap;
};

void ProcessInfo::setToStringProcessCount(size_t count)
{
	mToStringCount = count;
}

void ProcessInfo::setUserIdFilter(uid_t uid)
{
	mUserIdFilter = uid;
}

size_t ProcessInfo::getProcessCount()const
{
	return mProcessVec.size();
}

const Process* ProcessInfo::getProcess(size_t index)const
{
	if (index >= mProcessVec.size())
	{
		return NULL;
	}
	return mProcessVec[index];
}

const std::vector<Process*>& ProcessInfo::getProcesses()const
{
	return mProcessVec;
}

void ProcessInfo::begin()const
{
	mIterator = 0;
}

Process* ProcessInfo::next()const
{
	if (mIterator >= mProcessVec.size())
	{
		return NULL;
	}
	return mProcessVec[mIterator++];
}

bool ProcessInfo::hasNext()const
{
	return mIterator < mProcessVec.size();
}

#endif

