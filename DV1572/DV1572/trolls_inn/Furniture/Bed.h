#pragma once
#include "Furniture.h"

class Bed : public Furniture
{
public:
	Bed(DirectX::XMFLOAT3 pos, MESH meshid, const int setting = 0);
	~Bed();

	std::string WhatType() override;
	Furniture * MakeCopy() override;
	//static int getGridSize();
};