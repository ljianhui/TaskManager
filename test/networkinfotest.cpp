#include <unistd.h>
#include <iostream>
#include "networkinfo.h"

using namespace std;

int main()
{
	NetworkInfo info;
	for (int i = 0; i < 10; ++i)
	{
		cout << info.toString() << endl;
		sleep(1);
		info.update();
	}
	return 0;
}
