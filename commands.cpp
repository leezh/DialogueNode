#include "commands.hpp"
#include "nodes.hpp"

MoveCommand::MoveCommand(std::vector<Movement>&& movements, QUndoCommand* parent)
  : QUndoCommand(parent)
  , movements(movements)
{
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

ConnectCommand::ConnectCommand(NodeConnection* connection, Node* newNode, QUndoCommand* parent)
  : QUndoCommand(parent)
  , connection(connection)
  , oldNode(connection->node())
  , newNode(newNode)
{
}

void ConnectCommand::undo()
{
  connection->setNode(oldNode);
}

void ConnectCommand::redo()
{
  connection->setNode(newNode);
}
