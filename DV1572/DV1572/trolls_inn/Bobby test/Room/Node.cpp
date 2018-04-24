#include "Node.h"

Node::Node(Tile * tile, Node * parent, float gCost, float hCost)
	:tile(tile), parent(parent), gCost(gCost), hCost(hCost) {
	fCost = gCost + hCost;
}

Node::Node(Tile * tile, std::shared_ptr<Node> parent, float gCost, float hCost)
	: tile(tile), parent(parent), gCost(gCost), hCost(hCost) {
	fCost = gCost + hCost;
}

bool Node::operator<(const Node & other) const
{
	return fCost < other.fCost;
}

bool Node::operator==(Tile & other) const
{
	return tile->getQuad().getPosition().x == other.getQuad().getPosition().x && tile->getQuad().getPosition().z == other.getQuad().getPosition().z;
}

bool Node::operator==(Node & other) const
{
	return tile->getQuad().getPosition().x == other.tile->getQuad().getPosition().x && tile->getQuad().getPosition().z == other.tile->getQuad().getPosition().z;
}

bool Node::operator>(const Node& other) const
{
	return fCost > other.fCost;
	
}