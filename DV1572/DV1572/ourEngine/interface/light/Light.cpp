#include "Light.h"
#include "../../core/Dx.h"

void Light::updateMatrix()
{
	
	XMMATRIX view = XMMatrixLookToLH(
		XMLoadFloat4A(&m_pos),
		XMLoadFloat4A(&m_dir),
		up
	);
	
	m_viewProj = view * m_proj;
}

XMMATRIX Light::getViewProjMatrix() const
{
	return m_viewProj;
}

void Light::_createResources()
{
	HRESULT hr; 

	//Create light buffer
	D3D11_BUFFER_DESC lBdesc;
	lBdesc.Usage = D3D11_USAGE_DYNAMIC;
	lBdesc.ByteWidth = sizeof(DIRECTIONAL_LIGHT_BUFFER);
	lBdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lBdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lBdesc.MiscFlags = 0;
	lBdesc.StructureByteStride = 0;

	//Create texture and depth/resource views and buffer. 
	hr = DX::g_device->CreateBuffer(&lBdesc, nullptr, &m_pLightBuffer);
}

Light::Light()
{
	//m_proj = DirectX::XMMatrixOrthographicLH(1280.0f, 720.0f, 0.1f, 200.0f);
	//m_proj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45), static_cast<float>(1280) / 720, 0.1f, 5.0f);
}

void Light::Init(XMFLOAT4A pos, XMFLOAT4A dir, XMFLOAT4A color, float width, float height, Camera * cam)
{
	//Setting the initial values for the light via its buffer. 
	m_lightBuffer.pos = m_pos = pos;
	m_lightBuffer.dir = m_dir = dir;
	m_lightBuffer.color = m_color = color;

	//Setting width and height of 
	m_width = width;
	m_height = height;

	XMMATRIX view = cam->getViewMatrix();
	m_viewProj = view * m_proj;

	_createResources();
	//this->updateMatrix();
}

Light::~Light()
{
	m_pLightBuffer->Release();
}

XMFLOAT4A Light::getPos() const
{
	return m_pos; 
}

XMFLOAT4A Light::getDir() const
{
	return m_dir; 
}

XMFLOAT4A Light::getColor() const
{
	return m_color; 
}

void Light::setPos(XMFLOAT4A pos)
{
	m_lightBuffer.pos = pos; 
	m_pos = pos; 
	this->updateMatrix();
}

void Light::setDir(XMFLOAT4A dir)
{
	m_lightBuffer.dir = dir;
	m_dir = dir; 
	this->updateMatrix();
}

void Light::setColor(XMFLOAT4A color)
{
	m_lightBuffer.color = color;
	m_color = color; 
}

void Light::Move(XMFLOAT4A move)
{
	XMVECTOR newPos = XMLoadFloat4A(&m_pos) + XMLoadFloat4A(&move);
	XMStoreFloat4A(&m_pos, newPos); 
	this->updateMatrix();
	//Rebuild matrixes here
}

ID3D11DepthStencilView *& Light::getDepthView() 
{
	return m_pDepthStencilView; 
}

DIRECTIONAL_LIGHT_BUFFER& Light::getBuffer()
{
	return m_lightBuffer; 
}

ID3D11Buffer* Light::getBufferPointer()
{
	return m_pLightBuffer;
}