#ifndef INFOPROVIDER_H
#define INFOPROVIDER_H

#include <QThread>
#include <QReadWriteLock>

class BaseInfo;
class ProcessInfo;
class CPUInfo;
class MemoryInfo;
class NetworkInfo;
class FileSystemInfo;

class InfoProvider : public QThread
{
    Q_OBJECT
public:
    static InfoProvider* getInstance();
    ~InfoProvider();

    const BaseInfo* getBaseInfo()const;
    const ProcessInfo* getProcessInfo()const;
    const CPUInfo* getCPUInfo()const;
    const MemoryInfo* getMemoryInfo()const;
    const NetworkInfo* getNetworkInfo()const;
    const FileSystemInfo* getFileSystemInfo()const;

    void sortProcessesByPid();
    void sortProcessesByName();
    void sortProcessesByMemory();
    void sortProcessesByCPUOccupyRate();
    void sortProcessesByUserName();
    void sortProcessesByPriority();

    void resetProcessUserFilter(uid_t uid);

signals:
    void finishUpdate();
    void resort();
    void resetUserFilter();

protected:
    void run();

private:
    InfoProvider();

private:
    static InfoProvider *sInstance;

    BaseInfo *mBaseInfo;
    ProcessInfo *mProcessInfo;
    CPUInfo *mCPUInfo;
    MemoryInfo *mMemoryInfo;
    NetworkInfo *mNetworkInfo;
    FileSystemInfo *mFileSystemInfo;

    bool mRun;
};

#endif // INFOPROVIDER_H
