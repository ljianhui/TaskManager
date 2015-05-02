#ifndef TABLEVIEWHELPER_H
#define TABLEVIEWHELPER_H

class QTableView;
class QStandardItemModel;
class QStringList;

class TableViewHelper
{
public:
    TableViewHelper();
    ~TableViewHelper();
    static void setTableViewStyle(QTableView *tbv);
    static void setTableHeader(QTableView *tbv, QStandardItemModel &itemModel,
                          const QStringList &strList);
    static void setTableItem(QStandardItemModel &itemModel, const QStringList &strList,
                             int row, int col = 0);
};

#endif // TABLEVIEWHELPER_H
