#ifndef NODES_HPP
#define NODES_HPP

#include <QGraphicsItem>
#include <memory>
#include <vector>
#include <set>

class DialogueView;
class QGraphicsScene;
class Node;

class NodeConnection
{
    friend class Node;
  public:
    NodeConnection(Node* source, int id, QString name);
    void setNode(Node* newNode);
    Node* node();
    void calculatePath();

  private:
    QString name;
    Node* dest;
    Node* source;
    int sourceSlot;
    QPainterPath path;
};

class Node : public QGraphicsItem
{
    friend class NodeConnection;
  public:
    Node(DialogueView* view);

    void setConnection(int slot, Node* node);
    Node* connection(int slot);
    void setMoveable(bool moveable);
    bool movable();
    QPointF endPoint();
    QPointF startPoint(int connection);

    QRectF boundingRect() const Q_DECL_OVERRIDE;
    QPainterPath shape() const Q_DECL_OVERRIDE;

  protected:
    int addConnection(QString name = "");
    QGraphicsScene* scene();
    DialogueView* view();

    QVariant itemChange(GraphicsItemChange change, const QVariant& value) Q_DECL_OVERRIDE;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) Q_DECL_OVERRIDE;
    void dragEnterEvent(QGraphicsSceneDragDropEvent* event) Q_DECL_OVERRIDE;
    void dropEvent(QGraphicsSceneDragDropEvent* event) Q_DECL_OVERRIDE;

  private:
    DialogueView* parent;
    QPointF oldPos;
    QRectF oldBounds;
    bool canMove;

  protected:
    QRectF size;
    std::vector<std::unique_ptr<NodeConnection>> connections;
    std::set<NodeConnection*> receivers;
    static constexpr float HandleWidth = 15.f;
    static constexpr float ConnectionHeight = 20.f;
};

class TextNode : public Node
{
  public:
    TextNode(DialogueView* view);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) Q_DECL_OVERRIDE;

    void setText(const QString& text);
    const QString& text() const;

  private:
    QString txt;
};

#endif // NODES_HPP
