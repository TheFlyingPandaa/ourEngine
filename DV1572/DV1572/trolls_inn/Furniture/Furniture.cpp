#include "Furniture.h"

Furniture::Furniture()
{
}

Furniture::~Furniture()
{
}

Object3D Furniture::getObject3D()
{
	return m_object;
}

Attributes& Furniture::getAttributes()
{
	return m_attributes;
}

int Furniture::getGridSize()
{
	return m_gridSize;
}
