/****************************************************************************
*    drgeopoint.cpp
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

#include "drgeopoint.h"

drgeoPoint::drgeoPoint(QObject *parent) :
  QObject(parent)
{
  drgeoClick = true;
  drgeoPaintFlag = false;
}

QRectF drgeoPoint::boundingRect() const
{
  return QRect(0, 0, 1300, 550);
}

void drgeoPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  if(drgeoPaintFlag){
      QPen paintpen(Qt::red);
      paintpen.setWidth(4);

      QPoint p1;
      p1.setX(coordX);
      p1.setY(coordY);

      painter->setPen(paintpen);
      painter->drawPoint(p1);
    }
}

void drgeoPoint::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  if(event->button()==Qt::LeftButton)
    {
      if(drgeoClick)
        {
          coordX = event->pos().x();
          coordY = event->pos().y();
          drgeoClick = false;
          drgeoPaintFlag = true;
          update();
          emit drawFinished(); //This signal enables drawing of multiple times.
        }
    }
}
