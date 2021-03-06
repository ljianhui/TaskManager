#include <QStandardItem>
#include <QMenu>
#include <QAction>
#include <QCursor>
#include <QContextMenuEvent>
#include <QReadLocker>
#include <unistd.h>

#include "processinfoform.h"
#include "ui_processinfoform.h"

#include "publicreadwritelock.h"

#include "prioritysettingdialog.h"
#include "tableviewhelper.h"
#include "systeminfoprovider.h"
#include "processinfo.h"
#include "processmanager.h"
#include "process.h"
#include "utils.h"

ProcessInfoForm::ProcessInfoForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProcessInfoForm),
    mItemModel(),
    mMenu(NULL),
    mProcessInfo(NULL),
    mCurProcess()
{
    ui->setupUi(this);

    mProcessInfo = SystemInfoProvider::getInstance()->getProcessInfo();

    ui->rdoAllProcess->setChecked(true);
    initTableView();
    initConnection();
    refreshViews();
}

ProcessInfoForm::~ProcessInfoForm()
{
    delete ui;
    if (mMenu != NULL)
    {
        delete mMenu;
    }
}

void ProcessInfoForm::initTableView()
{
    initTableHeader();
    TableViewHelper::setTableViewStyle(ui->tbvProcesses);
    ui->tbvProcesses->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->tbvProcesses->setContextMenuPolicy(Qt::CustomContextMenu);
}

void ProcessInfoForm::initTableHeader()
{
    QStringList headerList;
    headerList.append(tr("PID"));
    headerList.append(tr("进程名"));
    headerList.append(tr("状态"));
    headerList.append(tr("占用内存"));
    headerList.append(tr("内存占用率"));
    headerList.append(tr("CPU占用率"));
    headerList.append(tr("所属用户"));
    headerList.append(tr("PRI"));
    headerList.append(tr("Nice"));
    headerList.append(tr("命令行"));
    TableViewHelper::setTableHeader(ui->tbvProcesses, mItemModel, headerList);
}

void ProcessInfoForm::refreshViews()
{
    if ((mMenu != NULL && !mMenu->isHidden()) || mProcessInfo == NULL)
    {
        return;
    }
    QReadLocker locker(PublicReadWriteLock::getLock());
    ui->labProcessCount->setText(QString::number(mProcessInfo->getProcessCount()));
    refreshTableItems();
}

void ProcessInfoForm::initConnection()
{
    connect(ui->rdoAllProcess, SIGNAL(clicked(bool)),
            this, SLOT(showAllProcess(bool)));
    connect(ui->rdoMyProcess, SIGNAL(clicked(bool)),
            this, SLOT(showMyProcess(bool)));
    connect(ui->tbvProcesses->horizontalHeader(), SIGNAL(sectionClicked(int)),
            this, SLOT(sortProcess(int)));
    connect(ui->tbvProcesses, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(showMenu(QPoint)));
    connect(ui->tbvProcesses, SIGNAL(clicked(QModelIndex)),
            this, SLOT(setCurProcess(QModelIndex)));
    connect(ui->tbvProcesses, SIGNAL(pressed(QModelIndex)),
            this, SLOT(setCurProcess(QModelIndex)));

    connect(SystemInfoProvider::getInstance(), SIGNAL(resort()),
            this, SLOT(refreshViews()));
    connect(SystemInfoProvider::getInstance(), SIGNAL(resetUserFilter()),
            this, SLOT(refreshViews()));
}

void ProcessInfoForm::refreshTableItems()
{
    int processCount = (int)mProcessInfo->getProcessCount();
    int index = 0;
    mProcessInfo->begin();
    while (mProcessInfo->hasNext())
    {
        setTableItem(index++, mProcessInfo->next());
    }

    if (mItemModel.rowCount() > processCount)
    {
        mItemModel.removeRows(processCount, mItemModel.rowCount() - processCount);
    }
}

void ProcessInfoForm::setTableItem(int row, const Process *p)
{
    QStringList cellList;
    cellList << QString::number(p->getPid()) <<
                QString::fromStdString(p->getProcessName()) <<
                QString(QChar(p->getState())) <<
                QString::fromStdString(BytesToString(p->getMemory())) <<
                QString::fromStdString(FloatToPercent(p->getMemoryOccupyRate())) <<
                QString::fromStdString(FloatToPercent(p->getCPUOccupyRate())) <<
                QString::fromStdString(p->getUserName()) <<
                QString::number(p->getPriority()) <<
                QString::number(p->getNice()) <<
                QString::fromStdString(p->getCmdLine());

    TableViewHelper::setTableItem(mItemModel, cellList, row);
}

void ProcessInfoForm::showAllProcess(bool isShowAllProcess)
{
    if (isShowAllProcess)
    {
        SystemInfoProvider::getInstance()->resetProcessUserFilter(-1);
    }
}

void ProcessInfoForm::showMyProcess(bool isShowMyProcess)
{
    if (isShowMyProcess)
    {
        SystemInfoProvider::getInstance()->resetProcessUserFilter(getuid());
    }
}

void ProcessInfoForm::sortProcess(int index)
{
    switch (index)
    {
    case 0:
        SystemInfoProvider::getInstance()->sortProcessesByPid();
        break;
    case 1:
        SystemInfoProvider::getInstance()->sortProcessesByName();
        break;
    case 3:
    case 4:
        SystemInfoProvider::getInstance()->sortProcessesByMemory();
        break;
    case 5:
        SystemInfoProvider::getInstance()->sortProcessesByCPUOccupyRate();
        break;
    case 6:
        SystemInfoProvider::getInstance()->sortProcessesByUserName();
        break;
    case 7:
    case 8:
        SystemInfoProvider::getInstance()->sortProcessesByPriority();
        break;
    default:;
    }
}

QMenu* ProcessInfoForm::getMenu()
{
    if (mMenu == NULL)
    {
        mMenu = new QMenu(ui->tbvProcesses);
        QAction *stop = mMenu->addAction(tr("暂停"));
        QAction *resume = mMenu->addAction(tr("恢复"));
        QAction *term = mMenu->addAction(tr("终止"));
        QAction *forceStop = mMenu->addAction(tr("强制关闭"));
        QAction *setNice = mMenu->addAction(tr("设置优先级"));

        ui->tbvProcesses->addAction(stop);
        ui->tbvProcesses->addAction(resume);
        ui->tbvProcesses->addAction(term);
        ui->tbvProcesses->addAction(forceStop);
        ui->tbvProcesses->addAction(setNice);

        connect(stop, SIGNAL(triggered()), this, SLOT(stopProcess()));
        connect(resume, SIGNAL(triggered()), this, SLOT(resumeProcess()));
        connect(term, SIGNAL(triggered()), this, SLOT(termProcess()));
        connect(forceStop, SIGNAL(triggered()), this, SLOT(forceStopProcess()));
        connect(setNice, SIGNAL(triggered()), this, SLOT(setProcessNice()));
    }
    return mMenu;
}

void ProcessInfoForm::stopProcess()
{
    ProcessManager *pm = ProcessManager::getInstance();
    pm->stop(mCurProcess);
    pm->releaseInstance();
}

void ProcessInfoForm::resumeProcess()
{
    ProcessManager *pm = ProcessManager::getInstance();
    pm->resume(mCurProcess);
    pm->releaseInstance();
}

void ProcessInfoForm::termProcess()
{
    ProcessManager *pm = ProcessManager::getInstance();
    pm->terminate(mCurProcess);
    pm->releaseInstance();
}

void ProcessInfoForm::forceStopProcess()
{
    ProcessManager *pm = ProcessManager::getInstance();
    pm->forceStop(mCurProcess);
    pm->releaseInstance();
}

void ProcessInfoForm::setProcessNice()
{
    PrioritySettingDialog dialog(&mCurProcess);
    dialog.exec();
}

void ProcessInfoForm::setCurProcess(QModelIndex index)
{
    QReadLocker locker(PublicReadWriteLock::getLock());
    const Process *p = mProcessInfo->getProcess(index.row());
    if (p != NULL)
    {
        mCurProcess = *p;
    }
    else
    {
        mCurProcess = Process();
    }
}

void ProcessInfoForm::showMenu(QPoint pos)
{
    getMenu()->exec(QCursor::pos());
}
