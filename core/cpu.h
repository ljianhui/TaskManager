#ifndef _CPU_H
#define _CPU_H

class CPU
{
	friend class CPUInfo;

	public:
		CPU();

		inline int getId()const;
		inline unsigned long long getUserTime()const;
		inline unsigned long long getNiceTime()const;
		inline unsigned long long getSystemTime()const;
		inline unsigned long long getIdleTime()const;
		inline unsigned long long getIowaitTime()const;
		inline unsigned long long getIrqTime()const;
		inline unsigned long long getSoftirqTime()const;
		inline unsigned long long getTotalTime()const;
		inline float getUsage()const;

	private: // function
		CPU(int id, unsigned long long userTime,
			unsigned long long niceTime, 
			unsigned long long systemTime,
			unsigned long long idleTime,
			unsigned long long iowaitTime,
			unsigned long long irqTime,
			unsigned long long softirqTime);

		inline void setUsage(float usage);

	private: // data
		int mId;
		unsigned long long mUserTime;
		unsigned long long mNiceTime;
		unsigned long long mSystemTime;
		unsigned long long mIdleTime;
		unsigned long long mIowaitTime;
		unsigned long long mIrqTime;
		unsigned long long mSoftirqTime;
		float mUsage;
};

void CPU::setUsage(float usage)
{
	mUsage = usage;
}

int CPU::getId()const
{
	return mId;
}

unsigned long long CPU::getUserTime()const
{
	return mUserTime;
}

unsigned long long CPU::getNiceTime()const
{
	return mNiceTime;
}

unsigned long long CPU::getSystemTime()const
{
	return mSystemTime;
}

unsigned long long CPU::getIdleTime()const
{
	return mIdleTime;
}

unsigned long long CPU::getIowaitTime()const
{
	return mIowaitTime;
}

unsigned long long CPU::getIrqTime()const
{
	return mIrqTime;
}

unsigned long long CPU::getSoftirqTime()const
{
	return mSoftirqTime;
}

unsigned long long CPU::getTotalTime()const
{
	return mUserTime + mNiceTime + mSystemTime + mIdleTime +
		mIowaitTime + mIrqTime + mSoftirqTime;
}

float CPU::getUsage()const
{
	return mUsage;
}

#endif

