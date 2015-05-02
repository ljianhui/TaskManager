#ifndef _NETWORKINFO_H
#define _NETWORKINFO_H

#include <string>
#include <vector>
#include <time.h>
#include "systeminfo.h"

class FileHelper;

class NetworkInfo : public SystemInfo
{
	public:
		NetworkInfo();
		NetworkInfo(const NetworkInfo &other);
		NetworkInfo& operator=(const NetworkInfo &other);
		virtual ~NetworkInfo();

		inline int getInterfaceCount()const;
		inline unsigned long long getReceivedBytes()const;
		inline unsigned long long getReceivedPackets()const;
		inline unsigned long long getReceivedErrors()const;
		inline unsigned long long getReceivedDrop()const;
		inline unsigned long long getSentBytes()const;
		inline unsigned long long getSentPackets()const;
		inline unsigned long long getSentErrors()const;
		inline unsigned long long getSentDrop()const;

		inline unsigned long long getReceivingRate()const;
		inline unsigned long long getSendingRate()const;
		inline float getReceivedErrorRate()const;
		inline float getSentErrorRate()const;
		inline float getReceivedDropRate()const;
		inline float getSentDropRate()const;

		virtual void update();
		virtual std::string toString()const;
		virtual std::string toString(
			const std::vector<char> &filter)const;

	public: //data
		const static char FILTER_RECEIVED_BYTES;
		const static char FILTER_RECEIVED_PACKETS;
		const static char FILTER_RECEIVED_ERRORS;
		const static char FILTER_RECEIVED_DROP;
		const static char FILTER_RECEIVING_RATE;
		const static char FILTER_RECEIVED_ERROR_RATE;
		const static char FILTER_RECEIVED_DROP_RATE;
		const static char FILTER_SENT_BYTES;
		const static char FILTER_SENT_PACKETS;
		const static char FILTER_SENT_ERRORS;
		const static char FILTER_SENT_DROP;
		const static char FILTER_SENDING_RATE;
		const static char FILTER_SENT_ERROR_RATE;
		const static char FILTER_SENT_DROP_RATE;

	private: // data
		const static std::string NAMES[];

	private: // struct
		struct Interface
		{
			Interface();
			unsigned long long recvBytes;
			unsigned long long recvPackets;
			unsigned long long recvErrors;
			unsigned long long recvDrop;
			unsigned long long sentBytes;
			unsigned long long sentPackets;
			unsigned long long sentErrors;
			unsigned long long sentDrop;
			time_t time;
		};

	private: // function
		void refreshData();
		time_t getBootTime()const;
		bool readRecord(const char *buffer, Interface &record);
		void addAssign(const Interface &src, Interface &dst);
		void getIncrement(const Interface &x, const Interface &y,
			Interface &increment);
		std::string getNameValue(int filter)const;
	private: // data
		int mInterfaceCount;
		Interface *mRecord;
		Interface *mIncrement;
};

int NetworkInfo::getInterfaceCount()const
{
	return mInterfaceCount;
}

unsigned long long NetworkInfo::getReceivedBytes()const
{
	return mRecord ? mRecord->recvBytes : 0;
}

unsigned long long NetworkInfo::getReceivedPackets()const
{
	return mRecord ? mRecord->recvPackets : 0;
}

unsigned long long NetworkInfo::getReceivedErrors()const
{
	return mRecord ? mRecord->recvErrors : 0;
}

unsigned long long NetworkInfo::getReceivedDrop()const
{
	return mRecord ? mRecord->recvDrop : 0;
}

unsigned long long NetworkInfo::getSentBytes()const
{
	return mRecord ? mRecord->sentBytes : 0;
}

unsigned long long NetworkInfo::getSentPackets()const
{
	return mRecord ? mRecord->sentPackets : 0;
}

unsigned long long NetworkInfo::getSentErrors()const
{
	return mRecord ? mRecord->sentErrors : 0;
}

unsigned long long NetworkInfo::getSentDrop()const
{
	return mRecord ? mRecord->sentDrop : 0;
}

unsigned long long NetworkInfo::getReceivingRate()const
{
	if (mIncrement == NULL || mIncrement->time == 0)
		return 0;
	return mIncrement->recvBytes / mIncrement->time;
}

unsigned long long NetworkInfo::getSendingRate()const
{
	if (mIncrement == NULL || mIncrement->time == 0)
		return 0;
	return mIncrement->sentBytes / mIncrement->time;
}

float NetworkInfo::getReceivedErrorRate()const
{
	if (mRecord == NULL || mRecord->recvPackets == 0)
		return 0;
	return (float)mRecord->recvErrors / (float)mRecord->recvPackets;
}

float NetworkInfo::getSentErrorRate()const
{
	if (mRecord == NULL || mRecord->sentPackets == 0)
		return 0;
	return (float)mRecord->sentErrors / (float)mRecord->sentPackets;
}

float NetworkInfo::getReceivedDropRate()const
{
	if (mRecord == NULL || mRecord->recvPackets == 0)
		return 0;
	return (float)mRecord->recvDrop / (float)mRecord->recvPackets;
}

float NetworkInfo::getSentDropRate()const
{
	if (mRecord == NULL || mRecord->sentPackets == 0)
		return 0;
	return (float)mRecord->sentDrop / (float)mRecord->recvPackets;
}

#endif

