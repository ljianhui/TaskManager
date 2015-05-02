#include <iostream>
#include "baseinfo.h"

using namespace std;

int main()
{
	BaseInfo info;
	info.update();
	cout << info.toString() << endl;
	return 0;
}
