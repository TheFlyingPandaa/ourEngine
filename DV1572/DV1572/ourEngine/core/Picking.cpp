#include "Picking.h"
#include <d3d11.h>
#include "../Core/Dx.h"
#include "../interface/Input.h"
#pragma comment (lib, "d3d11.lib")

Picking::Picking()
{
}

Picking::~Picking()
{
}

void Picking::InitPickingTexture(const int width, const int height, const int sample, ID3D11Texture2D*& TextureMap, ID3D11RenderTargetView*&RTV, ID3D11ShaderResourceView*&SRV, ID3D11Texture2D*&  m_pickingReadBuffer)
{
	D3D11_TEXTURE2D_DESC tDesc{};
	tDesc.Width = width;
	tDesc.Height = height;
	tDesc.MipLevels = 1;
	tDesc.ArraySize = 1;
	tDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	tDesc.SampleDesc.Count = sample;
	tDesc.Usage = D3D11_USAGE_DEFAULT;
	tDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	//tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	D3D11_RENDER_TARGET_VIEW_DESC rDesc{};
	rDesc.Format = tDesc.Format;
	rDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	D3D11_SHADER_RESOURCE_VIEW_DESC sDesc{};
	sDesc.Format = tDesc.Format;
	sDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	sDesc.Texture2D.MipLevels = 1;

	DX::g_device->CreateTexture2D(&tDesc, nullptr, &TextureMap);
	DX::g_device->CreateRenderTargetView(TextureMap, &rDesc, &RTV);
	DX::g_device->CreateShaderResourceView(TextureMap, &sDesc, &SRV);

	D3D11_TEXTURE2D_DESC StagedDesc = {
		1,//UINT Width;
		1,//UINT Height;
		1,//UINT MipLevels;
		1,//UINT ArraySize;
		DXGI_FORMAT_R32G32B32A32_FLOAT,//DXGI_FORMAT Format;
		1, 0,//DXGI_SAMPLE_DESC SampleDesc;
		D3D11_USAGE_STAGING,//D3D11_USAGE Usage;
		0,//UINT BindFlags;
		D3D11_CPU_ACCESS_READ,//UINT CPUAccessFlags;
		0//UINT MiscFlags;
	};
	HRESULT hr = DX::g_device->CreateTexture2D(&StagedDesc, 0, &m_pickingReadBuffer);
	hr = 0;
}

Shape * Picking::getPicked(Camera * c, ID3D11RenderTargetView*&RTV, ID3D11DepthStencilView*& m_depthStencilView, const DirectX::XMMATRIX m_projectionMatrix, DirectX::XMMATRIX m_HUDview, ID3D11Buffer*& m_pickingBuffer, ID3D11VertexShader*& m_pickingVertexShader, ID3D11PixelShader*& m_pickingPixelShader, ID3D11Texture2D*& TextureMap, ID3D11Texture2D*& m_pickingReadBuffer)
{
	DX::g_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DX::g_deviceContext->IASetInputLayout(DX::g_inputLayout);

	float color[4]{ 0.0f, 0.0f, 0.0f, 1.0f };

	DX::g_deviceContext->ClearRenderTargetView(RTV, color);
	DX::g_deviceContext->OMSetRenderTargets(1, &RTV, m_depthStencilView);

	PICK_BUFFER pb;
	DirectX::XMMATRIX vp = c->getViewMatrix() * m_projectionMatrix;
	DirectX::XMFLOAT4A counter = { 0.0f, 0.0f, 0.0f, 0.0f };
	pb.index = DirectX::XMFLOAT4A(0.0f, 0.0f, 0.0f, 0.0f);
	for (auto s : DX::g_pickingQueue)
	{
		if (++counter.x > 255)
		{
			counter.x = 0;
			counter.y++;
		}
		if (counter.y > 255)
		{
			counter.y = 0;
			counter.z++;
		}
		if (counter.z > 255)
		{
			exit(0);
		}
		//counter.x = 120;
		DirectX::XMMATRIX mvp;

		RectangleShape* isRectangle = dynamic_cast<RectangleShape*>(s);
		if (isRectangle && isRectangle->isHud())
			mvp = DirectX::XMMatrixTranspose(s->getWorld() * m_HUDview);
		else
			mvp = DirectX::XMMatrixTranspose(s->getWorld() * vp);

		DirectX::XMStoreFloat4x4A(&pb.MVP, mvp);
		pb.index = counter;
		D3D11_MAPPED_SUBRESOURCE dataPtr;
		DX::g_deviceContext->Map(m_pickingBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);
		memcpy(dataPtr.pData, &pb, sizeof(PICK_BUFFER));
		DX::g_deviceContext->Unmap(m_pickingBuffer, 0);
		DX::g_deviceContext->VSSetConstantBuffers(0, 1, &m_pickingBuffer);

		DX::g_deviceContext->VSSetShader(m_pickingVertexShader, nullptr, 0);
		DX::g_deviceContext->HSSetShader(nullptr, nullptr, 0);
		DX::g_deviceContext->DSSetShader(nullptr, nullptr, 0);
		DX::g_deviceContext->GSSetShader(nullptr, nullptr, 0);
		DX::g_deviceContext->PSSetShader(m_pickingPixelShader, nullptr, 0);

		UINT32 vertexSize = sizeof(VERTEX);
		UINT offset = 0;

		ID3D11Buffer* v = s->getMesh()->getVertices();
		DX::g_deviceContext->IASetVertexBuffers(0, 1, &v, &vertexSize, &offset);
		DX::g_deviceContext->Draw(s->getMesh()->getNumberOfVertices(), 0);
	}
	DX::g_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//gDeviceContext->CopyResource(computeReadWriteBuffer, computeOutputBuffer);


	D3D11_BOX srcBox;
	srcBox.left = Input::getMousePosition().x;
	srcBox.right = srcBox.left + 1;
	srcBox.top = Input::getMousePosition().y;
	srcBox.bottom = srcBox.top + 1;
	srcBox.front = 0;
	srcBox.back = 1;


	DX::g_deviceContext->CopySubresourceRegion(m_pickingReadBuffer, 0, 0, 0, 0, TextureMap, 0, &srcBox);
	//DX::g_deviceContext->CopyResource(cpuAccess, m_pickingTexture.TextureMap);
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	//HRESULT hr = gDeviceContext->Map(computeReadWriteBuffer, 0, D3D11_MAP_READ, 0, &mappedResource);
	HRESULT hr = DX::g_deviceContext->Map(m_pickingReadBuffer, 0, D3D11_MAP_READ, 0, &mappedResource);

	//computeShader* dataView = reinterpret_cast<computeShader*>(mappedResource.pData);
	//DirectX::XMFLOAT4A ** dataView = reinterpret_cast<DirectX::XMFLOAT4A**>(mappedResource.pData);

	DirectX::XMFLOAT4A pixel = *((DirectX::XMFLOAT4A*)mappedResource.pData);

	int index = static_cast<int>(pixel.x + (pixel.y * 255) + (pixel.z * 255 * 255) + 0.5f);
	DX::g_deviceContext->Unmap(m_pickingReadBuffer, 0);
	//m_depthStencilView->Release();
	if (index == 0)
		return nullptr;
	return DX::g_pickingQueue[index - 1];
}
