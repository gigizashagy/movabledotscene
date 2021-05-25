#include "dotgrapgicsview.h"
#define _USE_MATH_DEFINES
#include <math.h>

#include <QGraphicsRectItem>
#include <QResizeEvent>
#include <QApplication>

#include "dotitem.h"
#include "dotitemtablemodel.h"

DotGrapgicsView::DotGrapgicsView(QWidget *parent)
    : QGraphicsView(parent), m_model(nullptr)
{
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(FullViewportUpdate);
    setCursor(Qt::ArrowCursor);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    setDragMode(QGraphicsView::RubberBandDrag);
    setOptimizationFlag(QGraphicsView::IndirectPainting);
    setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    setBackgroundBrush(QBrush(Qt::white, Qt::SolidPattern));
    scale(1,-1);
}

void DotGrapgicsView::setItemModel(DotItemTableModel *model)
{
    m_model = model;
    if (!m_model) return;
    scene()->clear();

    for (auto item : m_model->getItemList() )
        scene()->addItem(item);
}

void DotGrapgicsView::drawBackground(QPainter *painter, const QRectF &rect)
{
    QGraphicsView::drawBackground(painter, rect);
    QRectF srec = sceneRect();
    srec -= QMargins(10,10,10,10);
    drawArrow(srec.topLeft(), srec.bottomLeft(), painter);
    drawArrow(srec.topLeft(), srec.topRight(), painter);
}

void DotGrapgicsView::resizeEvent(QResizeEvent *event)
{
    setSceneRect(QRect(QPoint(), event->size()));

    QGraphicsView::resizeEvent(event);
}

void DotGrapgicsView::wheelEvent(QWheelEvent *event)
{
    scaleView(pow(2.0, event->angleDelta().y() / 450.0));
}

void DotGrapgicsView::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.9 || factor > 4)
        return;
    scale(scaleFactor, scaleFactor);
}

void DotGrapgicsView::drawArrow(const QPointF &start, const QPointF &end, QPainter *painter, int arrowSize)
{
    painter->save();
    painter->setPen(QPen(Qt::lightGray, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(start, end);

    QPolygonF arrowHead;
    auto direction = start - end;
    double angle = atan2(-direction.y(), direction.x());
    QPointF arrowP1 = end + QPointF(sin(angle + M_PI / 3) * arrowSize,
                                    cos(angle + M_PI / 3) * arrowSize);
    QPointF arrowP2 = end + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize,
                                    cos(angle + M_PI - M_PI / 3) * arrowSize);

    arrowHead << end << arrowP1 << arrowP2;
    painter->setBrush(QBrush(Qt::lightGray));
    painter->drawPolygon(arrowHead);
    painter->restore();
}

void DotGrapgicsView::mousePressEvent(QMouseEvent *event)
{
    auto selected = scene()->selectedItems();
    for (auto item : items())
    if (item->isSelected())
        item->setZValue(item->zValue()-1);
    else
        item->setZValue(0);
    QGraphicsView::mousePressEvent(event);
}

void DotGrapgicsView::mouseReleaseEvent(QMouseEvent *event)
{
    emit selectedItems(scene()->selectedItems());
    QGraphicsView::mouseReleaseEvent(event);
}

void DotGrapgicsView::drawItems(QPainter *painter, int numItems, QGraphicsItem *itemss[], const QStyleOptionGraphicsItem options[])
{
    QLinearGradient  gradient;
    auto items = m_model->getItemList();
    for (int i = 0; i < items.count() - 1; ++i)
    {
        gradient.setStart(items[i]->pos());
        gradient.setFinalStop(items[i + 1]->pos());
        gradient.setColorAt(0, items[i]->isSelected() ? DotItem::cYellow : DotItem::cBlue);
        gradient.setColorAt(1, items[i+1]->isSelected() ? DotItem::cYellow : DotItem::cBlue);

        painter->setPen(QPen(QBrush(gradient), 2));
        painter->drawLine(items[i]->pos(), items[i + 1]->pos());
    }

    QGraphicsView::drawItems(painter, numItems, itemss, options);
}

void DotGrapgicsView::mouseDoubleClickEvent(QMouseEvent *event)
{
    createItem(mapToScene(event->pos()));
}

void DotGrapgicsView::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        if (!scene()->selectedItems().isEmpty())
            m_model->layoutChanged();
    }
    QGraphicsView::mouseMoveEvent(event);
}

void DotGrapgicsView::createItem(const QPointF &pos)
{
    DotItem* item = new DotItem(pos);
    scene()->addItem(item);

    if (m_model)
        m_model->addItem(item);
}
