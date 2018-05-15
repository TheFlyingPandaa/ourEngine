#pragma once
#include "Furniture.h"

class Bar : public Furniture
{
public:
	Bar(DirectX::XMFLOAT3 pos, MESH meshid, const int setting = 0);
	~Bar();

	std::string WhatType() override;
	Furniture * MakeCopy() override;
	//static int getGridSize();
};
