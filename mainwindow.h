/****************************************************************************
*    mainwindow.h
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include "ui_mainwindow.h"
#include "drgeopoint.h"

class MainWindow : public QMainWindow, public Ui::MainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private:
  QTabWidget *drgeoTab;
  QString curDoc;
  QGraphicsScene *scene;
  QGraphicsView *view;
  drgeoPoint *point;

  void initializeTDI();

private slots:
  void drgeoClose(int index);
  void drgeoNew();
  void drgeoDrawPoint();
};

#endif // MAINWINDOW_H
