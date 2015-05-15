#ifndef LOGWORKER_H
#define LOGWORKER_H

#include <QThread>
#include <QMutex>

class Log;
class SystemInfo;

class LogWorker : public QThread
{
public:
    static LogWorker* getInstance();
    ~LogWorker();

    void stopWorker();
    void startWorker();
    void setFrq(unsigned long sec);

    void addToWorker(const SystemInfo* info,
                     const QString &logFileName);
    void removeFromWorker(const SystemInfo *info);

    void addFilterToSystemInfo(const SystemInfo *info, char filter);
    void removeFilterFromSystemInfo(const SystemInfo *info, char filter);

protected:
    void run();

private: // function
    LogWorker();

private: // data
    static LogWorker *sInstance;

    unsigned long mSeconds;
    QMutex mMutex;
    Log *mLog;
    volatile bool mRun;
};

#endif // LOGWORKER_H
