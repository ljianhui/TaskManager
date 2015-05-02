#ifndef _UTILS_H
#define _UTILS_H

#include <string>

std::string UnsignedToString(unsigned long long n);
std::string SignedToString(long long n);
std::string FloatToString(float f, bool keepOnePlace = true);
std::string DoubleToString(double d, bool keepOnePlace = true);
std::string BytesToString(unsigned long long bytes);
std::string FloatToPercent(float f);

std::string LocalTime();

bool equal(float x, float y);

#endif

