#pragma once
#include <memory>
#include "Structure.h"

struct Node
{
	Tile* tile;
	std::shared_ptr<Node> parent;
	float fCost, gCost, hCost;

	Node(Tile* tile, Node* parent, float gCost, float hCost);
	
	Node(Tile* tile, std::shared_ptr<Node> parent, float gCost, float hCost);
		
	bool operator>(const Node& other) const;
	
	bool operator<(const Node& other) const;
	
	bool operator==(Tile& other) const;
	
	bool operator==(Node& other) const;
	
};