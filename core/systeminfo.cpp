#include "systeminfo.h"
#include "utils.h"

using std::string;
using std::vector;

const std::string SystemInfo::INVALID_NAME("");

SystemInfo::~SystemInfo()
{
}

string SystemInfo::concatNameValue(
	const string &name, const string &value)const
{
	return name + ": " + value;
}

void SystemInfo::initFilterFlags(bool *filterFlags, int size,
	const vector<char> &filter) const
{
	if (filterFlags == NULL || size < 0)
	{
		return;
	}

	for (int i = 0; i < size; ++i)
	{
		filterFlags[i] = false;
	}

	for (size_t i = 0; i < filter.size(); ++i)
	{
		int index = (int)filter[i];
		if (index < size)
		{
			filterFlags[index] = true;
		}
	}
}

string SystemInfo::concatNameValue(
	const string &name, short value)const
{
	return concatNameValue(name, (long long)value);
}

string SystemInfo::concatNameValue(
	const string &name, unsigned short value)const
{
	return concatNameValue(name, (unsigned long long)value);
}

string SystemInfo::concatNameValue(
	const string &name, int value)const
{
	return concatNameValue(name, (long long)value);
}

string SystemInfo::concatNameValue(
	const string &name, unsigned int value)const
{
	return concatNameValue(name, (unsigned long long)value);
}

string SystemInfo::concatNameValue(
	const string &name, long value)const
{
	return concatNameValue(name, (long long)value);
}

string SystemInfo::concatNameValue(
	const string &name, unsigned long value)const
{
	return concatNameValue(name, (unsigned long long)value);
}

string SystemInfo::concatNameValue(
	const string &name, long long value)const
{
	return concatNameValue(name, SignedToString(value));
}

string SystemInfo::concatNameValue(
	const string &name, unsigned long long value)const
{
	return concatNameValue(name, UnsignedToString(value));
}

string SystemInfo::concatNameValue(
	const string &name, float value)const
{
	return concatNameValue(name, FloatToString(value));
}

string SystemInfo::concatNameValue(
	const string &name, double value)const
{
	return concatNameValue(name, DoubleToString(value));
}

