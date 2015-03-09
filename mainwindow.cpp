#include "mainwindow.hpp"
#include "nodes.hpp"
#include "commands.hpp"
#include <iostream>
#include <QMouseEvent>
#include <QDockWidget>
#include <QUndoStack>
#include <QMenuBar>
#include <QMenu>
#include <QMimeData>
#include <QDrag>

DialogueView::DialogueView(QGraphicsScene* scene, MainWindow* parent)
  : QGraphicsView(scene, parent)
{
  setMinimumSize(640, 480);
  setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
  setDragMode(QGraphicsView::RubberBandDrag);
  setAlignment(Qt::AlignLeft | Qt::AlignTop);
  setTransformationAnchor(QGraphicsView::NoAnchor);
  setResizeAnchor(QGraphicsView::NoAnchor);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

Node* DialogueView::dragConnection(Node* node)
{
  nodeConnectionFrom = node;
  nodeConnectionTo = 0;

  QByteArray data;
  QDataStream stream(&data, QIODevice::WriteOnly);
  stream << (quintptr)this;

  QMimeData* mimeData = new QMimeData();
  mimeData->setData("application/dialoguenode-connection", data);
  QDrag* drag = new QDrag(this);
  drag->setMimeData(mimeData);

  drag->exec(Qt::LinkAction);

  nodeConnectionFrom = 0;
  return nodeConnectionTo;
}

Node* DialogueView::connectFrom()
{
  return nodeConnectionFrom;
}

void DialogueView::connectTo(Node* node)
{
  nodeConnectionTo = node;
}

void DialogueView::nodeMoveEvent(MoveCommand* movement)
{
  emit(nodeMoved(movement));
}

void DialogueView::nodeConnectEvent(ConnectCommand* connection)
{
  emit(nodeConnected(connection));
}

void DialogueView::mousePressEvent(QMouseEvent* event)
{
  if (event->button() == Qt::RightButton)
  {
    QMouseEvent fake(event->type(), event->pos(), Qt::LeftButton, Qt::LeftButton, event->modifiers());
    setDragMode(QGraphicsView::ScrollHandDrag);
    QGraphicsView::mousePressEvent(&fake);
  }
  else
  {
    QGraphicsView::mousePressEvent(event);
  }
}

void DialogueView::mouseReleaseEvent(QMouseEvent* event)
{
  if (event->button() == Qt::RightButton)
  {
    QMouseEvent fake(event->type(), event->pos(), Qt::LeftButton, Qt::LeftButton, event->modifiers());
    QGraphicsView::mouseReleaseEvent(&fake);
    setDragMode(QGraphicsView::RubberBandDrag);
  }
  else
  {
    qobject_cast<MainWindow*>(parent())->updateSceneRect();
    QGraphicsView::mouseReleaseEvent(event);
  }
}

void DialogueView::resizeEvent(QResizeEvent* event)
{
  qobject_cast<MainWindow*>(parent())->updateSceneRect();
  QGraphicsView::resizeEvent(event);
}

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
{
  undoStack = new QUndoStack(this);

  createActions();
  createMenus();
  createDocks();

  scene = new QGraphicsScene(this);

  view = new DialogueView(scene, this);
  connect(view, SIGNAL(nodeMoved(MoveCommand*)), this, SLOT(nodeMoved(MoveCommand*)));
  connect(view, SIGNAL(nodeConnected(ConnectCommand*)), this, SLOT(nodeConnected(ConnectCommand*)));
  setCentralWidget(view);

  auto node0 = new TextNode(view);
  node0->setPos(50, 60);
  scene->addItem(node0);

  auto node1 = new TextNode(view);
  node1->setPos(300, 60);
  node0->setConnection(0, node1);
  scene->addItem(node1);

  auto node2 = new TextNode(view);
  node2->setPos(500, 60);
  node1->setConnection(0, node2);
  scene->addItem(node2);

  auto node3 = new TextNode(view);
  node3->setPos(300, 200);
  node3->setConnection(0, node2);
  scene->addItem(node3);

  updateSceneRect();
}

void MainWindow::createActions()
{
  openAction = new QAction(tr("&Open"), this);
  openAction->setShortcuts(QKeySequence::Open);
  connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

  saveAction = new QAction(tr("&Save"), this);
  saveAction->setShortcuts(QKeySequence::Save);
  connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

  saveAsAction = new QAction(tr("Save &As"), this);
  saveAsAction->setShortcuts(QKeySequence::SaveAs);
  connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));

  exportAction = new QAction(tr("&Export"), this);
  connect(exportAction, SIGNAL(triggered()), this, SLOT(exportFile()));

  quitAction = new QAction(tr("&Quit"), this);
  quitAction->setShortcuts(QKeySequence::Quit);
  connect(quitAction, SIGNAL(triggered()), this, SLOT(quit()));

  deleteAction = new QAction(tr("&Delete Nodes"), this);
  connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteItem()));

  deleteLooseAction = new QAction(tr("Delete &Loose Nodes"), this);
  connect(deleteLooseAction, SIGNAL(triggered()), this, SLOT(deleteItem()));

  addTextNodeAction = new QAction(tr("Add &Text Node"), this);
  connect(addTextNodeAction, SIGNAL(triggered()), this, SLOT(addTextNode()));

  undoAction = undoStack->createUndoAction(this, tr("&Undo"));
  undoAction->setShortcuts(QKeySequence::Undo);

  redoAction = undoStack->createRedoAction(this, tr("&Redo"));
  redoAction->setShortcuts(QKeySequence::Redo);
}

void MainWindow::createMenus()
{
  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(openAction);
  fileMenu->addAction(saveAction);
  fileMenu->addAction(saveAsAction);
  fileMenu->addSeparator();
  fileMenu->addAction(exportAction);
  fileMenu->addSeparator();
  fileMenu->addAction(quitAction);

  editMenu = menuBar()->addMenu(tr("&Edit"));
  editMenu->addAction(undoAction);
  editMenu->addAction(redoAction);
  editMenu->addSeparator();
  editMenu->addAction(deleteAction);
  editMenu->addAction(deleteLooseAction);
}

void MainWindow::createDocks()
{
  overviewWidget = new QDockWidget(tr("Overview"), this);
  overviewWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  overviewWidget->setMinimumWidth(200);
  overviewWidget->setFeatures(QDockWidget::DockWidgetMovable);
  addDockWidget(Qt::RightDockWidgetArea, overviewWidget);

  propertiesWidget = new QDockWidget(tr("Properties"), this);
  propertiesWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  propertiesWidget->setMinimumWidth(200);
  propertiesWidget->setFeatures(QDockWidget::DockWidgetMovable);
  addDockWidget(Qt::RightDockWidgetArea, propertiesWidget);
}

void MainWindow::addTextNode()
{
}

void MainWindow::deleteItem()
{
  if (scene->selectedItems().isEmpty())
  {
    return;
  }
}

void MainWindow::nodeMoved(MoveCommand* movement)
{
  undoStack->push(movement);
}

void MainWindow::nodeConnected(ConnectCommand* connection)
{
  undoStack->push(connection);
}

void MainWindow::open()
{
}

void MainWindow::save()
{
}

void MainWindow::saveAs()
{
}

void MainWindow::exportFile()
{
}

void MainWindow::quit()
{
   close();
}

void MainWindow::updateSceneRect()
{
  QRectF size = view->viewport()->rect();
  size = size.marginsAdded(QMarginsF(0, 0, 200, 0.1));
  QRectF bounds = scene->itemsBoundingRect().marginsAdded(QMarginsF(50, 50, 200, 50));
  view->setSceneRect(size.united(bounds));
}
