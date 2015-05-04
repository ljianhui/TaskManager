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
    struct CheckBoxHistorys
    {
        QVector<QCheckBox*> checkBoxs;
        QVector<bool> historys;
    };

private: // function
    void registerAllToLog();
    void registerToLog(bool checked,
        const SystemInfo *info, const QString &logFileName,
        const CheckBoxHistorys &checkBoxHistorys);

    const SystemInfo* getSystemInfo(int index);

    void initBaseInfoCheckBoxs();
    void initProcessInfoCheckBoxs();
    void initCPUInfoCheckBoxs();
    void initMemoryInfoCheckBoxs();
    void initNetworkInfoCheckBoxs();
    void initFileSystemInfoCheckBoxs();

    void initCheckedHistory(const QVector<QCheckBox*> &checkBoxs,
        QVector<bool> &checkedHistorys);

    void makeConnection();

private: // data
    const static int INDEX_BASE_INFO;
    const static int INDEX_PROCESS_INFO;
    const static int INDEX_CPU_INFO;
    const static int INDEX_MEMORY_INFO;
    const static int INDEX_NETWORK_INFO;
    const static int INDEX_FILESYSTEM_INFO;

    Ui::LogForm *ui;

    QVector<CheckBoxHistorys> mCheckBoxHistorysVec;
    LogWorker *mLogWorker;
};

#endif // LOGFORM_H
