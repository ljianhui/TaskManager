#ifndef PRIORITYSETTINGDIALOG_H
#define PRIORITYSETTINGDIALOG_H

#include <QDialog>

namespace Ui {
class PrioritySettingDialog;
}

class Process;

class PrioritySettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PrioritySettingDialog(const Process *p, QWidget *parent = 0);
    ~PrioritySettingDialog();

private slots:
    void adjustNice(int nice);
    void setNice();

private:
    void initViews();

private:
    Ui::PrioritySettingDialog *ui;

    const Process *mProcess;
    int mNice;
};

#endif // PRIORITYSETTINGDIALOG_H
