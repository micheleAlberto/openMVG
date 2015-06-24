// Copyright (c) 2015 Pierre MOULON.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "node.hpp"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>

#include <iostream>

control_point_2DNode::control_point_2DNode(const QPointF& pos, double & x, double & y, size_t id_control_point)
  :_x(x), _y(y), _id_control_point(id_control_point)
{
  setFlags(ItemIsMovable | ItemIsSelectable);
  setFlag(ItemSendsGeometryChanges);
  setCacheMode(DeviceCoordinateCache);
  setZValue(1.);
  setPos(pos);
}


QRectF control_point_2DNode::boundingRect() const {
  const qreal adjust = 2;
  return QRectF( -10 - adjust, -10 - adjust,
    20 + adjust, 20 + adjust);
}

QPainterPath control_point_2DNode::shape() const  {
  QPainterPath path;
  path.addEllipse(QPointF(0,0),10,10);
  return path;
}

void control_point_2DNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  QRadialGradient gradient(-3, -3, 10);
  if (isSelected())  {
    gradient.setCenter(3, 3);
    gradient.setFocalPoint(3, 3);
    gradient.setColorAt(1, QColor(Qt::yellow).light(120));
    gradient.setColorAt(0, QColor(Qt::darkYellow).light(120));
  }
  else  {
    gradient.setColorAt(0, Qt::yellow);
    gradient.setColorAt(1, Qt::darkYellow);
  }
  painter->setBrush(gradient);
  painter->setPen(QPen(Qt::black, 0));
  painter->drawEllipse(QPointF(0,0),10,10);

  // Or just draw a cross ?
  //painter->setPen(QPen(Qt::black, 1));
  //painter->drawLine(10, 10, -10, -10);
  //painter->drawLine(10, -10, -10, 10);

  // Paint the index of the control_point
  painter->setFont(QFont("Arial", 15));
  painter->setPen(QPen(Qt::black, 5));
  painter->drawText(-5, 8, QString::number(_id_control_point));
}

QVariant control_point_2DNode::itemChange(GraphicsItemChange change, const QVariant &value)
{
  const QVariant variant = QGraphicsItem::itemChange(change, value);
  _x = scenePos().x();
  _y = scenePos().y();
  return variant;
}

void control_point_2DNode::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  update();
  QGraphicsItem::mousePressEvent(event);
}
void control_point_2DNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
  update();
  QGraphicsItem::mouseReleaseEvent(event);
}

