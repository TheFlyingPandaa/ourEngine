#pragma once
#include "Furniture.h"

class Chair : public Furniture
{
public:
	Chair(DirectX::XMFLOAT3 pos, MESH meshid, const int setting = 0);
	~Chair();

	std::string WhatType() override;
	Furniture * MakeCopy() override;
	//static int getGridSize();
};

