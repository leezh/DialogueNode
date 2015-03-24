#include <QGraphicsScene>
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

DeleteCommand::OldNode::OldNode(Node* node)
  : node(node)
  , receivers(node->receivers)
{
  for (auto& connection : node->connections)
  {
    connections.insert(std::make_pair(connection.get(), connection->node()));
  }
}

DeleteCommand::DeleteCommand(const std::vector<Node*>& nodes, QUndoCommand* parent)
  : QUndoCommand(parent)
  , ownership(false)
{
  for(auto node : nodes)
  {
    oldNodes.push_back(std::unique_ptr<OldNode>(new OldNode(node)));
  }
}

DeleteCommand::~DeleteCommand()
{
  if (ownership)
  {
    for (auto& oldNode : oldNodes)
    {
      delete oldNode->node;
    }
  }
}

void DeleteCommand::undo()
{
  ownership = false;
  for (auto& oldNode : oldNodes)
  {
    oldNode->node->scene()->addItem(oldNode->node);
    for (auto& connection : oldNode->connections)
    {
      connection.first->setNode(connection.second);
    }
    for (auto& receivers : oldNode->receivers)
    {
      receivers->setNode(oldNode->node);
    }
  }
}

void DeleteCommand::redo()
{
  for (auto& oldNode : oldNodes)
  {
    for (auto& connection : oldNode->node->connections)
    {
      connection->setNode(0);
    }
    for (auto& receivers : oldNode->receivers)
    {
      receivers->setNode(0);
    }
    oldNode->node->scene()->removeItem(oldNode->node);
  }
  ownership = true;
}
