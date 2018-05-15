#pragma once
#include "Furniture.h"

class Bed : public Furniture
{

	static int lowPrice;
	static int highPrice;
public:
	Bed(DirectX::XMFLOAT3 pos, MESH meshid, const int setting = 0);
	~Bed();

	std::string WhatType() override;
	Furniture * MakeCopy() override;

	static int getPrice(bool low);
	//static int getGridSize();
};