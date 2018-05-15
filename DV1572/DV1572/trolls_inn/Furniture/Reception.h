#pragma once
#include "Furniture.h"

class Reception : public Furniture
{
public:
	Reception(DirectX::XMFLOAT3 pos, MESH meshid, const int setting = 0);
	~Reception();

	std::string WhatType() override;
	Furniture * MakeCopy() override;
	//static int getGridSize();
};