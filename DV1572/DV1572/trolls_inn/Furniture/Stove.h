#pragma once
#include "Furniture.h"

class Stove : public Furniture
{
public:
	Stove(DirectX::XMFLOAT3 pos, MESH meshid, const int setting = 0);
	~Stove();

	std::string WhatType() override;
	Furniture * MakeCopy() override;
	//static int getGridSize();
};
