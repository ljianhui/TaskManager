#include <iostream>
#include "memoryinfo.h"
#include "utils.h"

using namespace std;

int main()
{
	MemoryInfo memoryInfo;
	memoryInfo.update();
	vector<char> filter;
	filter.push_back(MemoryInfo::FILTER_TOTAL_RAM);
	filter.push_back(MemoryInfo::FILTER_FREE_RAM);
	filter.push_back(MemoryInfo::FILTER_TOTAL_SWAP);
	filter.push_back(MemoryInfo::FILTER_FREE_SWAP);
	cout << memoryInfo.toString(filter) << endl;

	cout << memoryInfo.toString() << endl;
	cout << LocalTime() << endl;
	return 0;
}
