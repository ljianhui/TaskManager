#ifndef _MEMORYINFO_H
#define _MEMORYINFO_H

#include "systeminfo.h"

class MemoryInfo : public SystemInfo
{
	public: // function
		MemoryInfo();
		MemoryInfo(const MemoryInfo &other);
		MemoryInfo& operator=(const MemoryInfo &other);
		virtual ~MemoryInfo();

		static unsigned long getTotalRAM();
		static unsigned long getTotalSwap();
		static int getPageSize();

		inline unsigned long getFreeRAM()const;
		inline unsigned long getFreeSwap()const;

		inline float getRAMUsage()const;
		inline float getSwapUsage()const;

		virtual void update();
		virtual std::string toString() const;
		virtual std::string toString(const std::vector<char> &filter) const;
	
	public: // data
		const static char FILTER_TOTAL_RAM;
		const static char FILTER_FREE_RAM;
		const static char FILTER_RAM_USAGE;
		const static char FILTER_TOTAL_SWAP;
		const static char FILTER_FREE_SWAP;
		const static char FILTER_SWAP_USAGE;
		const static char FILTER_PAGE_SIZE;

	private: // data
		const static std::string NAMES[];

		static unsigned long sTotalRAM;
		static unsigned long sTotalSwap;
		static int sPageSize;

	private: // function
		std::string getPairByName(int filter)const;
		void refreshData();
		static void getTotalRamSwap();

	private: // data
		unsigned long mFreeRAM;
		unsigned long mFreeSwap;
};

unsigned long MemoryInfo::getFreeRAM()const
{
	return mFreeRAM;
}

unsigned long MemoryInfo::getFreeSwap()const
{
	return mFreeSwap;
}

float MemoryInfo::getRAMUsage()const
{
	unsigned long totalRAM = getTotalRAM();
	if (totalRAM == 0)
	{
		return 0;
	}
	unsigned long usedRAM = totalRAM - getFreeRAM();
	return (float)usedRAM / (float)totalRAM;
}

float MemoryInfo::getSwapUsage()const
{
	unsigned long totalSwap = getTotalSwap();
	if (totalSwap == 0)
	{
		return 0;
	}
	unsigned long usedSwap = totalSwap - getFreeSwap();
	return (float)usedSwap / (float)totalSwap;
}

#endif

