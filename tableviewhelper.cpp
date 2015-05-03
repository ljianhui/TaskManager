#include <QTableView>
#include <QStandardItemModel>
#include <QTableWidget>
#include <QHeaderView>
#include <QStringList>
#include "tableviewhelper.h"

TableViewHelper::TableViewHelper()
{

}

TableViewHelper::~TableViewHelper()
{

}

void TableViewHelper::setTableViewStyle(QTableView *tbv)
{
    if (tbv == NULL)
    {
        return;
    }
    tbv->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tbv->setSelectionBehavior(QAbstractItemView::SelectRows);
    tbv->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tbv->verticalHeader()->hide();
    tbv->setShowGrid(false);
}

void TableViewHelper::setTableHeader(QTableView *tbv, QStandardItemModel &itemModel,
    const QStringList &strList)
{
    if (tbv == NULL)
    {
        return;
    }

    int col = 0;
    for (QStringList::const_iterator it = strList.begin();
         it != strList.end(); ++it)
    {
        itemModel.setHorizontalHeaderItem(col++, new QStandardItem(*it));
    }
    tbv->setModel(&itemModel);
}

void TableViewHelper::setTableItem(QStandardItemModel &itemModel, const QStringList &strList,
                                   int row, int col)
{
    if (row < 0)
    {
        return;
    }
    for (QStringList::const_iterator it = strList.begin();
         it != strList.end(); ++it)
    {
        itemModel.setItem(row, col++, new QStandardItem(*it));
    }
}
