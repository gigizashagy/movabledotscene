#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class DotGrapgicsView;
class DotItemTableModel;
class QTableView;
class QGraphicsItem;
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

private:
    void deleteRow();
    Q_SLOT void onSelectedItems(const QList<QGraphicsItem *>& items);

    QTableView *tableView;

    DotGrapgicsView *graphicsView;
    DotItemTableModel *tableModel;
};
#endif // MAINWINDOW_H
