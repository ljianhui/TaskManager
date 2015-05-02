#ifndef RESOURCESFORM_H
#define RESOURCESFORM_H

#include <QWidget>
#include <QStandardItemModel>

namespace Ui {
class ResourcesForm;
}

class QTableView;

class CPUInfo;
class CPU;
class MemoryInfo;
class NetworkInfo;

class ResourcesForm : public QWidget
{
    Q_OBJECT

public:
    explicit ResourcesForm(QWidget *parent = 0);
    ~ResourcesForm();

    void refreshViews();

private:
    void initCPUTableView();
    void initCPUTableHeader();

    void initMemoryProgressBars();

    void initNetTableView();
    void initNetTableHeader();

    void refreshCPUViews();
    void refreshCPUTableView();
    void setCPUTableItem(int index, const CPU *cpu);

    void refreshMemoryViews();

    void refreshNetworkViews();
    void refreshNetTableView();

private:
    Ui::ResourcesForm *ui;

    QStandardItemModel mCPUItemModel;
    QStandardItemModel mNetItemModel;

    const CPUInfo *mCPUInfo;
    const MemoryInfo *mMemoryInfo;
    const NetworkInfo *mNetworkInfo;
};

#endif // RESOURCESFORM_H
