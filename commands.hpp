#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <QUndoCommand>
#include <QPointF>
#include <memory>
#include <vector>
#include <set>
#include <map>

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

class DeleteCommand : public QUndoCommand
{
  public:
    struct OldNode
    {
      public:
        OldNode(Node* node);
        Node* node;
        std::set<NodeConnection*> receivers;
        std::map<NodeConnection*, Node*> connections;
    };

    DeleteCommand(const std::vector<Node*>& nodes, QUndoCommand* parent = 0);
    ~DeleteCommand();
    void undo();
    void redo();

  private:
    std::vector<std::unique_ptr<OldNode>> oldNodes;
    bool ownership;
};

#endif // COMMANDS_HPP
