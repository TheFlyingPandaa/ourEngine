#include "Picking.h"
#include <d3d11.h>
#include "../Core/Dx.h"
#include "../interface/Input.h"
#pragma comment (lib, "d3d11.lib")

struct OFFSETBUFFER {
	float temp;
	float temp2;	//Padding
	float temp3;
	float temp4;
};

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

Shape * Picking::getPicked(Camera * c, ID3D11RenderTargetView*&RTV, ID3D11DepthStencilView*& m_depthStencilView, const DirectX::XMMATRIX m_projectionMatrix, DirectX::XMMATRIX m_HUDview, ID3D11Buffer*& m_pickingBuffer, ID3D11VertexShader*& m_pickingVertexShader, ID3D11PixelShader*& m_pickingPixelShader, ID3D11Texture2D*& TextureMap, ID3D11Texture2D*& m_pickingReadBuffer, ID3D11Buffer *& m_meshConstantBuffer, ID3D11Buffer* &m_pickingOffsetBuffer)
{
	DX::g_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DX::g_deviceContext->IASetInputLayout(DX::g_inputLayout);

	float color[4]{ 0.0f, 0.0f, 0.0f, 1.0f };

	DX::g_deviceContext->ClearRenderTargetView(RTV, color);
	DX::g_deviceContext->OMSetRenderTargets(1, &RTV, m_depthStencilView);

	DirectX::XMMATRIX view = c->getViewMatrix();
	DirectX::XMMATRIX viewProj = view * m_projectionMatrix;


	DX::g_deviceContext->VSSetShader(m_pickingVertexShader, nullptr, 0);
	DX::g_deviceContext->HSSetShader(nullptr, nullptr, 0);
	DX::g_deviceContext->DSSetShader(nullptr, nullptr, 0);
	DX::g_deviceContext->GSSetShader(nullptr, nullptr, 0);
	DX::g_deviceContext->PSSetShader(m_pickingPixelShader, nullptr, 0);


	MESH_BUFFER meshBuffer;
	OFFSETBUFFER offsetBuffer;

	ID3D11Buffer* instanceBuffer = nullptr;
	int counter = 0;
	for (auto& instance : DX::g_instanceGroupsPicking)
	{
		D3D11_BUFFER_DESC instBuffDesc;
		memset(&instBuffDesc, 0, sizeof(instBuffDesc));
		instBuffDesc.Usage = D3D11_USAGE_DEFAULT;
		instBuffDesc.ByteWidth = sizeof(DX::INSTANCE_ATTRIB) * (UINT)instance.attribs.size();
		instBuffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA instData;
		memset(&instData, 0, sizeof(instData));
		instData.pSysMem = &instance.attribs[0];
		HRESULT hr = DX::g_device->CreateBuffer(&instBuffDesc, &instData, &instanceBuffer);


		DirectX::XMMATRIX vp = DirectX::XMMatrixTranspose(viewProj);
		if (dynamic_cast<RectangleShape*>(instance.shape) && dynamic_cast<RectangleShape*>(instance.shape)->isHud())
			vp = DirectX::XMMatrixTranspose(m_HUDview);

		DirectX::XMStoreFloat4x4A(&meshBuffer.VP, vp);

		D3D11_MAPPED_SUBRESOURCE dataPtr;
		DX::g_deviceContext->Map(m_meshConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);
		memcpy(dataPtr.pData, &meshBuffer, sizeof(MESH_BUFFER));
		DX::g_deviceContext->Unmap(m_meshConstantBuffer, 0);
		DX::g_deviceContext->VSSetConstantBuffers(0, 1, &m_meshConstantBuffer);

		offsetBuffer.temp = static_cast<float>(counter);

		DX::g_deviceContext->Map(m_pickingOffsetBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);
		memcpy(dataPtr.pData, &offsetBuffer, sizeof(OFFSETBUFFER));
		DX::g_deviceContext->Unmap(m_pickingOffsetBuffer, 0);
		DX::g_deviceContext->VSSetConstantBuffers(1, 1, &m_pickingOffsetBuffer);

		UINT32 vertexSize = sizeof(VERTEX);
		UINT offset = 0;
		ID3D11Buffer* v = instance.shape->getMesh()->getVertices();
		ID3D11Buffer * bufferPointers[2];
		bufferPointers[0] = v;
		bufferPointers[1] = instanceBuffer;

		unsigned int strides[2];
		strides[0] = sizeof(VERTEX);
		strides[1] = sizeof(DX::INSTANCE_ATTRIB);

		unsigned int offsets[2];
		offsets[0] = 0;
		offsets[1] = 0;

		Mesh* mesh = instance.shape->getMesh();
		ID3D11Buffer* indices = mesh->getIndicesBuffer();

		DX::g_deviceContext->IASetIndexBuffer(indices, DXGI_FORMAT_R32_UINT, offset);
		DX::g_deviceContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);

		DX::g_deviceContext->DrawIndexedInstanced(instance.shape->getMesh()->getNrOfIndices(), (UINT)instance.attribs.size(), 0, 0, 0);
		counter += static_cast<int>(instance.attribs.size());
		instanceBuffer->Release();
	}


	DX::g_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	
	D3D11_BOX srcBox;
	srcBox.left = static_cast<UINT>(Input::getMousePosition().x);
	srcBox.right = srcBox.left + 1;
	srcBox.top = static_cast<UINT>(Input::getMousePosition().y);
	srcBox.bottom = srcBox.top + 1;
	srcBox.front = 0;
	srcBox.back = 1;


	DX::g_deviceContext->CopySubresourceRegion(m_pickingReadBuffer, 0, 0, 0, 0, TextureMap, 0, &srcBox);
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = DX::g_deviceContext->Map(m_pickingReadBuffer, 0, D3D11_MAP_READ, 0, &mappedResource);
	DirectX::XMFLOAT4A pixel = *((DirectX::XMFLOAT4A*)mappedResource.pData);
	DX::g_deviceContext->Unmap(m_pickingReadBuffer, 0);

	Shape * returnValue = nullptr;

	long index = static_cast<int>(pixel.x + (pixel.y * 256) + (pixel.z * 256 * 256) + 0.5f);
	if (index != 0)
	{
		index--;
		long indexInPickingQueue = 0;
		bool found = false;
	
		for (size_t i = 0; i < DX::g_instanceGroupsPicking.size() && !found; i++)
		{
			long nrOfShapes = static_cast<long>(DX::g_instanceGroupsPicking[i].index.size());
			if (index < nrOfShapes)
			{
				indexInPickingQueue = DX::g_instanceGroupsPicking[i].index[index];
				found = true;
			}
			else
				index -= nrOfShapes;
		}
		returnValue = DX::g_pickingQueue[indexInPickingQueue];
	}

	DX::g_pickingQueue.clear();
	DX::g_instanceGroupsPicking.clear();

	return returnValue;
}
