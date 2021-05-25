#include "dotitemtablemodel.h"

#include <QFont>
#include <QBrush>

#include "dotitem.h"

DotItemTableModel::DotItemTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    itemList = {new DotItem(QPointF(50,50)),
                new DotItem(QPointF(50,100)),
                new DotItem(QPointF(100,50)),
                new DotItem(QPointF(100,100))};
}

int DotItemTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return itemList.count();
}

int DotItemTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

QVariant DotItemTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
    {
        auto item = itemList.at(index.row());
        if (index.column() == 0) return item->x();
        if (index.column() == 1) return item->y();
    }
    case Qt::FontRole:
        if (index.column() == 1)
        {
            QFont boldFont;
            boldFont.setBold(true);
            return boldFont;
        }
    case Qt::BackgroundRole:
    {
        if (index.column() == 1)
            return QBrush("#b5cfff");
        return QBrush("#b5ffc5");
    }
    case Qt::TextAlignmentRole:
        return int(Qt::AlignHCenter | Qt::AlignVCenter);

    }
    return QVariant();
}

bool DotItemTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole)
    {
        if (!checkIndex(index))
            return false;
        if (index.column() == 0)
            itemList[index.row()]->setX(value.toFloat());
        if (index.column() == 1)
            itemList[index.row()]->setY(value.toFloat());
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

QVariant DotItemTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return QString("X");
        case 1:
            return QString("Y");
        }
    }
    return QVariant();
}

Qt::ItemFlags DotItemTableModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
}

void DotItemTableModel::addItem(DotItem* item)
{
    itemList.append(item);
    emit layoutChanged();
}

QList<DotItem*> DotItemTableModel::getItemList() const
{
    return itemList;
}

bool DotItemTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row+count-1);
    for (auto i = 0; i < count; ++i)
    {
        auto it =  itemList.takeAt(row);
        delete it;
    }
    endRemoveRows();
    emit layoutChanged();
    return true;
}

