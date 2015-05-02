#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <signal.h>
#include "processmanager.h"
#include "process.h"

bool ProcessManager::sendSignal(pid_t pid, int signal)const
{
	if (pid <= 0)
	{
		return false;
	}
	return kill(pid, signal) == 0;
}

bool ProcessManager::stop(pid_t pid)const
{
	return sendSignal(pid, SIGSTOP);
}

bool ProcessManager::stop(const Process &p)const
{
	return stop(p.getPid());
}

bool ProcessManager::resume(pid_t pid)const
{
	return sendSignal(pid, SIGCONT);
}

bool ProcessManager::resume(const Process &p)const
{
	return resume(p.getPid());
}

bool ProcessManager::terminate(pid_t pid)const
{
	return sendSignal(pid, SIGTERM);
}

bool ProcessManager::terminate(const Process &p)const
{
	return terminate(p.getPid());
}

bool ProcessManager::forceStop(pid_t pid)const
{
	return sendSignal(pid, SIGKILL);
}

bool ProcessManager::forceStop(const Process &p)const
{
	return forceStop(p.getPid());
}

bool ProcessManager::setNice(pid_t pid, int nice)const
{
	if (pid <= 0 || nice < -20 || nice > 19)
	{
		return false;
	}
	
	const int WHICH = PRIO_PROCESS;
	int who = pid;

	int priority = getpriority(WHICH, who);
	if (priority == -1)
	{
		return false;
	}

	if (setpriority(WHICH, who, nice) < 0)
	{
		return false;
	}
	return true;
}

bool ProcessManager::setNice(const Process &p, int nice)const
{
	return setNice(p.getPid(), nice);
}

