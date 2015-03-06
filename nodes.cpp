#include "nodes.hpp"
#include "mainwindow.hpp"
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneEvent>
#include <QPainter>
#include <utility>

Node::Connection::Connection(Node* source, int sourceSlot, QString name)
  : name(name)
  , node(0)
  , source(source)
  , sourceSlot(sourceSlot)
{
}

void Node::Connection::connect(Node* newNode)
{
  if (node)
  {
    node->receivers.erase(this);
  }
  node = newNode;
  calculatePath();
  if (node)
  {
    node->receivers.insert(this);
  }
}

void Node::Connection::calculatePath()
{
  path = QPainterPath();
  if (node)
  {
    QPointF start = source->startPoint(sourceSlot);
    QPointF end = node->pos() + node->endPoint() - source->pos();
    QPointF c = QPointF(50.f, 0.f);
    QPointF m = QPointF(5.f, 0.f);
    path.moveTo(start);
    path.lineTo(start + m);
    path.cubicTo(start + c + m, end - c - m, end - m);
    path.lineTo(end);
  }
}

Node::Node(DialogueView* view)
  : view(view)
  , canMove(false)
  , size(0.f, 0.f, 120.f, 50.f)
{
  setFlag(ItemIsSelectable);
  setFlag(ItemSendsScenePositionChanges);
  setAcceptHoverEvents(true);
  setAcceptDrops(true);
}

void Node::setConnection(int slot, Node* node)
{
  connections[slot].connect(node);
}

Node* Node::connection(int slot)
{
  return connections[slot].node;
}

void Node::setMoveable(bool moveable)
{
  canMove = moveable;
}

bool Node::movable()
{
  return canMove;
}

QPointF Node::endPoint()
{
  return QPointF(0.f, .5f * size.height());
}

QPointF Node::startPoint(int connection)
{
  return QPointF(size.width() + HandleWidth, size.height() + .5f * ConnectionHeight * (connection * 2 + 1));
}

QRectF Node::boundingRect() const
{
  QRectF s;
  s.setHeight(size.height() + connections.size() * ConnectionHeight);
  s.setWidth(size.width() + HandleWidth);
  for (auto& connection : connections)
  {
    QRectF box = connection.path.boundingRect();
    s = s.united(box.marginsAdded(QMargins(5, 5, 5, 5)));
  }
  for (auto reciever : receivers)
  {
    QRectF box = reciever->path.boundingRect();
    box.translate(reciever->source->pos() - pos());
    s = s.united(box.marginsAdded(QMargins(5, 5, 5, 5)));
  }
  return s;
}

QPainterPath Node::shape() const
{
  QPainterPath path;
  QRectF s;
  s.setHeight(size.height() + connections.size() * ConnectionHeight);
  s.setWidth(size.width() + HandleWidth);
  path.addRect(s);
  return path;
}

int Node::addConnection(QString name)
{
  int slot = connections.size();
  connections.push_back(Connection(this, slot, name));
  return slot;
}

DialogueScene* Node::parent()
{
  return qobject_cast<DialogueScene*>(view->scene());
}

DialogueView* Node::mainView()
{
  return view;
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant& value)
{
  if (change == QGraphicsItem::ItemScenePositionHasChanged) {
    updateRequired = true;
    for (auto& connection : connections)
    {
      connection.calculatePath();
    }
    for (auto receiver : receivers)
    {
      receiver->calculatePath();
    }
  }
  return value;
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
  if (event->button() == Qt::LeftButton)
  {
    if (event->pos().x() < HandleWidth || event->pos().y() < size.height())
    {
      if (!parent()->selectedItems().contains(this))
      {
        if (canMove)
        {
          setFlag(ItemIsMovable);
          oldPos = pos();
        }
      }
      else
      {
        for (auto item : parent()->selectedItems())
        {
          auto item_cast = dynamic_cast<Node*>(item);
          if (item_cast && item_cast->canMove)
          {
            item_cast->setFlag(ItemIsMovable);
            item_cast->oldPos = item_cast->pos();
          }
        }
      }
      QGraphicsItem::mousePressEvent(event);
    }
  }
  else
  {
    QGraphicsItem::mousePressEvent(event);
  }
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
  QGraphicsItem::mouseReleaseEvent(event);
  if (flags() & ItemIsMovable)
  {
    std::vector<Node*> nodes;
    for (auto item : parent()->selectedItems())
    {
      auto item_cast = dynamic_cast<Node*>(item);
      if (item_cast && item_cast->canMove)
      {
        item_cast->setFlag(ItemIsMovable, false);
        if (item_cast->pos() != item_cast->oldPos)
        {
          nodes.push_back(item_cast);
        }
      }
    }
    if (!nodes.empty())
    {
      parent()->nodeMoveEvent(nodes);
    }
  }
}

void Node::paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget)
{
  Q_UNUSED(item);
  Q_UNUSED(widget);

  painter->setBrush(Qt::NoBrush);
  painter->setRenderHint(QPainter::Antialiasing, true);
  for (auto& connection : connections)
  {
    if (updateRequired)
    {
      connection.calculatePath();
    }
    painter->drawPath(connection.path);
  }
  updateRequired = false;
  painter->setRenderHint(QPainter::Antialiasing, false);

  const QColor handleColor = QColor(100, 100, 100);
  QRectF handleBox;
  handleBox.setHeight(size.height() + connections.size() * ConnectionHeight);
  handleBox.setWidth(HandleWidth + 1.f);
  painter->setBrushOrigin(1, 2);
  painter->setPen(Qt::NoPen);
  if (item->state & QStyle::State_Selected)
  {
    painter->setBrush(handleColor.light());
  }
  else
  {
    painter->setBrush(handleColor.dark());
  }
  painter->drawRect(handleBox);
  painter->setPen(handleColor);
  painter->setBrush(QBrush(handleColor, Qt::Dense3Pattern));
  painter->drawRect(handleBox);

  QColor color = QColor(200, 200, 200);
  if (item->state & QStyle::State_Selected)
  {
    color = color.light(110);
  }

  QColor boxColor = color;
  QRectF box = size;
  box.moveLeft(HandleWidth);
  painter->setPen(handleColor);
  painter->setBrush(boxColor);
  painter->drawRect(box);

  QColor connectionColor = color;
  box.moveTop(box.height() - 1.f);
  box.setHeight(ConnectionHeight + 1);
  for (auto& connection : connections)
  {
    painter->setBrush(connectionColor);
    painter->drawRect(box);
    painter->setPen(handleColor.dark(150));
    painter->drawText(box.marginsRemoved(QMargins(5, 0, 5, 0)), Qt::AlignRight, connection.name);
    painter->setPen(handleColor);
    box.moveTop(ConnectionHeight);
  }
}

TextNode::TextNode(DialogueView* view)
  : Node(view)
{
  setMoveable(true);
  addConnection("Next");
}

void TextNode::paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget)
{
  Node::paint(painter, item, widget);
}

void TextNode::setText(const QString& text)
{
  txt = text;
}

const QString& TextNode::text() const
{
  return txt;
}
