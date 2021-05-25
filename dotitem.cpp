#include "dotitem.h"
#include <QPainter>

const QColor DotItem::cBlue ("#5b9bd5");
const QColor DotItem::cYellow ("#ffc000");

DotItem::DotItem(const QPointF& pos)
    : m_diametr(6.f), m_hovered(false)
{
    setPos(pos);
    setAcceptHoverEvents(true);
    setFlags(ItemIsMovable | ItemIsSelectable | ItemIsFocusable);
}

QRectF DotItem::boundingRect() const
{
    auto size = m_diametr * 1.5f;
    return QRectF(-size, -size, size*2, size*2);
}

QPainterPath DotItem::shape() const
{
    QPainterPath shp;
    shp.addEllipse(QPointF(), m_diametr, m_diametr);
    return shp;
}

void DotItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (m_hovered)
       drawHovered(painter);
    drawShape(painter);

    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void DotItem::drawHovered(QPainter *painter)
{
    QColor color = isSelected() ? cYellow : cBlue;
    auto scaled = m_diametr * 1.5;
    QRadialGradient gradient(QPointF(), scaled);
    gradient.setFocalPoint(0, 0);
    gradient.setFocalRadius(m_diametr);
    gradient.setColorAt(0, color);
    gradient.setColorAt(1, Qt::transparent);
    painter->setBrush(QBrush(gradient));
    painter->setPen(QPen(Qt::transparent));
    painter->drawEllipse(QPointF(), scaled, scaled);
}

void DotItem::drawShape(QPainter *painter)
{
    QColor color = isSelected() ? cYellow : cBlue;
    auto penWidth = m_diametr/2;
    painter->setBrush(QBrush(color));
    painter->setPen(QPen(color.darker(125), penWidth));
    painter->drawEllipse(QPointF(), m_diametr - penWidth/2, m_diametr - penWidth/2);
}

void DotItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    m_hovered = true;
    QGraphicsItem::hoverEnterEvent(event);
}

void DotItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_hovered = false;
    QGraphicsItem::hoverLeaveEvent(event);
}
