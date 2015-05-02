#include <string>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "filehelper.h"

using std::string;

FileHelper::FileHelper(const char *fileName, const char *mode,
	const char *alterMode):
	mFile(NULL)
{
	mFile = fopen(fileName, mode);
	if (mFile == NULL && alterMode != NULL)
	{
		mFile = fopen(fileName, alterMode);
	}
}

FileHelper::~FileHelper()
{
	if (isOpened())
	{
		fclose(mFile);
		mFile = NULL;
	}
}

bool FileHelper::isOpened()const
{
	return mFile != NULL ? true : false;
}

bool FileHelper::isEnd()const
{
	if (!isOpened())
	{
		return true;
	}
	return feof(mFile);
}

char* FileHelper::readLine(char *buffer, int size)
{
	if (buffer == NULL || size <= 0 || mFile == NULL)
	{
		return NULL;
	}
	return fgets(buffer, size, mFile);
}

int FileHelper::write(const char *txt)
{
	if (txt == NULL || mFile == NULL)
		return 0;
	return fputs(txt, mFile);
}

int FileHelper::write(const string &txt)
{
	return write(txt.c_str());
}

FILE* FileHelper::filePtr()
{
	return mFile;
}

const FILE* FileHelper::filePtr()const
{
	return mFile;
}

const char* FileHelper::getErrorString()const
{
	return strerror(errno);
}

int FileHelper::getErrorCode()const
{
	return errno;
}

