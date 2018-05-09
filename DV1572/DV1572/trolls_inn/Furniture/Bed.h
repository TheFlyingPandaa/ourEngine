#pragma once
#include "Furniture.h"

class Bed : public Furniture
{
public:
	Bed(DirectX::XMFLOAT3 pos, Mesh * mesh, const int setting = 0);
	~Bed();

	std::string WhatType() override;

	void Draw() override;
	Furniture * MakeCopy() override;
	//static int getGridSize();
};