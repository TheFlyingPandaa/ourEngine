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
	m_gridscale = 1;
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

	m_highLightColor = XMFLOAT4A{ 1.0f,1.0f,1.0f ,1.0f };
	_buildMatrix();


}

Shape::~Shape()
{

}

void Shape::setMesh(Mesh * m)
{
	m_mesh = m;
}

Mesh * Shape::getMesh() const
{
	return m_mesh;
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

DirectX::XMFLOAT3 Shape::getPosition() const
{
	return m_pos;
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

DirectX::XMFLOAT3 Shape::getRotation() const
{
	return m_rot;
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

void Shape::setScale(float scl)
{
	setScale(XMFLOAT3(scl, scl, scl));
}

void Shape::setScale(float x, float y, float z)
{
	setScale(XMFLOAT3(x, y, z));
}

void Shape::setScale(DirectX::XMFLOAT3 scl)
{
	m_scl = scl;
	_buildMatrix();
}

DirectX::XMFLOAT3 Shape::getScale() const
{
	return m_scl;
}

void Shape::Scale(float scl)
{
	Scale(XMFLOAT3(scl, scl, scl));
}

void Shape::Scale(float x, float y, float z)
{
	Scale(XMFLOAT3(x, y, z));
}

void Shape::Scale(DirectX::XMFLOAT3 scl)
{
	XMVECTOR newScl = XMLoadFloat3(&m_scl) + XMLoadFloat3(&scl);
	XMStoreFloat3(&m_scl, newScl);
	_buildMatrix();
}

std::string Shape::toString() const
{
	std::string outputString = "";

	outputString += std::to_string(m_pos.x) + ":" + std::to_string(m_pos.y);

	return outputString;
}

void Shape::ApplyShaders()
{
	DX::g_deviceContext->VSSetShader(m_vs, nullptr, 0);
	DX::g_deviceContext->HSSetShader(m_hs, nullptr, 0);
	DX::g_deviceContext->DSSetShader(m_ds, nullptr, 0);
	DX::g_deviceContext->GSSetShader(m_gs, nullptr, 0);
	DX::g_deviceContext->PSSetShader(m_ps, nullptr, 0);

}

void Shape::ApplyMaterials(int i)
{
	ID3D11ShaderResourceView* dif = m_mesh->getMaterial(i)->getDiffuseMap();
	ID3D11ShaderResourceView* nor = m_mesh->getMaterial(i)->getNormalMap();
	ID3D11ShaderResourceView* hi = m_mesh->getMaterial(i)->getHighlightMap();

	DX::g_deviceContext->PSSetShaderResources(0, 1, &dif);
	DX::g_deviceContext->PSSetShaderResources(1, 1, &nor);
	DX::g_deviceContext->PSSetShaderResources(2, 1, &hi);
}

void Shape::CheckPick()
{
	DX::g_pickingQueue.push_back(this);
	DX::submitToInstance(this, DX::g_instanceGroupsPicking);
}

void Shape::TESTSHADOW()
{
	//DX::g_shadowQueue.push_back(this);
	DX::submitToInstance(this, DX::g_InstanceGroupsShadow);
}

const DirectX::XMMATRIX & Shape::getWorld() const
{
	return m_worldMatrix; 
}

DirectX::XMFLOAT4A Shape::getColor()
{
	return m_highLightColor;
}

void Shape::setColor(float r, float g, float b, float a)
{
	m_highLightColor = DirectX::XMFLOAT4A{ r,g,b,a };
}

void Shape::setColor(DirectX::XMFLOAT4A color)
{
	m_highLightColor = color;	//Fucking magnus wanted more functionality... erg erg passive argressive jocke :]
}

void Shape::Draw()
{
	if (m_mesh->getMaterial()->isTransparent())
	{
		DX::submitToInstance(this, DX::g_instanceGroupsTransparancy);
	}
	else 
	{
		DX::submitToInstance(this, DX::g_instanceGroups);
	}
}

void Shape::setGridScale(int scale)
{
	m_gridscale = scale;
}

int Shape::getGridScale() const
{
	return m_gridscale;
}

void Shape::TEMPTRANS()
{
	DX::submitToInstance(this, DX::g_instanceGroupsTransparancy);
}
