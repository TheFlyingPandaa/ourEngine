#pragma once
#include "Furniture.h"

class Table : public Furniture
{
private:
	static int lowPrice;
	static int highPrice;
public:
	//The setting is the decider for creepyness and stuff like that
	Table(DirectX::XMFLOAT3 pos, MESH mesh, const int size = 1, const int setting = 0);
	~Table();

	std::string WhatType() override;
	Furniture * MakeCopy() override;

	static int getPrice(bool low);
	//static int getGridSize();
};
