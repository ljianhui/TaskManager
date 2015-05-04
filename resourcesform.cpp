#include <QReadLocker>
#include "resourcesform.h"
#include "ui_resourcesform.h"

#include "publicreadwritelock.h"

#include "tableviewhelper.h"
#include "systeminfoprovider.h"
#include "cpuinfo.h"
#include "cpu.h"
#include "memoryinfo.h"
#include "networkinfo.h"
#include "utils.h"

ResourcesForm::ResourcesForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResourcesForm),
    mCPUInfo(NULL),
    mMemoryInfo(NULL),
    mNetworkInfo(NULL)
{
    ui->setupUi(this);

    mCPUInfo = SystemInfoProvider::getInstance()->getCPUInfo();
    mMemoryInfo = SystemInfoProvider::getInstance()->getMemoryInfo();
    mNetworkInfo = SystemInfoProvider::getInstance()->getNetworkInfo();

    initCPUTableView();
    initMemoryProgressBars();
    initNetTableView();
    refreshViews();
}

ResourcesForm::~ResourcesForm()
{
    delete ui;
}

void ResourcesForm::initCPUTableView()
{
    initCPUTableHeader();
    TableViewHelper::setTableViewStyle(ui->tbvCPU);
}

void ResourcesForm::initCPUTableHeader()
{
    QStringList headerList;
    headerList << tr("ID") << tr("使用率") << tr("总时间") <<
                  tr("IDLE") << tr("UTime") << tr("STime") <<
                  tr("NTime") << tr("IOWait") << tr("IRQ") <<
                  tr("SoftIRQ");
    TableViewHelper::setTableHeader(ui->tbvCPU, mCPUItemModel, headerList);
}

void ResourcesForm::refreshViews()
{
    QReadLocker locker(PublicReadWriteLock::getLock());
    refreshCPUViews();
    refreshMemoryViews();
    refreshNetworkViews();
}

void ResourcesForm::refreshCPUViews()
{
    if (mCPUInfo == NULL)
    {
        return;
    }
    const CPU *summary = mCPUInfo->getCPUSummary();
    if (summary != NULL)
    {
        ui->labCPUUsage->setText(
            QString::fromStdString(FloatToPercent(summary->getUsage())));
    }
    ui->labCPUCount->setText(QString::number(mCPUInfo->getCPUCount()));
    ui->labCPUType->setText(QString::fromStdString(mCPUInfo->getCPUType()));
    refreshCPUTableView();
}

void ResourcesForm::refreshCPUTableView()
{
    mCPUInfo->begin();
    int index = 0;
    while (mCPUInfo->hasNext())
    {
        const CPU *cpu = mCPUInfo->next();
        setCPUTableItem(index++, cpu);
    }
}

void ResourcesForm::setCPUTableItem(int row, const CPU *cpu)
{
    QStringList cellList;
    cellList << tr("CPU") + QString::number(cpu->getId()) <<
                QString::fromStdString(FloatToPercent(cpu->getUsage())) <<
                QString::number(cpu->getTotalTime()) <<
                QString::number(cpu->getIdleTime()) <<
                QString::number(cpu->getUserTime()) <<
                QString::number(cpu->getSystemTime()) <<
                QString::number(cpu->getNiceTime()) <<
                QString::number(cpu->getIowaitTime()) <<
                QString::number(cpu->getIrqTime()) <<
                QString::number(cpu->getSoftirqTime());

    TableViewHelper::setTableItem(mCPUItemModel, cellList, row);
}

void ResourcesForm::initMemoryProgressBars()
{
    ui->psbMemory->setMinimum(0);
    ui->psbMemory->setMaximum(1000);

    ui->psbSwap->setMinimum(0);
    ui->psbSwap->setMaximum(1000);
}

void ResourcesForm::refreshMemoryViews()
{
    if (mMemoryInfo == NULL)
    {
        return;
    }

    ui->psbMemory->setValue(mMemoryInfo->getRAMUsage() * 1000);
    ui->psbSwap->setValue(mMemoryInfo->getSwapUsage() * 1000);

    unsigned long totalRAM = mMemoryInfo->getTotalRAM();
    unsigned long usedRAM = totalRAM - mMemoryInfo->getFreeRAM();
    unsigned long totalSwap = mMemoryInfo->getTotalSwap();
    unsigned long usedSwap = totalSwap - mMemoryInfo->getFreeSwap();

    QString strTotalRAM = QString::fromStdString(BytesToString(totalRAM));
    QString strUsedRAM = QString::fromStdString(BytesToString(usedRAM));
    QString strTotalSwap = QString::fromStdString(BytesToString(totalSwap));
    QString strUsedSwap = QString::fromStdString(BytesToString(usedSwap));
    QString strPageSize = QString::fromStdString(BytesToString(mMemoryInfo->getPageSize()));
    ui->labMemory->setText(strUsedRAM + tr("/") + strTotalRAM);
    ui->labSwap->setText(strUsedSwap + tr("/") + strTotalSwap);
    ui->labPageSize->setText(strPageSize);
}

void ResourcesForm::initNetTableView()
{
    TableViewHelper::setTableViewStyle(ui->tbvNetwork);
    initNetTableHeader();
}

void ResourcesForm::initNetTableHeader()
{
    QStringList headerList;
    headerList << tr("方向") << tr("数据量") << tr("包数") <<
                  tr("错误") << tr("错误率") << tr("丢弃") << tr("丢弃率");
    TableViewHelper::setTableHeader(ui->tbvNetwork, mNetItemModel, headerList);
}

void ResourcesForm::refreshNetworkViews()
{
    if (mNetworkInfo == NULL)
    {
        return;
    }
    QString receivingRate = QString::fromStdString(BytesToString(mNetworkInfo->getReceivingRate()));
    receivingRate += "/s";
    QString sendingRate = QString::fromStdString(BytesToString(mNetworkInfo->getSendingRate()));
    sendingRate += "/s";
    ui->labReceivingRate->setText(receivingRate);
    ui->labSendingRate->setText(sendingRate);
    refreshNetTableView();
}

void ResourcesForm::refreshNetTableView()
{

    QStringList cellList;
    cellList << tr("下载") <<
                QString::fromStdString(BytesToString(mNetworkInfo->getReceivedBytes())) <<
                QString::number(mNetworkInfo->getReceivedPackets()) <<
                QString::number(mNetworkInfo->getReceivedErrors()) <<
                QString::fromStdString(FloatToPercent(mNetworkInfo->getReceivedErrorRate())) <<
                QString::number(mNetworkInfo->getReceivedDrop()) <<
                QString::fromStdString(FloatToPercent(mNetworkInfo->getReceivedDropRate()));
    TableViewHelper::setTableItem(mNetItemModel, cellList, 0);

    cellList.clear();
    cellList << tr("上传") <<
                QString::fromStdString(BytesToString(mNetworkInfo->getSentBytes())) <<
                QString::number(mNetworkInfo->getSentPackets()) <<
                QString::number(mNetworkInfo->getSentErrors()) <<
                QString::fromStdString(FloatToPercent(mNetworkInfo->getSentErrorRate())) <<
                QString::number(mNetworkInfo->getSentDrop()) <<
                QString::fromStdString(FloatToPercent(mNetworkInfo->getSentDropRate()));
    TableViewHelper::setTableItem(mNetItemModel, cellList, 1);
}
