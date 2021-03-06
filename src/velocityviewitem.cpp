#include "velocityviewitem.h"
#include <src/midimanager.h>
#include <QDebug>
#include <src/trackwidget.h>
VelocityViewItem::VelocityViewItem(VelocityView *velocityView)
{
    velocity_view = velocityView;
    setCacheMode(QGraphicsItem::NoCache);
    setFlag(QGraphicsItem::ItemIgnoresTransformations,true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable,true);
}

void VelocityViewItem::notifyVelocityChanged(int velocity)
{
}

void VelocityViewItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Qt::BrushStyle style = Qt::SolidPattern;
    QBrush brush(Qt::black,style);
    QBrush brush2(QColor(102, 179, 255),style);
    painter->setPen(QPen(Qt::black,0));
    painter->setBrush(brush2);
    painter->drawPath(shape());
    painter->setBrush(brush);
    painter->drawRect(3,8,1,velocity);
}

QRectF VelocityViewItem::boundingRect() const
{
    return QRectF(0,0,30,velocity);
}

QPainterPath VelocityViewItem::shape() const
{
    QPainterPath path;
    path.addEllipse(0,0,radius,radius);
    return path;
}

void VelocityViewItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
}

void VelocityViewItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << event->lastScenePos().y();
    if (event->lastScenePos().y() < 0)
    {
        setY(0);
    }else if(event->lastScenePos().y() > viewHeight - 6)
    {
        setY(viewHeight-6);
    }
    else
    {
        setY(event->lastScenePos().y());
        velocity_view->viewport()->update();
        int OldRange = (viewHeight - 0);
        int NewRange = (127 - 1);
        int NewValue =127 - (((this->y() - 0) * NewRange) / OldRange);
        velocity = NewValue;
    }
}

void VelocityViewItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    int OldRange = (viewHeight - 0);
    int NewRange = (127 - 1);
    int NewValue =127 - (((this->y() - 0) * NewRange) / OldRange);
    velocity = NewValue;
    qDebug() << "New velocity: " << velocity;
    MidiManager::changeMidiVelocity(x(),note,velocity,velocity_view->getMidiData());
    velocity_view->viewport()->update();
}

void VelocityViewItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseDoubleClickEvent(event);
}

