#include "prioritysettingdialog.h"
#include "ui_prioritysettingdialog.h"

#include "process.h"
#include "processmanager.h"

PrioritySettingDialog::PrioritySettingDialog(const Process *p, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrioritySettingDialog),
    mProcess(p),
    mNice(0)
{
    ui->setupUi(this);

    initViews();

    connect(ui->btnChange, SIGNAL(clicked()), this, SLOT(setNice()));
    connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->hsNice, SIGNAL(valueChanged(int)), this, SLOT(adjustNice(int)));
}

PrioritySettingDialog::~PrioritySettingDialog()
{
    delete ui;
}

void PrioritySettingDialog::initViews()
{
    ui->btnChange->setFocus();
    ui->hsNice->setMaximum(19);
    ui->hsNice->setMinimum(-20);
    if (mProcess != NULL)
    {
        ui->hsNice->setValue(mProcess->getNice());
    }
    else
    {
        ui->hsNice->setValue(0);
    }
    ui->labPid->setText(QString::number(mProcess->getPid()));
    ui->labNice->setText(QString::number(mProcess->getNice()));
}

void PrioritySettingDialog::adjustNice(int nice)
{
    mNice = nice;
    ui->labNice->setText(QString::number(nice));
}

void PrioritySettingDialog::setNice()
{
    if (mProcess == NULL)
    {
        return;
    }
    ProcessManager *pm = ProcessManager::getInstance();
    pm->setNice(*mProcess, mNice);
    pm->releaseInstance();
}
