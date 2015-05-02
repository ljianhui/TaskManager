#ifndef _FILEHELPER_H
#define _FILEHELPER_H

#include <stdio.h>

class string;

class FileHelper
{
	public:
		FileHelper(const char *fileName, const char *mode,
			const char *alterMode = NULL);
		~FileHelper();
		bool isOpened()const;
		bool isEnd()const;

		char* readLine(char *buffer, int size);
		int write(const char *txt);
		int write(const std::string &txt);

		FILE* filePtr();
		const FILE* filePtr()const;

		const char* getErrorString()const;
		int getErrorCode()const;

	private: // function
		FileHelper(const FileHelper &other){}
		FileHelper& operator=(const FileHelper &other)
		{ return *this; }

	private: // data
		FILE *mFile;
};

#endif
