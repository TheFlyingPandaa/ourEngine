#pragma once
#include <DirectXMath.h>
#include "Mesh.h"

class Shape
{
private:
	DirectX::XMFLOAT3		m_pos, m_scl, m_rot;
	Mesh*					m_mesh;
	ID3D11VertexShader*		m_vs;
	ID3D11HullShader*		m_hs;
	ID3D11DomainShader*		m_ds;
	ID3D11GeometryShader*	m_gs;
	ID3D11PixelShader*		m_ps;

protected:
	void setVertexShader(ID3D11VertexShader* s);
	void setHullShader(ID3D11HullShader* s);
	void setDomainShader(ID3D11DomainShader* s);
	void setGeometryShader(ID3D11GeometryShader* s);
	void setPixelShader(ID3D11PixelShader* s);

public:
	Shape();
	void setMesh(Mesh* m);
	Mesh* getMesh() const;
	ID3D11Buffer* getVertices() const;
	void setPos(float x, float y, float z);
	void setPos(DirectX::XMFLOAT3 pos);
	void Move(float x, float y, float z);
	void Move(DirectX::XMFLOAT3 move);

	void Draw();
};