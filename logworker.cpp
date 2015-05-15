#include <QReadLocker>
#include <QMutex>
#include <QMutexLocker>
#include "logworker.h"
#include "publicreadwritelock.h"
#include "log.h"

LogWorker* LogWorker::sInstance(NULL);

LogWorker* LogWorker::getInstance()
{
    if (sInstance == NULL)
    {
        static QMutex mutex;
        QMutexLocker locker(&mutex);
        if (sInstance == NULL)
        {
            sInstance = new LogWorker();
        }
    }
    return sInstance;
}

LogWorker::LogWorker():
    mSeconds(5),
    mMutex(),
    mLog(Log::getInstance()),
    mRun(true)
{

}

LogWorker::~LogWorker()
{
    mLog->releaseInstance();
    if (isRunning())
    {
        mRun = false;
        exit();
        wait();
    }
}

void LogWorker::stopWorker()
{
    if (isRunning())
    {
        mRun = false;
        exit();
    }
}

void LogWorker::startWorker()
{
    if (!isRunning())
    {
        start();
        mRun = true;
    }
}

void LogWorker::setFrq(unsigned long sec)
{
    if (sec == mSeconds)
    {
        return;
    }
    QMutexLocker locker(&mMutex);
    mSeconds = sec;
}

void LogWorker::addToWorker(const SystemInfo *info, const QString &logFileName)
{
    if (info == NULL)
    {
        return;
    }
    QMutexLocker locker(&mMutex);
    mLog->registerSystemInfo(info, logFileName.toStdString());
}

void LogWorker::removeFromWorker(const SystemInfo *info)
{
    if (info == NULL)
    {
        return;
    }
    QMutexLocker locker(&mMutex);
    mLog->unregisterSystemInfo(info);
}

void LogWorker::addFilterToSystemInfo(const SystemInfo *info, char filter)
{
    if (info == NULL)
    {
        return;
    }
    QMutexLocker locker(&mMutex);
    mLog->addFilter(info, filter);
}

void LogWorker::removeFilterFromSystemInfo(const SystemInfo *info, char filter)
{
    if (info == NULL)
    {
        return;
    }
    QMutexLocker locker(&mMutex);
    mLog->removeFilter(info, filter);
}

void LogWorker::run()
{
    int times = 0;
    while (mRun)
    {
        sleep(1);
        ++times;
        if (times % mSeconds == 0)
        {
            QReadLocker Readlocker(PublicReadWriteLock::getLock());
            QMutexLocker mutexLocker(&mMutex);
            Log::getInstance()->logAll();
        }
    }
}



