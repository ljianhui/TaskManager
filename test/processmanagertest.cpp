#include <stdio.h>
#include <string.h>
#include "processmanager.h"

using namespace std;

int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		return 0;
	}

	ProcessManager *pm = ProcessManager::getInstance();
	pid_t pid = atoi(argv[2]);
	if (strcmp(argv[1], "stop") == 0)
	{
		if (pm->stop(pid))
		{
			printf("stop: %d\n", pid);
		}
	}
	else if (strcmp(argv[1], "resume") == 0)
	{
		if (pm->resume(pid))
		{
			printf("resume: %d\n", pid);
		}
	}
	else if (strcmp(argv[1], "term") == 0)
	{
		if (pm->terminate(pid))
		{
			printf("term: %d\n", pid);
		}
	}
	else if (strcmp(argv[1], "forcestop") == 0)
	{
		if (pm->forceStop(pid))
		{
			printf("forceStop: %d\n", pid);
		}
	}
	else if (strcmp(argv[1], "nice")  == 0 && argc > 3)
	{
		int nice = atoi(argv[3]);
		if (pm->setNice(pid, nice))
		{
			printf("set nice:%d, %d\n", pid, nice);
		}
	}
	pm->releaseInstance();
	return 0;
}

