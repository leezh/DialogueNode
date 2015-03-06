#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QDockWidget>
#include <QUndoStack>
#include <QMenuBar>
#include <QMenu>

class Node;
class MainWindow;

class DialogueScene : public QGraphicsScene
{
    Q_OBJECT

  signals:
    void nodeMoved(const std::vector<Node*>& nodes);

  public slots:
    virtual void nodeMoveEvent(const std::vector<Node*>& nodes);

  public:
    DialogueScene(QObject* parent = 0);
};

class DialogueView : public QGraphicsView
{
    Q_OBJECT
  public:
    DialogueView(QGraphicsScene* scene, MainWindow* parent);

  protected:
    void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent* event) Q_DECL_OVERRIDE;
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
    QGraphicsView* overview;
    QDockWidget* overviewWidget;
    QDockWidget* propertiesWidget;

    DialogueScene* scene;
    DialogueView* view;
};

#endif // MAINWINDOW_HPP
