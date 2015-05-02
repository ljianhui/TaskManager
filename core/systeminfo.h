#ifndef _SYSTEMINFO_H
#define _SYSTEMINFO_H

#include <string>
#include <vector>

class SystemInfo
{
	public:
		virtual void update() = 0;
		virtual std::string toString()const = 0;
		virtual std::string toString(const std::vector<char> &filter)const = 0;
		virtual ~SystemInfo();
	protected: // data
		const static std::string INVALID_NAME;

	protected: // function
		void initFilterFlags(bool *filterFlags, int size,
			const std::vector<char> &filter)const;
		std::string concatNameValue(
			const std::string &name, const std::string &valeu)const;

		std::string concatNameValue(
			const std::string &name, short value)const;
		std::string concatNameValue(
			const std::string &name, unsigned short value)const;

		std::string concatNameValue(
			const std::string &name, int value)const;
		std::string concatNameValue(
			const std::string &name, unsigned int value)const;

		std::string concatNameValue(
			const std::string &name, long value)const;
		std::string concatNameValue(
			const std::string &name, unsigned long value)const;

		std::string concatNameValue(
			const std::string &name, long long value)const;
		std::string concatNameValue(
			const std::string &name, unsigned long long value)const;

		std::string concatNameValue(
			const std::string &name, float value)const;
		std::string concatNameValue(
			const std::string &name, double value)const;
};

#endif
