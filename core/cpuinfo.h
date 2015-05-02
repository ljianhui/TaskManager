#ifndef _CPUINFO_H
#define _CPUINFO_H

#include <vector>
#include <string>
#include "systeminfo.h"
#include "cpu.h"

class FileHelper;

class CPUInfo : public SystemInfo
{
	public: // function
		CPUInfo();
		CPUInfo(const CPUInfo &other);
		CPUInfo& operator=(const CPUInfo &other);
		virtual ~CPUInfo();

		static unsigned short getCPUCount();
		static std::string getCPUType();
		static long getHertz();

		const CPU* getCPU(size_t index)const;
		const CPU* getCPUSummary()const;

		inline void begin()const;
		inline const CPU* next()const;
		inline bool hasNext()const;

		virtual void update();
		virtual std::string toString() const;
		virtual std::string toString(
			const std::vector<char> &filter) const;

	public: // data
		const static char FILTER_CPU_COUNT;
		const static char FILTER_CPU_TYPE;
		const static char FILTER_USAGE;

	private: // function
		static void readCPUType();
		void refreshData();
		void initVectors(int size);
		void readCPU();
		void readCPUSummary(FileHelper &file);
		void readEachCPU(FileHelper &file);
		float calculateUsage(const CPU &newRecord,
			const CPU &oldRecord);
		std::string usedRatesToString()const;
		std::string getNameValue(int filter)const;

	private: // data
		const static std::string NAMES[];

		static unsigned short sCPUCount;
		static std::string sCPUType;
		static long sHertz;

	private: // data
		mutable size_t mIterator;
		std::vector<CPU> mRecordsVec;
};

void CPUInfo::begin()const
{
	mIterator = 0;
}

const CPU* CPUInfo::next()const
{
	if (mIterator < mRecordsVec.size() - 1)
	{
		return &mRecordsVec[mIterator++];
	}
	return NULL;
}

bool CPUInfo::hasNext()const
{
	return mIterator < (mRecordsVec.size() - 1);
}

#endif

