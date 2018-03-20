#include "Shape.h"
#include "../../core/Dx.h"
Shape::Shape()
{
	//DX::g_deviceContext = nullptr;
}

void Shape::setMesh(Mesh * m)
{
	m_mesh = m;
}

Mesh * Shape::getMesh() const
{
	return m_mesh;
}

ID3D11Buffer * Shape::getVertices() const
{
	return m_mesh->getVertices();
}
