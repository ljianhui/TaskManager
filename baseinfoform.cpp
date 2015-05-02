#include <QReadLocker>
#include "baseinfoform.h"
#include "ui_baseinfoform.h"
#include "publicreadwritelock.h"

#include "baseinfo.h"
#include "infoprovider.h"

BaseInfoForm::BaseInfoForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BaseInfoForm)
{
    ui->setupUi(this);

    mBaseInfo = InfoProvider::getInstance()->getBaseInfo();
    refreshViews();
}

BaseInfoForm::~BaseInfoForm()
{
    delete ui;
}

void BaseInfoForm::refreshViews()
{
    if (mBaseInfo == NULL)
    {
        return;
    }

    QReadLocker locker(PublicReadWriteLock::getLock());
    ui->labUserName->setText(QString::fromStdString(mBaseInfo->getUserName()));
    ui->labUserId->setText(QString::number(mBaseInfo->getUserId()));
    ui->labSystemName->setText(QString::fromStdString(mBaseInfo->getSystemName()));
    ui->labHostName->setText(QString::fromStdString(mBaseInfo->getHostName()));
    ui->labSystemRelease->setText(QString::fromStdString(mBaseInfo->getSystemRelease()));
    ui->labSystemVersion->setText(QString::fromStdString(mBaseInfo->getSystemVersion()));
    ui->labMachineType->setText(QString::fromStdString(mBaseInfo->getMachineType()));
}
