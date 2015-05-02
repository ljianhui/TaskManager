#ifndef PROCESSINFOFORM_H
#define PROCESSINFOFORM_H

#include <QWidget>
#include <QStandardItemModel>
#include <QPoint>
#include <QModelIndex>

namespace Ui {
class ProcessInfoForm;
}

class ProcessInfo;
class Process;
class QMenu;

class ProcessInfoForm : public QWidget
{
    Q_OBJECT

public:
    explicit ProcessInfoForm(QWidget *parent = 0);
    ~ProcessInfoForm();

public slots:
    void refreshViews();

private: // function
    void initTableView();
    void initTableHeader();

    void refreshTableItems();
    void setTableItem(int index, const Process* p);

    void initConnection();
    QMenu* getMenu();

private slots: //slot
    void showAllProcess(bool isShowAllProcess);
    void showMyProcess(bool isShowMyProcess);
    void sortProcess(int index);
    void showMenu(QPoint pos);
    void stopProcess();
    void resumeProcess();
    void termProcess();
    void forceStopProcess();
    void setProcessNice();
    void setCurIndex(QModelIndex index);

private: // data
    Ui::ProcessInfoForm *ui;

    QStandardItemModel mItemModel;
    QModelIndex mCurModelIndex;
    QMenu *mMenu;
    const ProcessInfo *mProcessInfo;
};

#endif // PROCESSINFOFORM_H
