/****************************************************************************
*    drgeopoint.h
*
*    Dr. Geo: An interactive Geometry Software
*
*    Copyright (C) 2014  Gurjot Singh <bhattigurjot@gmail.com>
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*****************************************************************************/

#ifndef DRGEOPOINT_H
#define DRGEOPOINT_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>

class drgeoPoint : public QObject, public QGraphicsItem
{
  Q_OBJECT
public:
  explicit drgeoPoint(QObject *parent = 0);
  QRectF boundingRect() const;
  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *event);

signals:
  void drawFinished();

private:
  int coordX, coordY;
  bool drgeoClick, drgeoPaintFlag;
};

#endif // DRGEOPOINT_H
