#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <QUndoCommand>
#include "mainwindow.hpp"
#include "nodes.hpp"

class MoveCommand : public QUndoCommand
{
  public:
    MoveCommand(const std::vector<Node*>& nodes, QUndoCommand* parent = 0);

    void undo();
    void redo();

  private:
    class Movement
    {
      public:
        Movement(Node* node);

        Node* node;
        QPointF oldPos;
        QPointF newPos;
    };

    std::vector<Movement> movements;
};

class ConnectCommand : public QUndoCommand
{
  public:
    ConnectCommand(NodeConnection* connection, Node* oldNode, QUndoCommand* parent = 0);

    void undo();
    void redo();

  private:
    NodeConnection* connection;
    Node* oldNode;
    Node* newNode;
};

#endif // COMMANDS_HPP
