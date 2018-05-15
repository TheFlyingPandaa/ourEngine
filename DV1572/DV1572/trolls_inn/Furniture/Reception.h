#pragma once
#include "Furniture.h"

class Reception : public Furniture
{
private :
	static int lowPrice;
	static int highPrice;
public:
	Reception(DirectX::XMFLOAT3 pos, MESH meshid, const int setting = 0);
	~Reception();

	std::string WhatType() override;
	Furniture * MakeCopy() override;

	static int getPrice(bool low);
	//static int getGridSize();
};