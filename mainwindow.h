#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class BaseInfoForm;
class ProcessInfoForm;
class ResourcesForm;
class FileSystemInfoForm;
class LogForm;

class SystemInfoProvider;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void refreshTabs();

private: // function
    void setForm(int index, QWidget *form);

private:
    Ui::MainWindow *ui;

    SystemInfoProvider *mSystemInfoProvider;

    BaseInfoForm *mBaseInfoForm;
    ProcessInfoForm *mProcessInfoForm;
    ResourcesForm *mResourceForm;
    FileSystemInfoForm *mFileSystemInfoForm;
    LogForm *mLogForm;
};

#endif // MAINWINDOW_H
