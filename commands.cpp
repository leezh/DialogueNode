#include "commands.hpp"

MoveCommand::MoveCommand(const std::vector<Node*>& nodes, QUndoCommand* parent)
  : QUndoCommand(parent)
{
  for (auto node : nodes)
  {
    movements.push_back(Movement(node));
  }
}

void MoveCommand::undo()
{
  for (auto movement : movements)
  {
    movement.node->setPos(movement.oldPos);
  }
}

void MoveCommand::redo()
{
  for (auto movement : movements)
  {
    movement.node->setPos(movement.newPos);
  }
}

MoveCommand::Movement::Movement(Node* node)
  : node(node)
  , oldPos(node->oldPos)
  , newPos(node->pos())
{
}

ConnectCommand::ConnectCommand(Node::Connection* connection, Node* oldNode, QUndoCommand* parent)
  : QUndoCommand(parent)
  , connection(connection)
  , oldNode(oldNode)
  , newNode(connection->node)
{
}

void ConnectCommand::undo()
{
  connection->connect(oldNode);
}

void ConnectCommand::redo()
{
  connection->connect(newNode);
}
