#include "mainwindow.h"

#include <QShortcut>
#include <QDebug>
#include <QLayout>
#include <QTableView>

#include "dotgrapgicsview.h"
#include "dotitemtablemodel.h"
#include "dotitem.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setCentralWidget(new QWidget);
    QHBoxLayout *hlay = new QHBoxLayout;
    centralWidget()->setLayout(hlay);

    tableView = new QTableView;
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setSizeAdjustPolicy(QTableView::AdjustToContents);
    tableView->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);

    graphicsView = new DotGrapgicsView;
    tableModel = new DotItemTableModel;

    graphicsView->setItemModel(tableModel);
    tableView->setModel(tableModel);

    QShortcut* shortcut = new QShortcut(QKeySequence(QKeySequence::Delete), tableView);
    connect(shortcut, &QShortcut::activated, this, &MainWindow::deleteRow);
    connect(graphicsView, &DotGrapgicsView::selectedItems, this, &MainWindow::onSelectedItems);

    hlay->addWidget(graphicsView);
    hlay->addWidget(tableView);
}

void MainWindow::deleteRow()
{
    auto selected = tableView->selectionModel()->selectedIndexes();
    if (selected.empty()) return;
    std::sort(selected.begin(), selected.end(), [](const QModelIndex &left, const QModelIndex &right)
    {
        return left.row() > right.row();
    });
    for (int i = 0; i < selected.count(); ++i)
    {
        if (selected[i].column() == 0)
        {
            tableView->model()->removeRow(selected[i].row(), selected[i]);
        }
    }
    tableView->selectionModel()->clearSelection();
}

void MainWindow::onSelectedItems(const QList<QGraphicsItem *> &items)
{
    tableView->reset();
    tableView->selectionModel()->clearSelection();

    auto selection = tableView->selectionModel();
    auto itemList = tableModel->getItemList();
    for (auto item : items)
    {
        DotItem* dot = dynamic_cast<DotItem*>(item);
        int index = itemList.indexOf(dot);
        if (index != -1)
        {
            selection->select(tableModel->index(index, 0), QItemSelectionModel::Select);
            selection->select(tableModel->index(index, 1), QItemSelectionModel::Select);
        }
    }
    tableView->setSelectionModel(selection);
}

