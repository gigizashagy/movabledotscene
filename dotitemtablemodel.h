#ifndef DotItemTableModel_H
#define DotItemTableModel_H
#include <QAbstractTableModel>
class DotItem;
class DotItemTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    DotItemTableModel(QObject *parent = nullptr);

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool removeRows(int row, int count, const QModelIndex &parent) override;

    Q_SLOT void addItem(DotItem *item);
    Q_SIGNAL void itemRemoved(DotItem *item);
    QList<DotItem *> getItemList() const;
private:
    QList<DotItem*> itemList;
};
#endif // DotItemTableModel_H
