#pragma once
#include "../../ourEngine/interface/shape/3DObject.h"
#include "../AI_Base/Attributes.h"

class Furniture
{
public:
	Furniture();
	~Furniture();

	Object3D	getObject3D();
	Attributes&	getAttributes();
	int			getGridSize(); //gets the size this furniture will take up

protected:
	Object3D	m_object;
	Attributes	m_attributes;
	int			m_gridSize;
};
