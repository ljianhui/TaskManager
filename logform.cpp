#include <QReadWriteLock>
#include <QReadLocker>

#include "logform.h"
#include "ui_logform.h"

#include "logworker.h"
#include "systeminfoprovider.h"

#include "log.h"
#include "baseinfo.h"
#include "processinfo.h"
#include "cpuinfo.h"
#include "memoryinfo.h"
#include "networkinfo.h"
#include "filesysteminfo.h"

const int LogForm::INDEX_BASE_INFO(0);
const int LogForm::INDEX_PROCESS_INFO(1);
const int LogForm::INDEX_CPU_INFO(2);
const int LogForm::INDEX_MEMORY_INFO(3);
const int LogForm::INDEX_NETWORK_INFO(4);
const int LogForm::INDEX_FILESYSTEM_INFO(5);

LogForm::LogForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogForm),
    mCheckBoxHistorysVec(6),
    mLogWorker(LogWorker::getInstance())
{
    ui->setupUi(this);

    initBaseInfoCheckBoxs();
    initProcessInfoCheckBoxs();
    initCPUInfoCheckBoxs();
    initMemoryInfoCheckBoxs();
    initNetworkInfoCheckBoxs();
    initFileSystemInfoCheckBoxs();

    registerAllToLog();

    makeConnection();
}

LogForm::~LogForm()
{
    delete ui;
    delete mLogWorker;
}

void LogForm::initBaseInfoCheckBoxs()
{
    CheckBoxHistorys &checkBoxHistorys = mCheckBoxHistorysVec[INDEX_BASE_INFO];
    QVector<QCheckBox*> &checkBoxs = checkBoxHistorys.checkBoxs;
    checkBoxs.append(ui->chbBaseUserName);
    checkBoxs.append(ui->chbBaseUserId);
    checkBoxs.append(ui->chbBaseSystemName);
    checkBoxs.append(ui->chbBaseHostName);
    checkBoxs.append(ui->chbBaseSystemRelease);
    checkBoxs.append(ui->chbBaseSystemVersion);
    checkBoxs.append(ui->chbBaseMachineType);

    initCheckedHistory(checkBoxs, checkBoxHistorys.historys);
}

void LogForm::initProcessInfoCheckBoxs()
{
    CheckBoxHistorys &checkBoxHistorys = mCheckBoxHistorysVec[INDEX_PROCESS_INFO];
    QVector<QCheckBox*> &checkBoxs = checkBoxHistorys.checkBoxs;
    checkBoxs.append(ui->chbProcessPid);
    checkBoxs.append(ui->chbProcessName);
    checkBoxs.append(ui->chbProcessState);
    checkBoxs.append(ui->chbProcessMemory);
    checkBoxs.append(ui->chbProcessMemoryRate);
    checkBoxs.append(ui->chbProcessCPURate);
    checkBoxs.append(ui->chbProcessUser);
    checkBoxs.append(ui->chbProcessPRI);
    checkBoxs.append(ui->chbProcessNice);
    checkBoxs.append(ui->chbProcessCmdLine);

    initCheckedHistory(checkBoxs, checkBoxHistorys.historys);
}

void LogForm::initCPUInfoCheckBoxs()
{
    CheckBoxHistorys &checkBoxHistorys = mCheckBoxHistorysVec[INDEX_CPU_INFO];
    QVector<QCheckBox*> &checkBoxs = checkBoxHistorys.checkBoxs;
    checkBoxs.append(ui->chbCPUCount);
    checkBoxs.append(ui->chbCPUType);
    checkBoxs.append(ui->ChbCPUUsage);

    initCheckedHistory(checkBoxs, checkBoxHistorys.historys);
}

void LogForm::initMemoryInfoCheckBoxs()
{
    CheckBoxHistorys &checkBoxHistorys = mCheckBoxHistorysVec[INDEX_MEMORY_INFO];
    QVector<QCheckBox*> &checkBoxs = checkBoxHistorys.checkBoxs;
    checkBoxs.append(ui->chbMemoryTotalRAM);
    checkBoxs.append(ui->chbMemoryFreeRAM);
    checkBoxs.append(ui->chbMemoryRAMUsage);
    checkBoxs.append(ui->chbMemoryTotalSwap);
    checkBoxs.append(ui->chbMemoryFreeSwap);
    checkBoxs.append(ui->chbMemorySwapUsage);

    initCheckedHistory(checkBoxs, checkBoxHistorys.historys);
}

void LogForm::initNetworkInfoCheckBoxs()
{
    CheckBoxHistorys &checkBoxHistorys = mCheckBoxHistorysVec[INDEX_NETWORK_INFO];
    QVector<QCheckBox*> &checkBoxs = checkBoxHistorys.checkBoxs;
    checkBoxs.append(ui->chbNetRecvBytes);
    checkBoxs.append(ui->chbNetRecvPackets);
    checkBoxs.append(ui->chbNetRecvErrors);
    checkBoxs.append(ui->chbNetRecvDrop);
    checkBoxs.append(ui->chbNetRecvRate);
    checkBoxs.append(ui->chbNetRecvErrorRate);
    checkBoxs.append(ui->chbNetRecvDropRate);

    checkBoxs.append(ui->chbNetSentBytes);
    checkBoxs.append(ui->chbNetSentPackets);
    checkBoxs.append(ui->chbNetSentErrors);
    checkBoxs.append(ui->chbNetSentDrop);
    checkBoxs.append(ui->chbNetSentRate);
    checkBoxs.append(ui->chbNetSentErrorRate);
    checkBoxs.append(ui->chbNetSentDropRate);

    initCheckedHistory(checkBoxs, checkBoxHistorys.historys);
}

void LogForm::initFileSystemInfoCheckBoxs()
{
    CheckBoxHistorys &checkBoxHistorys = mCheckBoxHistorysVec[INDEX_FILESYSTEM_INFO];
    QVector<QCheckBox*> &checkBoxs = checkBoxHistorys.checkBoxs;
    checkBoxs.append(ui->chbFSIsMounted);
    checkBoxs.append(ui->chbFSDeviceName);
    checkBoxs.append(ui->chbFSMountDir);
    checkBoxs.append(ui->chbFSMountType);
    checkBoxs.append(ui->chbFSBlockSize);
    checkBoxs.append(ui->chbFSBlockCount);
    checkBoxs.append(ui->chbFSBlockFree);
    checkBoxs.append(ui->chbFSTotalBytes);
    checkBoxs.append(ui->chbFSFreeBytes);
    checkBoxs.append(ui->chbFSUsage);
    checkBoxs.append(ui->chbFSLabel);
    checkBoxs.append(ui->chbFSUuid);

    initCheckedHistory(checkBoxs, checkBoxHistorys.historys);
}

void LogForm::initCheckedHistory(const QVector<QCheckBox*> &checkBoxs,
                                 QVector<bool> &historys)
{
    for (int i = 0; i < checkBoxs.size(); ++i)
    {
        QCheckBox *checkBox = checkBoxs[i];
        if (checkBox != NULL)
        {
            historys.append(checkBox->isChecked());
        }
        else
        {
            historys.append(false);
        }
    }
}

void LogForm::toggleCheckBox(bool checked)
{
    for (int i = 0; i < mCheckBoxHistorysVec.size(); ++i)
    {
        CheckBoxHistorys &checkBoxHistorys = mCheckBoxHistorysVec[i];
        for (int j = 0; j < checkBoxHistorys.checkBoxs.size(); ++j)
        {
            bool isChecked = checkBoxHistorys.checkBoxs[j]->isChecked();
            if (isChecked != checkBoxHistorys.historys[j])
            {
                if (isChecked)
                {
                    mLogWorker->addFilterToSystemInfo(getSystemInfo(i), (char)j);
                }
                else
                {
                    mLogWorker->removeFilterFromSystemInfo(getSystemInfo(i), (char)j);
                }
                checkBoxHistorys.historys[j] = isChecked;
                return;
            }
        }
    }
}

const SystemInfo* LogForm::getSystemInfo(int index)
{
    const SystemInfo *systemInfo = NULL;
    switch (index)
    {
    case INDEX_BASE_INFO:
        systemInfo = SystemInfoProvider::getInstance()->getBaseInfo();
        break;
    case INDEX_PROCESS_INFO:
        systemInfo = SystemInfoProvider::getInstance()->getProcessInfo();
        break;
    case INDEX_CPU_INFO:
        systemInfo = SystemInfoProvider::getInstance()->getCPUInfo();
        break;
    case INDEX_MEMORY_INFO:
        systemInfo = SystemInfoProvider::getInstance()->getMemoryInfo();
        break;
    case INDEX_NETWORK_INFO:
        systemInfo = SystemInfoProvider::getInstance()->getNetworkInfo();
        break;
    case INDEX_FILESYSTEM_INFO:
        systemInfo = SystemInfoProvider::getInstance()->getFileSystemInfo();
    default:
        break;
    }
    return systemInfo;
}

void LogForm::registerAllToLog()
{
    registerBaseInfo(ui->gbBaseInfo->isCheckable());
    registerProcessInfo(ui->gbProcessInfo->isChecked());
    registerCPUInfo(ui->gbCPUInfo->isChecked());
    registerMemoryInfo(ui->gbMemoryInfo->isChecked());
    registerNetworkInfo(ui->gbNetworkInfo->isChecked());
    registerFileSystemInfo(ui->gbFileSystemInfo->isChecked());
}

void LogForm::registerBaseInfo(bool checked)
{
    registerToLog(checked, getSystemInfo(INDEX_BASE_INFO),
                  tr("baseinfo.log"),
                  mCheckBoxHistorysVec[INDEX_BASE_INFO]);
}

void LogForm::registerProcessInfo(bool checked)
{
    registerToLog(checked, getSystemInfo(INDEX_PROCESS_INFO),
                  tr("processinfo.log"),
                  mCheckBoxHistorysVec[INDEX_PROCESS_INFO]);
}

void LogForm::registerCPUInfo(bool checked)
{
    registerToLog(checked, getSystemInfo(INDEX_CPU_INFO),
                  tr("cpuinfo.log"),
                  mCheckBoxHistorysVec[INDEX_CPU_INFO]);
}

void LogForm::registerMemoryInfo(bool checked)
{
    registerToLog(checked, getSystemInfo(INDEX_MEMORY_INFO),
                  tr("memoryinfo.log"),
                  mCheckBoxHistorysVec[INDEX_MEMORY_INFO]);
}

void LogForm::registerNetworkInfo(bool checked)
{
    registerToLog(checked, getSystemInfo(INDEX_NETWORK_INFO),
                  tr("networkinfo.log"),
                  mCheckBoxHistorysVec[INDEX_NETWORK_INFO]);
}

void LogForm::registerFileSystemInfo(bool checked)
{
    registerToLog(checked, getSystemInfo(INDEX_FILESYSTEM_INFO),
                  tr("filesysteminfo.log"),
                  mCheckBoxHistorysVec[INDEX_FILESYSTEM_INFO]);
}

void LogForm::registerToLog(bool checked,
    const SystemInfo *info, const QString &logFileName,
    const CheckBoxHistorys &checkBoxHistorys)
{
    if (checked)
    {
        mLogWorker->addToWorker(info, logFileName);
        const QVector<bool> &historys = checkBoxHistorys.historys;
        for (int i = 0; i < historys.size(); ++i)
        {
            if (historys[i])
            {
                mLogWorker->addFilterToSystemInfo(info, (char)i);
            }
        }
    }
    else
    {
        mLogWorker->removeFromWorker(info);
    }
}

void LogForm::makeConnection()
{
    connect(ui->chbStartLog, SIGNAL(toggled(bool)),
            this, SLOT(toggleLogWorker(bool)));
    connect(ui->cbbLogFrq, SIGNAL(currentIndexChanged(int)),
            this, SLOT(setFrqForLogWorker(int)));
    connect(ui->gbBaseInfo, SIGNAL(toggled(bool)),
            this, SLOT(registerBaseInfo(bool)));
    connect(ui->gbProcessInfo, SIGNAL(toggled(bool)),
            this, SLOT(registerProcessInfo(bool)));
    connect(ui->gbCPUInfo, SIGNAL(toggled(bool)),
            this, SLOT(registerCPUInfo(bool)));
    connect(ui->gbMemoryInfo, SIGNAL(toggled(bool)),
            this, SLOT(registerMemoryInfo(bool)));
    connect(ui->gbNetworkInfo, SIGNAL(toggled(bool)),
            this, SLOT(registerNetworkInfo(bool)));
    connect(ui->gbFileSystemInfo, SIGNAL(toggled(bool)),
            this, SLOT(registerFileSystemInfo(bool)));

    for (int i = 0; i < mCheckBoxHistorysVec.size(); ++i)
    {
        QVector<QCheckBox*> &checkBoxs = mCheckBoxHistorysVec[i].checkBoxs;
        for (int j = 0; j < checkBoxs.size(); ++j)
        {
            connect(checkBoxs[j], SIGNAL(toggled(bool)),
                    this, SLOT(toggleCheckBox(bool)));
        }
    }
}

void LogForm::toggleLogWorker(bool start)
{
    if (start)
    {
        mLogWorker->startWorker();
    }
    else
    {
        mLogWorker->stopWorker();
    }
}

void LogForm::setFrqForLogWorker(int index)
{
    static int seconds[] = {2, 5, 10, 30, 60, 300, 600,
                           1800, 3600};
    if ((size_t)index >= sizeof(seconds) / sizeof(int))
    {
        return;
    }

    mLogWorker->setFrq(seconds[index]);
}
