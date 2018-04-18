#pragma once
#include "../../ourEngine/interface/shape/3DObject.h"
#include "../AI_Base/Attributes.h"

class Furniture
{
public:
	Furniture(DirectX::XMFLOAT3 pos, Mesh * mesh);
	~Furniture();

	Object3D	getObject3D();
	Attributes&	getAttributes();
	int	getGridSize(); //gets the size this furniture will take up

	DirectX::XMINT3 getPosition();
	void Draw();

private:
	DirectX::XMINT3 pos;
protected:
	Object3D	p_object;
	Attributes	p_attributes;
	int			p_gridSize;
	int			p_rot;
};
