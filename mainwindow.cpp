/****************************************************************************
*    mainwindow.cpp
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

#include "mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent)
{
  setupUi(this); //Sets the user interface
  initializeTDI(); //Adds the TDI
}

MainWindow::~MainWindow()
{
}

void MainWindow:: initializeTDI()
{
  /** Creates TDI **/
  drgeoTab = new QTabWidget(centerWidget);
  drgeoTab->setFixedSize(1300,550);
  drgeoTab->setDocumentMode(true);
  drgeoTab->setTabsClosable(true);
  drgeoTab->setMovable(true);
  drgeoTab->setFocusPolicy(Qt::StrongFocus);
  drgeoNew();

  /** Signals **/
  connect(actionNew, SIGNAL(triggered()), this, SLOT(drgeoNew()));
  connect(actionQuit, SIGNAL(triggered()), this, SLOT(close()));
  connect(actionPoint, SIGNAL(triggered()), this, SLOT(drgeoDrawPoint()));
  connect(drgeoTab, SIGNAL(tabCloseRequested(int)), this, SLOT(drgeoClose(int)));
}

/** Actions **/

void MainWindow::drgeoNew() //Creates new tab
{
  static int docNumber = 1;
  curDoc = tr("Document %1").arg(docNumber++);
  scene = new QGraphicsScene;
  view = new QGraphicsView;
  view->setScene(scene);
  drgeoTab->addTab(view, curDoc);
}

void MainWindow::drgeoClose(int index) //Closes the current tab
{
  bool close = true;
  if(close) {
      drgeoTab->removeTab(index);
    }
}

void MainWindow::drgeoDrawPoint() //Draws point
{
  point = new drgeoPoint;
  scene->addItem(point);
  connect(point, SIGNAL(drawFinished()), this, SLOT(drgeoDrawPoint()));
}
