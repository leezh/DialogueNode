#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMainWindow>

class QMenu;
class QMenuBar;
class QDockWidget;
class QUndoStack;
class Node;
class NodeConnection;
class MainWindow;

class DialogueView : public QGraphicsView
{
    Q_OBJECT
    friend class Node;
  public:
    DialogueView(QGraphicsScene* scene, MainWindow* parent);

    Node* dragConnection(Node* node);
    Node* connectFrom();
    void connectTo(Node* node);

  signals:
    void nodeMoved(const std::vector<Node*>& nodes);
    void nodeConnected(NodeConnection* connection, Node* oldNode);

  protected:
    void nodeMoveEvent(const std::vector<Node*>& nodes);
    void nodeConnectEvent(NodeConnection* connection, Node* oldNode);

    void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent* event) Q_DECL_OVERRIDE;

    Node* nodeConnectionFrom;
    Node* nodeConnectionTo;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = 0);
    void updateSceneRect();

  private slots:
    void open();
    void save();
    void saveAs();
    void exportFile();
    void quit();
    void addTextNode();
    void deleteItem();
    void nodeMoved(const std::vector<Node*>& nodes);
    void nodeConnected(NodeConnection* connection, Node* oldNode);

  private:
    void createActions();
    void createMenus();
    void createDocks();

    QUndoStack* undoStack;

    QAction* openAction;
    QAction* saveAction;
    QAction* saveAsAction;
    QAction* exportAction;
    QAction* quitAction;
    QAction* undoAction;
    QAction* redoAction;
    QAction* deleteAction;
    QAction* deleteLooseAction;
    QAction* addTextNodeAction;

    QMenu* fileMenu;
    QMenu* editMenu;
    QToolBar* editToolbar;
    QDockWidget* overviewWidget;
    QDockWidget* propertiesWidget;

    QGraphicsScene* scene;
    DialogueView* view;
};

#endif // MAINWINDOW_HPP
