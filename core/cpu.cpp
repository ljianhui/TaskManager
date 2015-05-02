#include "cpu.h"

CPU::CPU():
	mId(-1),
	mUserTime(0),
	mNiceTime(0),
	mSystemTime(0),
	mIdleTime(0),
	mIowaitTime(0),
	mIrqTime(0),
	mSoftirqTime(0),
	mUsage(0.0)
{
}

CPU::CPU(int id, unsigned long long userTime,
	unsigned long long niceTime, unsigned long long systemTime,
	unsigned long long idleTime, unsigned long long iowaitTime,
	unsigned long long irqTime, unsigned long long softirqTime):
	mId(id),
	mUserTime(userTime),
	mNiceTime(niceTime),
	mSystemTime(systemTime),
	mIdleTime(idleTime),
	mIowaitTime(iowaitTime),
	mIrqTime(irqTime),
	mSoftirqTime(softirqTime),
	mUsage(0.0)
{
}

