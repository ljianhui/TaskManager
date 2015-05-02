#ifndef _PROCESSMANAGER_H
#define _PROCESSMANAGER_H

#include <sys/types.h>
#include "singleton.h"

class Process;

class ProcessManager : public Singleton<ProcessManager>
{
	friend class Singleton<ProcessManager>;

	public:
		bool stop(pid_t pid)const;
		bool stop(const Process &p)const;
		
		bool resume(pid_t pid)const;
		bool resume(const Process &p)const;

		bool terminate(pid_t pid)const;
		bool terminate(const Process &p)const;

		bool forceStop(pid_t pid)const;
		bool forceStop(const Process &p)const;

		bool setNice(pid_t pid, int nice)const;
		bool setNice(const Process &p, int nice)const;

	private: // function
		ProcessManager(){}
		ProcessManager(const ProcessManager &other){}
		ProcessManager& operator=(
			const ProcessManager &other)
		{ return *this; }
		virtual ~ProcessManager(){}

		bool sendSignal(pid_t pid, int signal)const;
};

#endif

