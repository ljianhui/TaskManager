#include <QWriteLocker>
#include <QMutex>
#include <QMutexLocker>

#include <sys/types.h>

#include "systeminfoprovider.h"
#include "publicreadwritelock.h"

#include "baseinfo.h"
#include "processinfo.h"
#include "cpuinfo.h"
#include "memoryinfo.h"
#include "networkinfo.h"
#include "filesysteminfo.h"

SystemInfoProvider* SystemInfoProvider::sInstance(NULL);

SystemInfoProvider* SystemInfoProvider::getInstance()
{
    static QMutex mutex;
    if (sInstance == NULL)
    {
        QMutexLocker locker(&mutex);
        if (sInstance == NULL)
        {
            sInstance = new SystemInfoProvider;
        }
    }
    return sInstance;
}

const BaseInfo* SystemInfoProvider::getBaseInfo()const
{
    return mBaseInfo;
}

const ProcessInfo* SystemInfoProvider::getProcessInfo()const
{
    return mProcessInfo;
}

const CPUInfo* SystemInfoProvider::getCPUInfo()const
{
    return mCPUInfo;
}

const MemoryInfo* SystemInfoProvider::getMemoryInfo()const
{
    return mMemoryInfo;
}

const NetworkInfo* SystemInfoProvider::getNetworkInfo()const
{
    return mNetworkInfo;
}

const FileSystemInfo* SystemInfoProvider::getFileSystemInfo()const
{
    return mFileSystemInfo;
}

void SystemInfoProvider::sortProcessesByPid()
{
    {
    QWriteLocker locker(PublicReadWriteLock::getLock());
    mProcessInfo->sortByPid();
    }
    emit resort();
}

void SystemInfoProvider::sortProcessesByName()
{
    {
    QWriteLocker locker(PublicReadWriteLock::getLock());
    mProcessInfo->sortByProcessName();
    }
    emit resort();
}

void SystemInfoProvider::sortProcessesByMemory()
{
    {
    QWriteLocker locker(PublicReadWriteLock::getLock());
    mProcessInfo->sortByMemory();
    }
    emit resort();
}

void SystemInfoProvider::sortProcessesByCPUOccupyRate()
{
    {
    QWriteLocker locker(PublicReadWriteLock::getLock());
    mProcessInfo->sortByCPUOccupyRate();
    }
    emit resort();
}

void SystemInfoProvider::sortProcessesByUserName()
{
    {
    QWriteLocker locker(PublicReadWriteLock::getLock());
    mProcessInfo->sortByUserName();
    }
    emit resort();
}

void SystemInfoProvider::sortProcessesByPriority()
{
    {
    QWriteLocker locker(PublicReadWriteLock::getLock());
    mProcessInfo->sortByPriority();
    }
    emit resort();
}

void SystemInfoProvider::resetProcessUserFilter(uid_t uid)
{
    {
    QWriteLocker locker(PublicReadWriteLock::getLock());
    mProcessInfo->setUserIdFilter(uid);
    mProcessInfo->update();
    }
    emit resetUserFilter();
}

void SystemInfoProvider::run()
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

SystemInfoProvider::SystemInfoProvider():
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

SystemInfoProvider::~SystemInfoProvider()
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
