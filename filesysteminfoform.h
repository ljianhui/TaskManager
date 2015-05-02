#ifndef FILESYSTEMINFOFORM_H
#define FILESYSTEMINFOFORM_H

#include <QWidget>
#include <QStandardItemModel>

namespace Ui {
class FileSystemInfoForm;
}

class FileSystemInfo;
class FileSystem;

class FileSystemInfoForm : public QWidget
{
    Q_OBJECT

public:
    explicit FileSystemInfoForm(QWidget *parent = 0);
    ~FileSystemInfoForm();

    void refreshViews();

private:
    void initTableView();
    void initTableHeader();

    void addMountFileSystems(int row);
    void addUnmountFileSystems(int row);
    void setTableItem(int row, const FileSystem *fs);
    QStandardItem* getCheckStateItem(const FileSystem *fs);

private:
    Ui::FileSystemInfoForm *ui;

    QStandardItemModel mItemModel;
    const FileSystemInfo *mFileSystemInfo;
};

#endif // FILESYSTEMINFOFORM_H
