#include <string.h>
#include <stdio.h>
#include "networkinfo.h"
#include "filehelper.h"
#include "utils.h"

using std::string;
using std::vector;

const char NetworkInfo::FILTER_RECEIVED_BYTES(0);
const char NetworkInfo::FILTER_RECEIVED_PACKETS(1);
const char NetworkInfo::FILTER_RECEIVED_ERRORS(2);
const char NetworkInfo::FILTER_RECEIVED_DROP(3);
const char NetworkInfo::FILTER_RECEIVING_RATE(4);
const char NetworkInfo::FILTER_RECEIVED_ERROR_RATE(5);
const char NetworkInfo::FILTER_RECEIVED_DROP_RATE(6);
const char NetworkInfo::FILTER_SENT_BYTES(7);
const char NetworkInfo::FILTER_SENT_PACKETS(8);
const char NetworkInfo::FILTER_SENT_ERRORS(9);
const char NetworkInfo::FILTER_SENT_DROP(10);
const char NetworkInfo::FILTER_SENDING_RATE(11);
const char NetworkInfo::FILTER_SENT_ERROR_RATE(12);
const char NetworkInfo::FILTER_SENT_DROP_RATE(13);

const string NetworkInfo::NAMES[] = {
	"received_bytes",
	"received_packets",
	"received_errors",
	"received_drop",
	"receiving_rate",
	"received_error_rate",
	"receivde_drop_rate",
	"sent_bytes",
	"sent_packets",
	"sent_errors",
	"sent_drop",
	"sending_rate",
	"sent_error_rate",
	"sent_drop_rate"
};

NetworkInfo::Interface::Interface():
	recvBytes(0),
	recvPackets(0),
	recvErrors(0),
	recvDrop(0),
	sentBytes(0),
	sentPackets(0),
	sentErrors(0),
	sentDrop(0),
	time(0)
{
}

NetworkInfo::NetworkInfo():
	mInterfaceCount(0),
	mRecord(new Interface),
	mIncrement(new Interface)
{
	mRecord->time = getBootTime();
	refreshData();
}

time_t NetworkInfo::getBootTime()const
{
	static time_t bootTime = 0;
	if (bootTime == 0)
	{
		FileHelper file("/proc/uptime", "r");
		if (!file.isOpened())
		{
			return 0;
		}
		float uptime = 0;
		char buffer[128] = {0};
		file.readLine(buffer, sizeof(buffer));
		int num = sscanf(buffer, "%f %*f", &uptime);
		if (num == 1)
		{
			bootTime = time(NULL) - (time_t)(uptime + 0.5);
		}
	}
	return bootTime;
}

NetworkInfo::NetworkInfo(const NetworkInfo &other):
	SystemInfo(other),
	mInterfaceCount(other.mInterfaceCount),
	mRecord(new Interface(*other.mRecord)),
	mIncrement(new Interface(*other.mIncrement))
{
}

NetworkInfo& NetworkInfo::operator=(const NetworkInfo &other)
{
	if (this != &other)
	{
		mInterfaceCount = other.mInterfaceCount;

		NetworkInfo tmpInfo(other);
		Interface *tmpInterface = tmpInfo.mRecord;
		tmpInfo.mRecord = mRecord;
		mRecord = tmpInterface;

		tmpInterface = tmpInfo.mIncrement;
		tmpInfo.mIncrement = mIncrement;
		mIncrement = tmpInterface;
	}
	return *this;
}

NetworkInfo::~NetworkInfo()
{
	delete mRecord;
	delete mIncrement;
}

void NetworkInfo::update()
{
	refreshData();
}

void NetworkInfo::refreshData()
{
	time_t currentTime = time(NULL);
	if (currentTime <= mRecord->time)
		return;

	FileHelper file("/proc/net/dev", "r");
	char buffer[256] = {0};
	Interface result;
	Interface newRecord;

	int i = 0;
	while (file.readLine(buffer, sizeof(buffer)))
	{
		if (readRecord(buffer, newRecord))
		{
			addAssign(newRecord, result);
			++i;
		}
	}
	
	mInterfaceCount = i;
	result.time = currentTime;
	getIncrement(result, *mRecord, *mIncrement);
	*mRecord = result;
}

bool NetworkInfo::readRecord(const char *buffer, Interface &record)
{
	char name[128] = {0};
	unsigned long long tmp = 0;
	
	int num = sscanf(buffer, 
		" %127[^:]:%Lu%Lu%Lu%Lu%Lu%Lu%Lu%Lu%Lu%Lu%Lu%Lu",
		name, &record.recvBytes, &record.recvPackets, 
		&record.recvErrors, &record.recvDrop,
		&tmp, &tmp, &tmp, &tmp,
		&record.sentBytes, &record.sentPackets,
		&record.sentErrors, &record.sentDrop);
	
	if (num != 13)
	{
		return false;
	}
	if (strcmp(name, "lo") == 0)
	{
		return false;
	}
	return true;
}

void NetworkInfo::addAssign(const Interface &src, Interface &dst)
{
	dst.recvBytes += src.recvBytes;
	dst.recvPackets += src.recvPackets;
	dst.recvErrors += src.recvErrors;
	dst.recvDrop += src.recvDrop;

	dst.sentBytes += src.sentBytes;
	dst.sentPackets += src.sentPackets;
	dst.sentErrors += src.sentErrors;
	dst.sentDrop += src.sentDrop;
}

void NetworkInfo::getIncrement(const Interface &x,
	const Interface &y, Interface &increment)
{
	increment.recvBytes = x.recvBytes - y.recvBytes;
	increment.recvPackets = x.recvPackets - y.recvPackets;
	increment.recvErrors = x.recvErrors - y.recvErrors;
	increment.recvDrop = x.recvDrop -y.recvDrop;

	increment.sentBytes = x.sentBytes - y.sentBytes;
	increment.sentPackets = x.sentPackets - y.sentPackets;
	increment.sentErrors = x.sentErrors - y.sentErrors;
	increment.sentDrop = x.sentDrop - y.sentDrop;

	increment.time = x.time - y.time;
}

string NetworkInfo::toString()const
{
	string txt;
	txt += concatNameValue(NAMES[FILTER_RECEIVED_BYTES],
		BytesToString(mRecord->recvBytes)) + ", ";
	txt += concatNameValue(NAMES[FILTER_RECEIVED_PACKETS],
		mRecord->recvPackets) + ", ";
	txt += concatNameValue(NAMES[FILTER_RECEIVING_RATE],
		BytesToString((unsigned long long)getReceivingRate())) + "/s\n";

	txt += concatNameValue(NAMES[FILTER_SENT_BYTES],
		BytesToString(mRecord->sentBytes)) + ", ";
	txt += concatNameValue(NAMES[FILTER_SENT_PACKETS],
		mRecord->sentPackets) + ", ";
	txt += concatNameValue(NAMES[FILTER_SENDING_RATE],
		BytesToString((unsigned long long)getSendingRate())) + "/s\n";
	return txt;
}

string NetworkInfo::toString(const vector<char> &filter)const
{
	const int size = 14;
	bool filterFlags[size];
	initFilterFlags(filterFlags, size, filter);

	string txt;
	for (int i = 0; i < size; ++i)
	{
		if (!filterFlags[i])
		{
			continue;
		}

		const string &nameValue = getNameValue(i);
		if (nameValue != INVALID_NAME)
		{
			txt += nameValue;
			txt += "\n";
		}
	}
	txt += "\n";
	return txt;
}

string NetworkInfo::getNameValue(int filter)const
{
	switch (filter)
	{
	case FILTER_RECEIVED_BYTES:
		return concatNameValue(NAMES[filter],
			BytesToString(mRecord->recvBytes));
	case FILTER_RECEIVED_PACKETS:
		return concatNameValue(NAMES[filter], mRecord->recvPackets);
	case FILTER_RECEIVED_ERRORS:
		return concatNameValue(NAMES[filter], mRecord->recvErrors);
	case FILTER_RECEIVED_DROP:
		return concatNameValue(NAMES[filter], mRecord->recvDrop);
	case FILTER_SENT_BYTES:
		return concatNameValue(NAMES[filter],
			BytesToString(mRecord->sentBytes));
	case FILTER_SENT_PACKETS:
		return concatNameValue(NAMES[filter], mRecord->sentPackets);
	case FILTER_SENT_ERRORS:
		return concatNameValue(NAMES[filter], mRecord->sentErrors);
	case FILTER_SENT_DROP:
		return concatNameValue(NAMES[filter], mRecord->sentDrop);
	case FILTER_RECEIVING_RATE:
		return concatNameValue(NAMES[filter], BytesToString(
			(unsigned long long)getReceivingRate())) + "/s";
	case FILTER_SENDING_RATE:
		return concatNameValue(NAMES[filter], BytesToString(
			(unsigned long long)getReceivingRate())) + "/s";
	case FILTER_RECEIVED_ERROR_RATE:
		return concatNameValue(NAMES[filter], FloatToPercent(
			getReceivedErrorRate()));
	case FILTER_SENT_ERROR_RATE:
		return concatNameValue(NAMES[filter], FloatToPercent(
			getSentErrorRate()));
	case FILTER_RECEIVED_DROP_RATE:
		return concatNameValue(NAMES[filter], FloatToPercent(
			getReceivedDropRate()));
	case FILTER_SENT_DROP_RATE:
		return concatNameValue(NAMES[filter], FloatToPercent(
			getSentDropRate()));
	default:;
	}
	return INVALID_NAME;
}

