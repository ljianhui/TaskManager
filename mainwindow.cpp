#include <QHBoxLayout>
#include <QReadLocker>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "publicreadwritelock.h"

#include "baseinfoform.h"
#include "processinfoform.h"
#include "resourcesform.h"
#include "filesysteminfoform.h"
#include "logform.h"

#include "infoprovider.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mInfoProvider(InfoProvider::getInstance()),
    mBaseInfoForm(new BaseInfoForm),
    mProcessInfoForm(new ProcessInfoForm),
    mResourceForm(new ResourcesForm),
    mFileSystemInfoForm(new FileSystemInfoForm),
    mLogForm(new LogForm)
{
    ui->setupUi(this);

    int index = 0;
    setForm(index++, mBaseInfoForm);
    setForm(index++, mProcessInfoForm);
    setForm(index++, mResourceForm);
    setForm(index++, mFileSystemInfoForm);
    setForm(index++, mLogForm);

    connect(mInfoProvider, SIGNAL(finishUpdate()), this, SLOT(refreshTabs()));
}

MainWindow::~MainWindow()
{
    delete ui;

    delete mLogForm;
    delete mFileSystemInfoForm;
    delete mResourceForm;
    delete mProcessInfoForm;
    delete mBaseInfoForm;

    delete mInfoProvider;
}

void MainWindow::setForm(int index, QWidget *form)
{
    if (index >= ui->tabWidget->count() || form == NULL)
    {
        return;
    }

    QHBoxLayout *lay = new QHBoxLayout;
    ui->tabWidget->widget(index)->setLayout(lay);

    lay->addWidget(form);
    form->show();
}

void MainWindow::refreshTabs()
{
    mBaseInfoForm->refreshViews();
    mProcessInfoForm->refreshViews();
    mResourceForm->refreshViews();
    mFileSystemInfoForm->refreshViews();
}
