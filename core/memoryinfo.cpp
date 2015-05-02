#include <unistd.h>
#include <sys/sysinfo.h>
#include "memoryinfo.h"
#include "utils.h"

using std::string;
using std::vector;

const char MemoryInfo::FILTER_TOTAL_RAM(0);
const char MemoryInfo::FILTER_FREE_RAM(1);
const char MemoryInfo::FILTER_RAM_USAGE(2);
const char MemoryInfo::FILTER_TOTAL_SWAP(3);
const char MemoryInfo::FILTER_FREE_SWAP(4);
const char MemoryInfo::FILTER_SWAP_USAGE(5);
const char MemoryInfo::FILTER_PAGE_SIZE(6);

const string MemoryInfo::NAMES[] = {
	"total_ram",
	"free_ram",
	"ram_usage",
	"total_swap",
	"free_swap",
	"swap_usage",
	"page_size"};

unsigned long MemoryInfo::sTotalRAM(0);
unsigned long MemoryInfo::sTotalSwap(0);
int MemoryInfo::sPageSize(0);

MemoryInfo::MemoryInfo():
	mFreeRAM(0),
	mFreeSwap(0)
{
	refreshData();
}

MemoryInfo::MemoryInfo(const MemoryInfo &other):
	SystemInfo(other),
	mFreeRAM(other.mFreeRAM),
	mFreeSwap(other.mFreeSwap)
{
}

MemoryInfo& MemoryInfo::operator=(const MemoryInfo &other)
{
	if (this != &other)
	{
		SystemInfo::operator=(other);
		mFreeRAM = other.mFreeRAM;
		mFreeSwap = other.mFreeSwap;
	}
	return *this;
}

MemoryInfo::~MemoryInfo()
{
}

unsigned long MemoryInfo::getTotalRAM()
{
	if (sTotalRAM == 0)
	{
		getTotalRamSwap();
	}
	return sTotalRAM;
}

void MemoryInfo::getTotalRamSwap()
{
	struct sysinfo info;
	int res = sysinfo(&info);
	if (!res)
	{	
		sTotalRAM = info.totalram;
		sTotalSwap = info.totalswap;
	}
}

unsigned long MemoryInfo::getTotalSwap()
{
	if (sTotalSwap == 0)
	{
		getTotalRamSwap();
	}
	return sTotalSwap;
}

int MemoryInfo::getPageSize()
{
	if (sPageSize == 0)
	{
		sPageSize = getpagesize();
	}
	return sPageSize;
}

void MemoryInfo::update()
{
	refreshData();
}

void MemoryInfo::refreshData()
{
	struct sysinfo info;
	int res = sysinfo(&info);
	if (!res)
	{	
		sTotalRAM = info.totalram;
		mFreeRAM = info.freeram;
		sTotalSwap = info.totalswap;
		mFreeSwap = info.freeswap;
	}

	if (sPageSize == 0)
	{
		sPageSize = getpagesize();
	}
}

string MemoryInfo::toString()const
{
	string txt;
	int i = (int)FILTER_TOTAL_RAM;
	txt += (concatNameValue(NAMES[i++], sTotalRAM) + ", ");
	txt += (concatNameValue(NAMES[i++], mFreeRAM) + ", ");
	txt += (concatNameValue(NAMES[i++], sTotalSwap) + ", ");
	txt += (concatNameValue(NAMES[i++], mFreeSwap) + ", ");
	txt += (concatNameValue(NAMES[i++], sPageSize) +"\n");
	return txt;
}

string MemoryInfo::toString(const vector<char> &filter)const
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

string MemoryInfo::getPairByName(int filter)const
{
	switch (filter)
	{
	case FILTER_TOTAL_RAM:
		return concatNameValue(NAMES[filter], sTotalRAM);
	case FILTER_FREE_RAM:
		return concatNameValue(NAMES[filter], mFreeRAM);
	case FILTER_RAM_USAGE:
		return concatNameValue(NAMES[filter], FloatToPercent(getRAMUsage()));
	case FILTER_TOTAL_SWAP:
		return concatNameValue(NAMES[filter], sTotalSwap);
	case FILTER_FREE_SWAP:
		return concatNameValue(NAMES[filter], mFreeSwap);
	case FILTER_SWAP_USAGE:
		return concatNameValue(NAMES[filter], FloatToPercent(getSwapUsage()));
	case FILTER_PAGE_SIZE:
		return concatNameValue(NAMES[filter], sPageSize);
	default:;
	}
	return INVALID_NAME;
}

