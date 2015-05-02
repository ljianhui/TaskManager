#include <stdio.h>
#include <time.h>
#include <math.h>
#include "utils.h"

using std::string;

string UnsignedToString(unsigned long long n)
{
	char data[128] = {0};
	snprintf(data, sizeof(data), "%llu", n);
	return string(data);
}

string SignedToString(long long n)
{
	char data[128] = {0};
	snprintf(data, sizeof(data), "%lld", n);
	return string(data);
}

string FloatToString(float f, bool keepOnePlace)
{
	char data[128] = {0};
	if (keepOnePlace)
	{
		snprintf(data, sizeof(data), "%.1f", f);
	}
	else
	{
		snprintf(data, sizeof(data), "%f", f);
	}
	return string(data);
}

string DoubleToString(double d, bool keepOnePlaces)
{
	char data[128] = {0};
	if (keepOnePlaces)
	{
		snprintf(data, sizeof(data), "%.1lf", d);
	}
	else
	{
		snprintf(data, sizeof(data), "%lf", d);
	}
	return string(data);
}

string LocalTime()
{
	time_t currentTime = time(NULL);
	tm *time = localtime(&currentTime);
	char buffer[128] = {0};
	snprintf(buffer, sizeof(buffer), "%d/%02d/%02d %02d:%02d:%02d",
		time->tm_year + 1900, time->tm_mon + 1, time->tm_mday,
		time->tm_hour, time->tm_min, time->tm_sec);
	return string(buffer);
}

string BytesToString(unsigned long long bytes)
{
	float fbytes = (float)bytes;
	int i = 0;

	const static string UNITS[] = {"B", "K", "M", "G", "T"};
	for (i = 0; i < 5 && bytes >= 1024; ++i)
	{
		fbytes /= 1024;
		bytes /= 1024;
	}

	if (equal((float)bytes, fbytes))
	{
		return UnsignedToString(bytes) + UNITS[i];
	}
	return FloatToString(fbytes) + UNITS[i];
}

string FloatToPercent(float f)
{
	char buffer[128] = {0};
	snprintf(buffer, sizeof(buffer), "%.1f%%", f * 100);
	return string(buffer);
}

bool equal(float x, float y)
{
	return fabsf(x -y) < 1e-5;
}

