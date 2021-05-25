#ifndef DOTGRAPGICSVIEW_H
#define DOTGRAPGICSVIEW_H
#include <QGraphicsView>

class DotItemTableModel;

class DotItem;
class DotGrapgicsView : public QGraphicsView
{
    Q_OBJECT
public:
    DotGrapgicsView(QWidget *parent = nullptr);
    void setItemModel(DotItemTableModel *model);
    Q_SIGNAL void selectedItems(const QList<QGraphicsItem *>& indexes);

protected:
    void scaleView(qreal scaleFactor);
    void drawItems(QPainter *painter, int numItems, QGraphicsItem *itemss[], const QStyleOptionGraphicsItem options[]) override;
    void resizeEvent(QResizeEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
private:
    void createItem(const QPointF& pos);
    void drawArrow(const QPointF &start, const QPointF &end, QPainter *painter, int arrowSize = 12);

    DotItemTableModel *m_model;
};
#endif // DOTGRAPGICSVIEW_H
