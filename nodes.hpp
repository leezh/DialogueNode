#ifndef NODES_HPP
#define NODES_HPP

#include <QGraphicsItem>
#include <vector>
#include <set>

class DialogueView;
class DialogueScene;

class Node : public QGraphicsItem
{
    friend class MoveCommand;
  public:
    class Connection
    {
      public:
        Connection(Node* source, int id, QString name);
        void connect(Node* newNode);
        void calculatePath();

        QString name;
        Node* node;
        Node* source;
        int sourceSlot;
        QPainterPath path;
    };

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
    DialogueScene* parent();
    DialogueView* mainView();

    QVariant itemChange(GraphicsItemChange change, const QVariant& value) Q_DECL_OVERRIDE;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) Q_DECL_OVERRIDE;

  private:
    DialogueView* view;
    QPointF oldPos;
    bool canMove;
    bool updateRequired;

  protected:
    QRectF size;
    std::vector<Connection> connections;
    std::set<Connection*> receivers;
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
