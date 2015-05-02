#ifndef _PROCESS_H
#define _PROCESS_H

#include <stdio.h>
#include <sys/types.h>
#include <string>

class ProcessInfo;

class Process
{
	friend class ProcessInfo;
	
	public:
		Process();
		Process(pid_t pid);

		inline pid_t getPid()const;
		inline char getState()const;
		inline unsigned long getMemory()const;
		inline unsigned long long getUTime()const;
		inline unsigned long long getSTime()const;
		inline unsigned long getPriority()const;
		inline unsigned long getNice()const;
		inline uid_t getUserId()const;
		inline const std::string& getUserName()const;
		inline const std::string& getProcessName()const;
		inline const std::string& getCmdLine()const;

		inline float getCPUOccupyRate()const;
		inline float getMemoryOccupyRate()const;

	private: // function
		inline void setPid(pid_t pid);
		inline void setCPUOccupyRate(float rate);

		void refreshData();
		
		unsigned long getTotalMemory()const;
		int getPageSize()const;

		std::string pidToDir()const;
		void getInfoFromStat(const std::string &dir);
		void getUserIdName(const std::string &dir);
		void getCmdLine(const std::string &dir);

	private: // data
		pid_t mPid;
		char mState;
		long mPriority;
		long mNice;
		unsigned long mMemory; // unit: bytes
		unsigned long long mUTime;  // unit: jiffies
		unsigned long long mSTime; // unit: jiffies
		float mCPUOccupyRate;
		uid_t mUserId;
		std::string mUserName;
		std::string mProcessName;
		std::string mCmdLine;
};

void Process::setPid(pid_t pid)
{
	mPid = pid;
}

void Process::setCPUOccupyRate(float rate)
{
	mCPUOccupyRate = rate;
}

pid_t Process::getPid()const
{
	return mPid;
}

char Process::getState()const
{
	return mState;
}

unsigned long Process::getMemory()const
{
	return mMemory;
}

unsigned long long Process::getUTime()const
{
	return mUTime;
}

unsigned long long Process::getSTime()const
{
	return mSTime;
}

unsigned long Process::getPriority()const
{
	return mPriority;
}

unsigned long Process::getNice()const
{
	return mNice;
}

uid_t Process::getUserId()const
{
	return mUserId;
}

const std::string& Process::getUserName()const
{
	return mUserName;
}

const std::string& Process::getProcessName()const
{
	return mProcessName;
}

const std::string& Process::getCmdLine()const
{
	return mCmdLine;
}

float Process::getCPUOccupyRate()const
{
	return mCPUOccupyRate;
}

float Process::getMemoryOccupyRate()const
{
	unsigned long totalMemory = getTotalMemory();
	if (totalMemory == 0)
		return 0;
	return (float)mMemory / (float)totalMemory;
}

#endif

