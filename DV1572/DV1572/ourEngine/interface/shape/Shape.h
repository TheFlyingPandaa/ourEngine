#pragma once
#include <DirectXMath.h>
#include "Mesh.h"

class Shape
{
private:
	DirectX::XMFLOAT3 m_pos, m_scl, m_rot;
	Mesh* m_mesh;
public:
	Shape();
	void setMesh(Mesh* m);
	Mesh* getMesh() const;
	ID3D11Buffer* getVertices() const;
};