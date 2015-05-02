#ifndef _BASEINFO_H
#define _BASEINFO_H

#include <sys/types.h>
#include <string>
#include "systeminfo.h"

class BaseInfo : public SystemInfo
{
	public: // function
		BaseInfo();
		BaseInfo(const BaseInfo &other);
		BaseInfo& operator=(const BaseInfo &other);
		virtual ~BaseInfo();

		std::string getUserName()const;
		uid_t getUserId()const;
		std::string getSystemName()const;
		std::string getHostName()const;
		std::string getSystemRelease()const;
		std::string getSystemVersion()const;
		std::string getMachineType()const;

		virtual void update();
		virtual std::string toString()const;
		virtual std::string toString(const std::vector<char> &filter)const;

	public: // data
		const static char FILTER_USER_NAME;
		const static char FILTER_USER_ID;
		const static char FILTER_SYSTEM_NAME;
		const static char FILTER_HOST_NAME;
		const static char FILTER_SYSTEM_RELEASE;
		const static char FILTER_SYSTEM_VERSION;
		const static char FILTER_MACHINE_TYPE;

	private: // function
		std::string getPairByName(int filter)const;
		void refreshData();

	private: // data
		const static std::string NAMES[];

	private: // data
		std::string mUserName;
		uid_t mUserId;
		std::string mSystemName;
		std::string mHostName;
		std::string mSystemRelease;
		std::string mSystemVersion;
		std::string mMachineType;
};

#endif
