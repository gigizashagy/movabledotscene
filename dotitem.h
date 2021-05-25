#ifndef DOTITEM_H
#define DOTITEM_H
#include <QGraphicsItem>

class DotItem : public QGraphicsItem
{
public:
    const static QColor cBlue;
    const static QColor cYellow;
    enum CustomType
    {
        dotItem = UserType + 1
    };

    DotItem(const QPointF& pos);
    int type() const override { return dotItem; }

    // QGraphicsItem interface
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    void drawHovered(QPainter *painter);
    void drawShape(QPainter *painter);

    // QGraphicsItem interface
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
private:
    const float m_diametr;
    bool m_hovered;
};

#endif // DOTITEM_H
