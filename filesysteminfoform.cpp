#include <QReadLocker>
#include "filesysteminfoform.h"
#include "ui_filesysteminfoform.h"

#include "publicreadwritelock.h"

#include "tableviewhelper.h"
#include "infoprovider.h"
#include "filesysteminfo.h"
#include "filesystem.h"
#include "utils.h"

FileSystemInfoForm::FileSystemInfoForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileSystemInfoForm),
    mFileSystemInfo(NULL)
{
    ui->setupUi(this);

    mFileSystemInfo = InfoProvider::getInstance()->getFileSystemInfo();

    initTableView();
    refreshViews();
}

FileSystemInfoForm::~FileSystemInfoForm()
{
    delete ui;
}

void FileSystemInfoForm::initTableView()
{
    initTableHeader();
    TableViewHelper::setTableViewStyle(ui->tbvFileSystem);
}

void FileSystemInfoForm::initTableHeader()
{
    QStringList headerList;
    headerList << tr("挂载") << tr("设备") << tr("目录")
               << tr("类型") << tr("容量") << tr("可用")
               << tr("已用") << tr("块大小") << tr("总块数")
               << tr("可用块数") << tr("UUID") << tr("Label");
    TableViewHelper::setTableHeader(ui->tbvFileSystem, mItemModel, headerList);
}

void FileSystemInfoForm::refreshViews()
{
    if (mFileSystemInfo == NULL)
    {
        return;
    }
    QReadLocker locker(PublicReadWriteLock::getLock());
    addMountFileSystems(0);
    addUnmountFileSystems((int)mFileSystemInfo->getMountFileSystemCount());

    int fsCount = (int)mFileSystemInfo->getMountFileSystemCount() +
            (int)mFileSystemInfo->getUnmountFileSystemCount();
    if (mItemModel.rowCount() > fsCount)
    {
        mItemModel.removeRows(fsCount, mItemModel.rowCount() - fsCount);
    }
}

void FileSystemInfoForm::addMountFileSystems(int row)
{
    if (mFileSystemInfo == NULL)
    {
        return;
    }
    mFileSystemInfo->beginMountFileSystems();
    while (mFileSystemInfo->hasNextMountFileSystem())
    {
        setTableItem(row++, mFileSystemInfo->nextMountFileSystem());
    }
}

void FileSystemInfoForm::addUnmountFileSystems(int row)
{
    if (mFileSystemInfo == NULL)
    {
        return;
    }
    mFileSystemInfo->beginUnMountFileSystems();
    while (mFileSystemInfo->hasNextUnmountFileSystem())
    {
        setTableItem(row++, mFileSystemInfo->nextUnmountFileSystem());
    }
}

void FileSystemInfoForm::setTableItem(int row, const FileSystem *fs)
{
    int col = 0;
    mItemModel.setItem(row, col++, getCheckStateItem(fs));
    QStringList cellList;
    cellList << QString::fromStdString(fs->getDeviceName())
             << QString::fromStdString(fs->getMountDir())
             << QString::fromStdString(fs->getMountType())
             << QString::fromStdString(BytesToString(fs->getTotalBytes()))
             << QString::fromStdString(BytesToString(fs->getFreeBytes()))
             << QString::fromStdString(BytesToString(fs->getTotalBytes() - fs->getFreeBytes())) +
                tr("(") + QString::fromStdString(FloatToPercent(fs->getUsage())) + tr(")")
             << QString::fromStdString(BytesToString(fs->getBlockSize()))
             << QString::number(fs->getBlockCount())
             << QString::number(fs->getBlockFree())
             << QString::fromStdString(fs->getUuid())
             << QString::fromStdString(fs->getLabel());
    TableViewHelper::setTableItem(mItemModel, cellList, row, col);
}

QStandardItem* FileSystemInfoForm::getCheckStateItem(const FileSystem *fs)
{
    QStandardItem *item = new QStandardItem();
    item->setCheckable(true);
    item->setEnabled(false);
    if (fs->isMounted())
    {
        item->setCheckState(Qt::Checked);
        item->setText(tr("已挂载"));
    }
    else
    {
        item->setCheckState(Qt::Unchecked);
        item->setText(tr("未挂载"));
    }
    return item;
}
