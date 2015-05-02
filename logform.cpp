#include <QReadWriteLock>
#include <QReadLocker>

#include "logform.h"
#include "ui_logform.h"

#include "logworker.h"
#include "infoprovider.h"

#include "log.h"
#include "baseinfo.h"
#include "processinfo.h"
#include "cpuinfo.h"
#include "memoryinfo.h"
#include "networkinfo.h"
#include "filesysteminfo.h"

LogForm::LogForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogForm),
    mSysInfoCheckBoxs(6),
    mLogWorker(new LogWorker())
{
    ui->setupUi(this);

    registerAllToLog();

    int index = 0;
    initBaseInfoCheckBoxs(InfoProvider::getInstance()->getBaseInfo(), index++);
    initProcessInfoCheckBoxs(InfoProvider::getInstance()->getProcessInfo(), index++);
    initCPUInfoCheckBoxs(InfoProvider::getInstance()->getCPUInfo(), index++);
    initMemoryInfoCheckBoxs(InfoProvider::getInstance()->getMemoryInfo(), index++);
    initNetworkInfoCheckBoxs(InfoProvider::getInstance()->getNetworkInfo(), index++);
    initFileSystemInfoCheckBoxs(InfoProvider::getInstance()->getFileSystemInfo(), index++);

    makeConnection();
}

LogForm::~LogForm()
{
    delete ui;
    delete mLogWorker;
}

void LogForm::initBaseInfoCheckBoxs(const SystemInfo *info, int index)
{
    QVector<QCheckBox*> checkBoxs;
    checkBoxs.append(ui->chbBaseUserName);
    checkBoxs.append(ui->chbBaseUserId);
    checkBoxs.append(ui->chbBaseSystemName);
    checkBoxs.append(ui->chbBaseHostName);
    checkBoxs.append(ui->chbBaseSystemRelease);
    checkBoxs.append(ui->chbBaseSystemVersion);
    checkBoxs.append(ui->chbBaseMachineType);

    initSystemInfoCheckBoxs(info, index, checkBoxs);
}

void LogForm::initProcessInfoCheckBoxs(const SystemInfo *info, int index)
{
    QVector<QCheckBox*> checkBoxs;
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

    initSystemInfoCheckBoxs(info, index, checkBoxs);
}

void LogForm::initCPUInfoCheckBoxs(const SystemInfo *info, int index)
{
    QVector<QCheckBox*> checkBoxs;
    checkBoxs.append(ui->chbCPUCount);
    checkBoxs.append(ui->chbCPUType);
    checkBoxs.append(ui->ChbCPUUsage);

    initSystemInfoCheckBoxs(info, index, checkBoxs);
}

void LogForm::initMemoryInfoCheckBoxs(const SystemInfo *info, int index)
{
    QVector<QCheckBox*> checkBoxs;
    checkBoxs.append(ui->chbMemoryTotalRAM);
    checkBoxs.append(ui->chbMemoryFreeRAM);
    checkBoxs.append(ui->chbMemoryRAMUsage);
    checkBoxs.append(ui->chbMemoryTotalSwap);
    checkBoxs.append(ui->chbMemoryFreeSwap);
    checkBoxs.append(ui->chbMemorySwapUsage);

    initSystemInfoCheckBoxs(info, index, checkBoxs);
}

void LogForm::initNetworkInfoCheckBoxs(const SystemInfo *info, int index)
{
    QVector<QCheckBox*> checkBoxs;
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

    initSystemInfoCheckBoxs(info, index, checkBoxs);
}

void LogForm::initFileSystemInfoCheckBoxs(const SystemInfo *info, int index)
{
    QVector<QCheckBox*> checkBoxs;
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

    initSystemInfoCheckBoxs(info, index, checkBoxs);
}

void LogForm::initSystemInfoCheckBoxs(const SystemInfo *info, int index,
    const QVector<QCheckBox*> &checkBoxs)
{
    SysInfoCheckBox &sysInfoCheckBox = mSysInfoCheckBoxs[index];
    sysInfoCheckBox.sysInfo = info;
    sysInfoCheckBox.checkBoxs = checkBoxs;

    for (int i = 0; i < checkBoxs.size(); ++i)
    {
        QCheckBox *checkBox = checkBoxs[i];
        connect(checkBox, SIGNAL(toggled(bool)), this, SLOT(toggleCheckBox(bool)));
        sysInfoCheckBox.checkedHistorys.append(checkBox->isChecked());
        if (checkBox->isChecked())
        {
            mLogWorker->addFilterToSystemInfo(info, i);
        }
    }
}

void LogForm::toggleCheckBox(bool checked)
{
    for (int i = 0; i < mSysInfoCheckBoxs.size(); ++i)
    {
        SysInfoCheckBox &sysInfoCheckBox = mSysInfoCheckBoxs[i];
        for (int j = 0; j < sysInfoCheckBox.checkBoxs.size(); ++j)
        {
            bool isChecked = sysInfoCheckBox.checkBoxs[j]->isChecked();
            if (isChecked != sysInfoCheckBox.checkedHistorys[j])
            {
                if (isChecked)
                {
                    mLogWorker->addFilterToSystemInfo(sysInfoCheckBox.sysInfo, (char)j);
                }
                else
                {
                    mLogWorker->removeFilterFromSystemInfo(sysInfoCheckBox.sysInfo, (char)j);
                }
                sysInfoCheckBox.checkedHistorys[j] = isChecked;
                return;
            }
        }
    }
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
    registerToLog(checked, InfoProvider::getInstance()->getBaseInfo(),
                  tr("baseinfo.log"));
}

void LogForm::registerProcessInfo(bool checked)
{
    registerToLog(checked, InfoProvider::getInstance()->getProcessInfo(),
                  tr("processinfo.log"));
}

void LogForm::registerCPUInfo(bool checked)
{
    registerToLog(checked, InfoProvider::getInstance()->getCPUInfo(),
                  tr("cpuinfo.log"));
}

void LogForm::registerMemoryInfo(bool checked)
{
    registerToLog(checked, InfoProvider::getInstance()->getMemoryInfo(),
                  tr("memoryinfo.log"));
}

void LogForm::registerNetworkInfo(bool checked)
{
    registerToLog(checked, InfoProvider::getInstance()->getNetworkInfo(),
                  tr("networkinfo.log"));
}

void LogForm::registerFileSystemInfo(bool checked)
{
    registerToLog(checked, InfoProvider::getInstance()->getFileSystemInfo(),
                  tr("filesysteminfo.log"));
}

void LogForm::registerToLog(bool checked, const SystemInfo *info,
    const QString &logFileName)
{
    if (checked)
    {
        mLogWorker->addToWorker(info, logFileName);
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
