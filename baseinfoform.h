#ifndef BASEINFOFORM_H
#define BASEINFOFORM_H

#include <QWidget>

namespace Ui {
class BaseInfoForm;
}

class BaseInfo;

class BaseInfoForm : public QWidget
{
    Q_OBJECT

public:
    explicit BaseInfoForm(QWidget *parent = 0);
    ~BaseInfoForm();

    void refreshViews();

private:
    Ui::BaseInfoForm *ui;

    const BaseInfo *mBaseInfo;
};

#endif // BASEINFOFORM_H
