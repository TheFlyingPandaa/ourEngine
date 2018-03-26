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

DirectX::XMFLOAT3 Shape::_convertToRad(DirectX::XMFLOAT3 deg)
{
	return DirectX::XMFLOAT3{
		DirectX::XMConvertToRadians(deg.x),
		DirectX::XMConvertToRadians(deg.y),
		DirectX::XMConvertToRadians(deg.z)
	};
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
	m_pos = { 0, 0, 0 };
	m_rot = { 0,0,0 };
	m_scl = XMFLOAT3{ 1,1,1 };
	_buildMatrix();
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

void Shape::setRotation(float x, float y, float z)
{
	setRotation(XMFLOAT3{ x, y, z });
}

void Shape::setRotation(DirectX::XMFLOAT3 rotation)
{
	m_rot = _convertToRad(rotation);
	_buildMatrix();
}

void Shape::Rotate(float x, float y, float z)
{
	Rotate(DirectX::XMFLOAT3{ x, y, z });
}

void Shape::Rotate(DirectX::XMFLOAT3 rotation)
{
	XMVECTOR newRot = XMLoadFloat3(&m_rot) + XMLoadFloat3(&_convertToRad(rotation));
	XMStoreFloat3(&m_rot, newRot);
	_buildMatrix();
}

void Shape::ApplyShaders()
{
	DX::g_deviceContext->VSSetShader(m_vs, nullptr, 0);
	DX::g_deviceContext->HSSetShader(m_hs, nullptr, 0);
	DX::g_deviceContext->DSSetShader(m_ds, nullptr, 0);
	DX::g_deviceContext->GSSetShader(m_gs, nullptr, 0);
	DX::g_deviceContext->PSSetShader(m_ps, nullptr, 0);

	ID3D11ShaderResourceView* dif = m_mesh->getMaterial()->getDiffuseMap();
	ID3D11ShaderResourceView* nor = m_mesh->getMaterial()->getNormalMap();
	ID3D11ShaderResourceView* hi = m_mesh->getMaterial()->getHighlightMap();
	DX::g_deviceContext->PSSetShaderResources(0, 1, &dif);
	DX::g_deviceContext->PSSetShaderResources(1, 1, &nor);
	DX::g_deviceContext->PSSetShaderResources(2, 1, &hi);

}

void Shape::checkPick()
{
	DX::g_pickingQueue.push_back(this);
}

const DirectX::XMMATRIX & Shape::getWorld() const
{
	return m_worldMatrix; 
}

void Shape::Draw()
{
	DX::g_renderQueue.push_back(this); 
}
