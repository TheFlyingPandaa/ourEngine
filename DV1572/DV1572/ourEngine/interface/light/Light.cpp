#include "Light.h"
#include "../../core/Dx.h"

void Light::updateMatrix()
{
	DirectX::XMMATRIX view = DirectX::XMMatrixLookToLH(
		XMLoadFloat4A(&m_pos),
		XMLoadFloat4A(&m_dir),
		up
	);
	
	m_lightBuffer.pos = m_pos;
	m_lightBuffer.dir = m_dir;

	m_viewProj = view * m_proj;

	DirectX::XMMatrixTranspose(m_viewProj);

	DirectX::XMStoreFloat4x4A(&m_lightBuffer.viewProjection, m_viewProj);
}

DirectX::XMMATRIX Light::getViewProjMatrix() const
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

	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.Height = static_cast<UINT>(m_height);
	textureDesc.Width = static_cast<UINT>(m_width);


	hr = 0;

	hr = DX::g_device->CreateTexture2D(&textureDesc, nullptr, &m_shadowTexture);

	D3D11_DEPTH_STENCIL_VIEW_DESC Dsvd;
	Dsvd.Flags = 0;
	Dsvd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	Dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	Dsvd.Texture2D.MipSlice = 0;

	hr = DX::g_device->CreateDepthStencilView(m_shadowTexture, &Dsvd, &m_shadowDepthStencilView);

	D3D11_SHADER_RESOURCE_VIEW_DESC Srvd;
	Srvd.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	Srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	Srvd.Texture2D.MipLevels = 1;
	Srvd.Texture2D.MostDetailedMip = 0;

	hr = DX::g_device->CreateShaderResourceView(m_shadowTexture, &Srvd, &m_shadowShaderResourceView);
}

Light::Light()
{
	//m_proj = DirectX::XMMatrixOrthographicLH(1280.0f, 720.0f, 0.1f, 200.0f);
	//m_proj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45), static_cast<float>(1280) / 720, 0.1f, 5.0f);
}

void Light::Init(DirectX::XMFLOAT4A pos, DirectX::XMFLOAT4A dir, DirectX::XMFLOAT4A color, float width, float height)
{
	//Setting the initial values for the light via its buffer. 
	m_lightBuffer.pos = m_pos = pos;
	m_lightBuffer.dir = m_dir = dir;
	m_lightBuffer.color = m_color = color;

	//Setting width and height of 
	m_width = width;
	m_height = height;

	m_proj = DirectX::XMMatrixOrthographicLH((float)m_width * 0.05f, (float)m_height * 0.05f, 0.1f, 2000.0f);

	_createResources();
	this->updateMatrix();
}

Light::~Light()
{
	m_pLightBuffer->Release();
}

DirectX::XMFLOAT4A Light::getPos() const
{
	return m_pos; 
}

DirectX::XMFLOAT4A Light::getDir() const
{
	return m_dir; 
}

DirectX::XMFLOAT4A Light::getColor() const
{
	return m_color; 
}

void Light::setPos(DirectX::XMFLOAT4A pos)
{
	m_lightBuffer.pos = pos; 
	m_pos = pos; 
	this->updateMatrix();
}

void Light::setDir(DirectX::XMFLOAT4A dir)
{
	m_lightBuffer.dir = dir;
	m_dir = dir; 
	this->updateMatrix();
}

void Light::setColor(DirectX::XMFLOAT4A color)
{
	m_lightBuffer.color = color;
	m_color = color; 
}

void Light::Move(DirectX::XMFLOAT4A move)
{
	DirectX::XMVECTOR newPos = DirectX::XMVectorAdd(DirectX::XMLoadFloat4A(&m_pos),DirectX::XMLoadFloat4A(&move));
	XMStoreFloat4A(&m_pos, newPos); 
	this->updateMatrix();
	//Rebuild matrixes here
}

ID3D11DepthStencilView *& Light::getDepthView()
{
	return m_shadowDepthStencilView;
}

ID3D11ShaderResourceView *& Light::getResourceView()
{
	return m_shadowShaderResourceView;
}



DIRECTIONAL_LIGHT_BUFFER& Light::getBuffer()
{
	return m_lightBuffer; 
}

ID3D11Buffer* Light::getBufferPointer()
{
	return m_pLightBuffer;
}