#ifndef LOGFORM_H
#define LOGFORM_H

#include <QVector>
#include <QWidget>

namespace Ui {
class LogForm;
}

class Log;
class SystemInfo;
class LogWorker;
class QCheckBox;

class LogForm : public QWidget
{
    Q_OBJECT

public:
    explicit LogForm(QWidget *parent = 0);
    ~LogForm();

private slots:
    void toggleCheckBox(bool checked);

    void registerBaseInfo(bool checked);
    void registerProcessInfo(bool checked);
    void registerCPUInfo(bool checked);
    void registerMemoryInfo(bool checked);
    void registerNetworkInfo(bool checked);
    void registerFileSystemInfo(bool checked);

    void toggleLogWorker(bool start);
    void setFrqForLogWorker(int index);

private: // struct
    struct SysInfoCheckBox
    {
        const SystemInfo *sysInfo;
        QVector<QCheckBox*> checkBoxs;
        QVector<bool> checkedHistorys;
    };

private: // function
    void registerAllToLog();

    void registerToLog(bool checked, const SystemInfo *info,
        const QString &logFileName);

    void initBaseInfoCheckBoxs(const SystemInfo *info, int index);
    void initProcessInfoCheckBoxs(const SystemInfo *info, int index);
    void initCPUInfoCheckBoxs(const SystemInfo *info, int index);
    void initMemoryInfoCheckBoxs(const SystemInfo *info, int index);
    void initNetworkInfoCheckBoxs(const SystemInfo *info, int index);
    void initFileSystemInfoCheckBoxs(const SystemInfo *info, int index);

    void initSystemInfoCheckBoxs(const SystemInfo *info, int index,
        const QVector<QCheckBox*> &checkBoxs);

    void makeConnection();

private: // data
    Ui::LogForm *ui;

    QVector<SysInfoCheckBox> mSysInfoCheckBoxs;
    LogWorker *mLogWorker;
};

#endif // LOGFORM_H
