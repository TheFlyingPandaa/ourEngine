#include "Light.h"
#include "../../core/Dx.h"

void Light::updateMatrix()
{
	m_pos = XMFLOAT4A(0, 0, 10, 1);
	DirectX::XMVECTOR vDir; 
	vDir = DirectX::XMLoadFloat4A(&m_lightBuffer.dir); 
	DirectX::XMStoreFloat4A(&m_dir, vDir); 
	
	XMMATRIX view = XMMatrixLookAtLH(
		XMLoadFloat4(&m_pos),
		XMLoadFloat4(&m_dir),
		up
	);
	//view = cam.getViewMatrix();
	//TODO: FIX THIS SHIT
	//XMMATRIX projection = XMMatrixOrthographicLH(m_width, m_height, 0.1f, 200.0f);
	XMMATRIX projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45), static_cast<float>(1280) / 720, 0.1f, 5.0f);
	//Update light wvp space
	XMStoreFloat4x4A(&m_lightBuffer.viewProjection, XMMatrixTranspose(view * projectionMatrix)); 

}

void Light::cpyDataDir(DIRECTIONAL_LIGHT_BUFFER& bufferToWriteFrom, ID3D11Buffer* bufferPointer)
{
	D3D11_MAPPED_SUBRESOURCE sunLightData;
	DX::g_deviceContext->Map(bufferPointer, 0, D3D11_MAP_WRITE_DISCARD, 0, &sunLightData);
	memcpy(sunLightData.pData, &(bufferToWriteFrom), sizeof(DIRECTIONAL_LIGHT_BUFFER));
	DX::g_deviceContext->Unmap(bufferPointer, 0);
	DX::g_deviceContext->PSSetConstantBuffers(2, 1, &bufferPointer);
}

void Light::CreatesShadows()
{
	DX::g_lightPos = m_pos;
	DX::g_lightDir = m_dir;
}

void Light::_createResources()
{
	HRESULT hr; 

	//Shadow Map Texture Desc
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

	//Creating Depth Stencil View Desc
	D3D11_DEPTH_STENCIL_VIEW_DESC Dsvd; 
	Dsvd.Flags = 0;
	Dsvd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	Dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	Dsvd.Texture2D.MipSlice = 0;

	//Creating Shader Resource 
	D3D11_SHADER_RESOURCE_VIEW_DESC Srvd;
	Srvd.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	Srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	Srvd.Texture2D.MipLevels = 1;
	Srvd.Texture2D.MostDetailedMip = 0; 

	//Create light buffer
	D3D11_BUFFER_DESC lBdesc;
	lBdesc.Usage = D3D11_USAGE_DYNAMIC;
	lBdesc.ByteWidth = sizeof(DIRECTIONAL_LIGHT_BUFFER);
	lBdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lBdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lBdesc.MiscFlags = 0;
	lBdesc.StructureByteStride = 0;

	//Create texture and depth/resource views and buffer. 
	hr = DX::g_device->CreateTexture2D(&textureDesc, NULL, &m_pShadowTexture); 
	hr = DX::g_device->CreateDepthStencilView(m_pShadowTexture, &Dsvd, &m_pDepthStencilView);
	hr = DX::g_device->CreateShaderResourceView(m_pShadowTexture, &Srvd, &m_pShaderResourceView); 
	hr = DX::g_device->CreateBuffer(&lBdesc, nullptr, &m_pLightBuffer);
}

void Light::InitDirectional(XMFLOAT4A pos, XMFLOAT4A dir, XMFLOAT4A color, float width, float height)
{
	//Setting the initial values for the light via its buffer. 
	m_lightBuffer.pos = pos;
	m_lightBuffer.dir = dir;
	m_lightBuffer.color = color;
	m_pos = pos;
	m_dir = dir;

	//Setting width and height of 
	m_width = width;
	m_height = height;

	_createResources();
}

Light::Light()
{
	
}

Light::~Light()
{
	m_pShadowTexture->Release();
	m_pDepthStencilView->Release();
	m_pShaderResourceView->Release();
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
}

void Light::setDir(XMFLOAT4A dir)
{
	m_lightBuffer.dir = dir;
	m_dir = dir; 
}

void Light::setColor(XMFLOAT4A color)
{
	m_lightBuffer.color = color;
	m_color = color; 
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