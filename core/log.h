#ifndef _LOG_H
#define _LOG_H

#include <map>
#include <vector>
#include <string>
#include "singleton.h"

class SystemInfo;

class Log : public Singleton<Log>
{
	friend class Singleton<Log>;

	public:
		void registerSystemInfo(const SystemInfo *info,
			const std::string &logFileName,
			const std::vector<char> &filter = std::vector<char>());
		void unregisterSystemInfo(const SystemInfo *info);
		void addFilter(const SystemInfo *info, char filter);
		void removeFilter(const SystemInfo *info, char filter);
		
		void log(const SystemInfo *info)const;
		void logAll()const;

	private: // struct
		struct FileFilter
		{
			FileFilter();
			FileFilter(const std::string &fname,
				const std::vector<char> &filterVec);
			std::string fileName;
			std::vector<char> filter;
		};

	private: // function
		Log();
		Log(const Log &other);
		Log& operator=(const Log &other);
		~Log();

		void log(std::map<const SystemInfo*, FileFilter>::
			const_iterator it, const std::string &dir)const;
		std::string getDir()const;
		bool isDirExist(const char *dir)const;

	private: // data
		std::map<const SystemInfo*, FileFilter> mInfoFilterMap; 
};

#endif

