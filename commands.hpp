#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <QUndoCommand>
#include <QPointF>

class Node;
class NodeConnection;

class MoveCommand : public QUndoCommand
{
  public:
    struct Movement
    {
      public:
        Node* node;
        QPointF oldPos;
        QPointF newPos;
    };

    MoveCommand(std::vector<Movement>&& movements, QUndoCommand* parent = 0);
    void undo();
    void redo();

  private:
    std::vector<Movement> movements;
};

class ConnectCommand : public QUndoCommand
{
  public:
    ConnectCommand(NodeConnection* connection, Node* newNode, QUndoCommand* parent = 0);
    void undo();
    void redo();

  private:
    NodeConnection* connection;
    Node* oldNode;
    Node* newNode;
};

#endif // COMMANDS_HPP
