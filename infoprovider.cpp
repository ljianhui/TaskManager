#include <QWriteLocker>
#include <QMutex>
#include <QMutexLocker>

#include <sys/types.h>

#include "infoprovider.h"
#include "publicreadwritelock.h"

#include "baseinfo.h"
#include "processinfo.h"
#include "cpuinfo.h"
#include "memoryinfo.h"
#include "networkinfo.h"
#include "filesysteminfo.h"

InfoProvider* InfoProvider::sInstance(NULL);

InfoProvider* InfoProvider::getInstance()
{
    static QMutex mutex;
    if (sInstance == NULL)
    {
        QMutexLocker locker(&mutex);
        if (sInstance == NULL)
        {
            sInstance = new InfoProvider;
        }
    }
    return sInstance;
}

const BaseInfo* InfoProvider::getBaseInfo()const
{
    return mBaseInfo;
}

const ProcessInfo* InfoProvider::getProcessInfo()const
{
    return mProcessInfo;
}

const CPUInfo* InfoProvider::getCPUInfo()const
{
    return mCPUInfo;
}

const MemoryInfo* InfoProvider::getMemoryInfo()const
{
    return mMemoryInfo;
}

const NetworkInfo* InfoProvider::getNetworkInfo()const
{
    return mNetworkInfo;
}

const FileSystemInfo* InfoProvider::getFileSystemInfo()const
{
    return mFileSystemInfo;
}

void InfoProvider::sortProcessesByPid()
{
    {
    QWriteLocker locker(PublicReadWriteLock::getLock());
    mProcessInfo->sortByPid();
    }
    emit resort();
}

void InfoProvider::sortProcessesByName()
{
    {
    QWriteLocker locker(PublicReadWriteLock::getLock());
    mProcessInfo->sortByProcessName();
    }
    emit resort();
}

void InfoProvider::sortProcessesByMemory()
{
    {
    QWriteLocker locker(PublicReadWriteLock::getLock());
    mProcessInfo->sortByMemory();
    }
    emit resort();
}

void InfoProvider::sortProcessesByCPUOccupyRate()
{
    {
    QWriteLocker locker(PublicReadWriteLock::getLock());
    mProcessInfo->sortByCPUOccupyRate();
    }
    emit resort();
}

void InfoProvider::sortProcessesByUserName()
{
    {
    QWriteLocker locker(PublicReadWriteLock::getLock());
    mProcessInfo->sortByUserName();
    }
    emit resort();
}

void InfoProvider::sortProcessesByPriority()
{
    {
    QWriteLocker locker(PublicReadWriteLock::getLock());
    mProcessInfo->sortByPriority();
    }
    emit resort();
}

void InfoProvider::resetProcessUserFilter(uid_t uid)
{
    {
    QWriteLocker locker(PublicReadWriteLock::getLock());
    mProcessInfo->setUserIdFilter(uid);
    mProcessInfo->update();
    }
    emit resetUserFilter();
}

void InfoProvider::run()
{
    while (mRun)
    {
        sleep(2);
        {
            QWriteLocker locker(PublicReadWriteLock::getLock());
            if (mBaseInfo != NULL)
            {
                mBaseInfo->update();
            }
            if (mProcessInfo != NULL)
            {
                mProcessInfo->update();
            }
            if (mCPUInfo != NULL)
            {
                mCPUInfo->update();
            }
            if (mMemoryInfo != NULL)
            {
                mMemoryInfo->update();
            }
            if (mNetworkInfo != NULL)
            {
                mNetworkInfo->update();
            }
            if (mFileSystemInfo != NULL)
            {
                mFileSystemInfo->update();
            }
        }
        emit finishUpdate();
    }
}

InfoProvider::InfoProvider():
    mBaseInfo(new BaseInfo()),
    mProcessInfo(new ProcessInfo()),
    mCPUInfo(new CPUInfo()),
    mMemoryInfo(new MemoryInfo()),
    mNetworkInfo(new NetworkInfo()),
    mFileSystemInfo(new FileSystemInfo()),
    mRun(true)
{
    start();
}

InfoProvider::~InfoProvider()
{
    {
        QWriteLocker locker(PublicReadWriteLock::getLock());
        mRun = false;
        delete mBaseInfo;
        mBaseInfo = NULL;
        delete mProcessInfo;
        mProcessInfo = NULL;
        delete mCPUInfo;
        mCPUInfo = NULL;
        delete mMemoryInfo;
        mMemoryInfo = NULL;
        delete mNetworkInfo;
        mNetworkInfo = NULL;
        delete mFileSystemInfo;
        mFileSystemInfo = NULL;
    }
    wait();
}

