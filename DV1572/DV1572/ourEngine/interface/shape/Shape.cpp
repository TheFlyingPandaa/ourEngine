#include "Shape.h"
#include "../../core/Dx.h"
using namespace DirectX; 

void Shape::_buildMatrix()
{
	XMMATRIX trans = XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z); 
	XMMATRIX rot = XMMatrixRotationRollPitchYaw(m_rot.x, m_rot.y, m_rot.z); 
	XMMATRIX scale = XMMatrixScaling(m_scl.x, m_scl.y, m_scl.z); 

	m_worldMatrix = rot * scale * trans; 
}
void Shape::setVertexShader(ID3D11VertexShader * s)
{
	m_vs = s;
}
void Shape::setHullShader(ID3D11HullShader * s)
{
	m_hs = s;
}
void Shape::setDomainShader(ID3D11DomainShader * s)
{
	m_ds = s;
}
void Shape::setGeometryShader(ID3D11GeometryShader * s)
{
	m_gs = s;
}
void Shape::setPixelShader(ID3D11PixelShader * s)
{
	m_ps = s;
}
Shape::Shape()
{
	m_mesh	= nullptr;
	m_vs	= nullptr;
	m_hs	= nullptr;
	m_ds	= nullptr;
	m_gs	= nullptr;
	m_ps	= nullptr;
	m_worldMatrix = XMMatrixIdentity();
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

void Shape::setPos(float x, float y, float z)
{
	setPos(XMFLOAT3(x, y, z)); 
}

void Shape::setPos(DirectX::XMFLOAT3 pos)
{
	m_pos = pos; 
	_buildMatrix(); 
}

void Shape::Move(float x, float y, float z)
{
	Move(XMFLOAT3(x, y, z)); 
}

void Shape::Move(DirectX::XMFLOAT3 move)
{
	XMVECTOR newPos = XMLoadFloat3(&m_pos) + XMLoadFloat3(&move); 
	XMStoreFloat3(&m_pos, newPos); 
	_buildMatrix(); 
}

const DirectX::XMMATRIX & Shape::getWorld() const
{
	return m_worldMatrix; 
}

void Shape::Draw()
{
	DX::g_renderQueue.push_back(this); 
}
