#include <iostream>
#include <unistd.h>
#include <vector>
#include "cpuinfo.h"

using namespace std;

int main()
{
	CPUInfo info;
	cout << info.getHertz() << endl;
	cout << info.toString() << endl;

	vector<char> filter;
	filter.push_back(CPUInfo::FILTER_USAGE);
	for (int i = 0; i < 5; ++i)
	{
		sleep(1);
		info.update();
		cout << info.toString(filter) << endl;
	}
	return 0;
}
