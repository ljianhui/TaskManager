#include "log.h"
#include "baseinfo.h"
#include "memoryinfo.h"
#include "cpuinfo.h"
#include "networkinfo.h"
#include "filesysteminfo.h"
#include "processinfo.h"

int main()
{
	Log *log = Log::getInstance();
	SystemInfo *baseInfo = new BaseInfo();
	SystemInfo *memoryInfo = new MemoryInfo();
	SystemInfo *cpuInfo = new CPUInfo();
	SystemInfo *networkInfo = new NetworkInfo();
	SystemInfo *fileSystemInfo = new FileSystemInfo();
	SystemInfo *processInfo = new ProcessInfo();

	baseInfo->update();
	memoryInfo->update();
	cpuInfo->update();
	networkInfo->update();
	fileSystemInfo->update();
	processInfo->update();
	
	log->registerSystemInfo(baseInfo, "baseinfo.log");
	log->registerSystemInfo(memoryInfo, "memoryinfo.log");
	log->registerSystemInfo(cpuInfo, "cpuinfo.log");
	log->registerSystemInfo(networkInfo, "networkinfo.log");
	log->registerSystemInfo(fileSystemInfo, "filesysteminfo.log");
	log->registerSystemInfo(processInfo, "processinfo.log");

	log->logAll();

	log->releaseInstance();
	return 0;
}
