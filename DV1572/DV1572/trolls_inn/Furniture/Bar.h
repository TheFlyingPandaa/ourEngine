#pragma once
#include "Furniture.h"

class Bar : public Furniture
{
private:
	static int lowPrice;
	static int highPrice;
public:
	Bar(DirectX::XMFLOAT3 pos, MESH meshid, const int setting = 0);
	~Bar();

	std::string WhatType() override;
	Furniture * MakeCopy() override;

	static int getPrice(bool low);
	//static int getGridSize();
};
