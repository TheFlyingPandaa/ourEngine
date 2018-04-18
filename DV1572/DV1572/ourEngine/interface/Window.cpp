﻿#include "Window.h"
#include "../core/Dx.h"
#include <thread>
#include "../core/Picking.h"
#include <chrono>

#define DEBUG 1
//Devices
ID3D11Device* DX::g_device;
ID3D11DeviceContext* DX::g_deviceContext;

//Standard Shaders and Input
ID3D11VertexShader* DX::g_3DVertexShader;
ID3D11PixelShader* DX::g_3DPixelShader;
ID3D11InputLayout* DX::g_inputLayout;

//Rendering Queues
std::vector<Shape*> DX::g_renderQueue;
std::vector<Shape*> DX::g_skyBoxQueue;
std::vector<Shape*> DX::g_shadowQueue;
std::vector<Shape*> DX::g_transQueue;
std::vector<Shape*> DX::g_pickingQueue;
std::vector<Shape*> DX::g_HUDQueue;
std::vector<PointLight*> DX::g_lightQueue; 

//Standard Tessellation
ID3D11HullShader* DX::g_standardHullShader;
ID3D11DomainShader* DX::g_standardDomainShader;

//SkyBoxShaders
ID3D11VertexShader* DX::g_skyBoxVertexShader;
ID3D11PixelShader* DX::g_skyBoxPixelShader;

std::vector<DX::INSTANCE_GROUP>				DX::g_instanceGroups;
std::vector<DX::INSTANCE_GROUP>				DX::g_instanceGroupsSkyBox;
std::vector<DX::INSTANCE_GROUP>				DX::g_instanceGroupsHUD;
std::vector<DX::INSTANCE_GROUP>				DX::g_instanceGroupsTransparancy;
std::vector<DX::INSTANCE_GROUP_INDEXED>		DX::g_instanceGroupsPicking;
std::vector<DX::INSTANCE_GROUP>				DX::g_InstanceGroupsShadow;

//TEXT
std::vector<std::unique_ptr<DirectX::SpriteFont>>	DX::g_fonts;
std::vector<Text*>									DX::g_textQueue;
std::unique_ptr<DirectX::SpriteBatch>				DX::g_spriteBatch;

XMFLOAT4A											DX::g_lightPos;
XMFLOAT4A											DX::g_lightDir;

void DX::submitToInstance(Shape* shape, std::vector<DX::INSTANCE_GROUP>& queue)
{
	
	int existingId = -1;
	for (int i = 0; i < queue.size() && existingId == -1; i++)
	{
		if (shape->getMesh()->CheckID(*queue[i].shape->getMesh()))
		{
			existingId = i;

		}
	}


	//Converting The worldMatrix into a instanced world matrix.
	//This allowes us to send in the matrix in the layout and now a constBuffer
	INSTANCE_ATTRIB attribDesc;
	
	XMMATRIX xmWorldMat = shape->getWorld();
	XMFLOAT4X4A worldMat;
	
	XMStoreFloat4x4A(&worldMat, xmWorldMat);

	XMFLOAT4A rows[4];
	for (int i = 0; i < 4; i++)
	{
		rows[i].x = worldMat.m[i][0];
		rows[i].y = worldMat.m[i][1];
		rows[i].z = worldMat.m[i][2];
		rows[i].w = worldMat.m[i][3];
	}

	
	attribDesc.w1 = rows[0];
	attribDesc.w2 = rows[1];
	attribDesc.w3 = rows[2];
	attribDesc.w4 = rows[3];

	attribDesc.highLightColor = shape->getColor(); //This allowes us to use a "click highlight"
	attribDesc.lightIndex = static_cast<float>(shape->getLightIndex());
	
	// Unique Mesh
	if (existingId == -1)
	{
		//If the queue dose not exist we create a new queue.
		//This is what allows the instancing to work
		INSTANCE_GROUP newGroup;
		newGroup.attribs.push_back(attribDesc);
		newGroup.shape = shape;
		queue.push_back(newGroup);
	}
	else
	{
		//If the mesh allready exists we just push it into a exsiting queue
		queue[existingId].attribs.push_back(attribDesc);
	}
	
}
void DX::submitToInstance(Shape* shape, std::vector<DX::INSTANCE_GROUP_INDEXED>& queue)
{
	int existingId = -1;
	for (int i = 0; i < queue.size() && existingId == -1; i++)
	{
		if (shape->getMesh()->CheckID(*queue[i].shape->getMesh()))
		{
			existingId = i;

		}
	}

	//Converting The worldMatrix into a instanced world matrix.
	//This allowes us to send in the matrix in the layout and now a constBuffer
	INSTANCE_ATTRIB attribDesc;

	XMMATRIX xmWorldMat = shape->getWorld();
	XMFLOAT4X4A worldMat;
	long index =static_cast<long>(DX::g_pickingQueue.size() - 1);

	XMStoreFloat4x4A(&worldMat, xmWorldMat);

	XMFLOAT4A rows[4];
	for (int i = 0; i < 4; i++)
	{
		rows[i].x = worldMat.m[i][0];
		rows[i].y = worldMat.m[i][1];
		rows[i].z = worldMat.m[i][2];
		rows[i].w = worldMat.m[i][3];
	}


	attribDesc.w1 = rows[0];
	attribDesc.w2 = rows[1];
	attribDesc.w3 = rows[2];
	attribDesc.w4 = rows[3];

	attribDesc.highLightColor = shape->getColor(); //This allowes us to use a "click highlight"


												   // Unique Mesh
	if (existingId == -1)
	{
		//If the queue dose not exist we create a new queue.
		//This is what allows the instancing to work
		INSTANCE_GROUP_INDEXED newGroup;
		newGroup.attribs.push_back(attribDesc);
		newGroup.shape = shape;
		newGroup.index.push_back(index);
		queue.push_back(newGroup);
		
	}
	else
	{
		//If the mesh allready exists we just push it into a exsiting queue
		queue[existingId].attribs.push_back(attribDesc);
		queue[existingId].index.push_back(index);
	}

}

void DX::CleanUp()
{
	DX::g_device->Release();
	DX::g_deviceContext->Release();
	DX::g_3DVertexShader->Release();
	DX::g_3DPixelShader->Release();
	DX::g_inputLayout->Release();
	DX::g_standardHullShader->Release();
	DX::g_standardDomainShader->Release();
}



bool Window::_initWindow()
{
	HICON hIicon = (HICON)LoadImage( // returns a HANDLE so we have to cast to HICON
		NULL,             // hInstance must be NULL when loading from a file
		"trolls_inn/Resources/favicon.ico",   // the icon file name
		IMAGE_ICON,       // specifies that the file is an icon
		0,                // width of the image (we'll specify default later on)
		0,                // height of the image
		LR_LOADFROMFILE |  // we want to load a file (as opposed to a resource)
		LR_DEFAULTSIZE |   // default metrics based on the type (IMAGE_ICON, 32x32)
		LR_SHARED         // let the system release the handle when it's no longer used
	);

	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = StaticWndProc;
	wcex.hInstance = m_hInstance;
	wcex.lpszClassName = "ourEngine";
	wcex.hIcon = hIicon;
	if (!RegisterClassEx(&wcex))
	{
		m_hwnd = false;
		return false;
	}

	RECT rc = { 0, 0, static_cast<long>(m_width), static_cast<long>(m_height) };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	HWND handle = CreateWindow(
		wcex.lpszClassName,
		m_title,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		nullptr,
		nullptr,
		m_hInstance,
		this);

	m_hwnd = handle;
	return true;
}

HRESULT Window::_initDirect3DContext()
{
	UINT createDeviceFlags = 0;

#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif //DEBUG

	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = m_hwnd;								// the window to be used
	scd.SampleDesc.Count = m_sampleCount;                   // how many multisamples
	scd.Windowed = !m_fullscreen;							// windowed/full-screen mode

	// create a device, device context and swap chain using the information in the scd struct
	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,//D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&m_swapChain,
		&DX::g_device,
		NULL,
		&DX::g_deviceContext);

	if (SUCCEEDED(hr))
	{
		// get the address of the back buffer
		ID3D11Texture2D* pBackBuffer = nullptr;
		m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		// use the back buffer address to create the render target
		DX::g_device->CreateRenderTargetView(pBackBuffer, NULL, &m_backBufferRTV);
		//we are creating the standard depth buffer here.
		_createDepthBuffer();
		_loadShadowBuffers();
		DX::g_deviceContext->OMSetRenderTargets(1, &m_backBufferRTV, m_depthStencilView);	//As a standard we set the rendertarget. But it will be changed in the prepareGeoPass
		pBackBuffer->Release();
	}
	return hr;
}

void Window::_initViewPort()
{
	m_viewport.Width = static_cast<float>(m_width);
	m_viewport.Height = static_cast<float>(m_height);
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
	m_viewport.TopLeftX = 0;
	m_viewport.TopLeftY = 0;
}

void Window::_setViewport()
{
	
	DX::g_deviceContext->RSSetViewports(1, &m_viewport);
}

void Window::_compileShaders()
{
	// Vertex Shader
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXELS", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		// INSTANCE ATTRIBUTES
		{ "INSTANCEWORLDONE", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCEWORLDTWO", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCEWORLDTHREE", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCEWORLDFOUR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		//This is the attribute that allows the color change without constant buffer
		{ "HIGHLIGHTCOLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "LIGHTINDEX", 0, DXGI_FORMAT_R32_FLOAT, 1, 80, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
	};
	ShaderCreator::CreateVertexShader(DX::g_device, DX::g_3DVertexShader,
		L"ourEngine/shaders/3DVertex.hlsl", "main",
		inputDesc, ARRAYSIZE(inputDesc), DX::g_inputLayout);
	ShaderCreator::CreatePixelShader(DX::g_device, DX::g_3DPixelShader,
		L"ourEngine/shaders/3DPixel.hlsl", "main");

	ShaderCreator::CreateVertexShader(DX::g_device, m_deferredVertexShader,
		L"ourEngine/Shaders/deferredVertexShader.hlsl", "main");
	ShaderCreator::CreatePixelShader(DX::g_device, m_deferredPixelShader,
		L"ourEngine/shaders/deferredPixelShader.hlsl", "main");

	ShaderCreator::CreateVertexShader(DX::g_device, m_transVertexShader,
		L"ourEngine/shaders/transVertexShader.hlsl", "main");
	ShaderCreator::CreatePixelShader(DX::g_device, m_transPixelShader,
		L"ourEngine/shaders/transPixelShader.hlsl", "main");

	//Compaile Computeshader
	ShaderCreator::CreateComputeShader(DX::g_device, m_computeShader,
		L"ourEngine/shaders/testComputeShader.hlsl", "main");

	//HUD Shader
	ShaderCreator::CreateVertexShader(DX::g_device, m_hudVertexShader,
		L"ourEngine/Shaders/hudVertexShader.hlsl", "main");
	ShaderCreator::CreatePixelShader(DX::g_device, m_hudPixelShader,
		L"ourEngine/shaders/hudPixelShader.hlsl", "main");

	ShaderCreator::CreateVertexShader(DX::g_device, DX::g_skyBoxVertexShader,
		L"ourEngine/Shaders/skyBoxVertexShader.hlsl", "main");
	ShaderCreator::CreatePixelShader(DX::g_device, DX::g_skyBoxPixelShader,
		L"ourEngine/shaders/skyBoxPixelShader.hlsl", "main");

	//Shadow
	ShaderCreator::CreateVertexShader(DX::g_device, m_shadowVertex,
		L"ourEngine/shaders/ShadowVertex.hlsl", "main");
	ShaderCreator::CreatePixelShader(DX::g_device, m_shadowPixel,
		L"ourEngine/shaders/ShadowPixel.hlsl", "main");

	_initPickingShaders();
	_initTessellationShaders();


}

void Window::_initPickingShaders()
{
	ShaderCreator::CreateVertexShader(DX::g_device, m_pickingVertexShader,
		L"ourEngine/Shaders/pickingVertexShader.hlsl", "main");

	ShaderCreator::CreatePixelShader(DX::g_device, m_pickingPixelShader,
		L"ourEngine/shaders/pickingPixelShader.hlsl", "main");
}

void Window::_initTessellationShaders()
{

	ShaderCreator::CreateHullShader(DX::g_device, DX::g_standardHullShader,
		L"ourEngine/Shaders/standardHullShader.hlsl", "main");

	ShaderCreator::CreateDomainShader(DX::g_device, DX::g_standardDomainShader,
		L"ourEngine/shaders/standardDomainShader.hlsl", "main");
}

void Window::_drawHUD()
{
	//The hud has a special kind of view matrix.
	//This allowes it to stick to the screen
	DirectX::XMMATRIX viewProj = m_HUDviewProj;
	DX::g_deviceContext->OMSetBlendState(m_transBlendState, 0, 0xffffffff);

	MESH_BUFFER meshBuffer;
	ID3D11Buffer* instanceBuffer = nullptr;

	//We change the vertexShaders and so on here, this is done because don't want to change rectangle.
	DX::g_deviceContext->IASetInputLayout(DX::g_inputLayout);
	DX::g_deviceContext->VSSetShader(m_hudVertexShader, nullptr, 0);
	DX::g_deviceContext->HSSetShader(nullptr, nullptr, 0);
	DX::g_deviceContext->DSSetShader(nullptr, nullptr, 0);
	DX::g_deviceContext->GSSetShader(nullptr, nullptr, 0);
	DX::g_deviceContext->PSSetShader(m_hudPixelShader, nullptr, 0);
	DX::g_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DX::g_deviceContext->OMSetRenderTargets(1, &m_backBufferRTV, m_depthStencilView);

	for (auto& instance : DX::g_instanceGroupsHUD)	//Every instance has it's own queue
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
		//We copy the data into the attribute part of the layout.
		//This is what makes instancing special

		DirectX::XMMATRIX vp = DirectX::XMMatrixTranspose(viewProj);	//Grabing the viewMatrix
		DirectX::XMStoreFloat4x4A(&meshBuffer.VP, vp);				

		//Sending the viewProj matrix in to a constant buffer, this is to get the positions in the end.
		D3D11_MAPPED_SUBRESOURCE dataPtr;
		DX::g_deviceContext->Map(m_meshConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);
		memcpy(dataPtr.pData, &meshBuffer, sizeof(MESH_BUFFER));
		DX::g_deviceContext->Unmap(m_meshConstantBuffer, 0);
		DX::g_deviceContext->VSSetConstantBuffers(0, 1, &m_meshConstantBuffer);

		instance.shape->ApplyMaterials();	//Applymaterials won't change the shaders, it will only copy the textures and whatever to the gpu

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
		instanceBuffer->Release();
	}
}

void Window::_initComputeShader()
{
	//What makes compute shaders wierd is the buffer
	//To make it work we need a standard constantBuffer to get the viewmatrix or whatever.
	//But to get the output we need to create 3 things
	//1 buffer that is conected to the UAV so we can ge the data out of the gpu
	//and then the a buffer to copy from the UAV buffer to a buffer that we can read in the CPU. it's fucking retarded.
	//Nvida pls fix getData();

	HRESULT hr;
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(computeBuffer);
	//bufferDesc.ByteWidth = sizeof(float) * 4;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	hr = 0;
	hr = DX::g_device->CreateBuffer(&bufferDesc, nullptr, &m_computeConstantBuffer);
	if (FAILED(hr))
	{
		// handle the error, could be fatal or a warning...
		exit(-1);
	}
	//OUTPUTBUFFER
	D3D11_BUFFER_DESC outputDesc;
	outputDesc.Usage = D3D11_USAGE_DEFAULT;
	outputDesc.ByteWidth = sizeof(computeBuffer);// *NUM_PARTICLES;
	//bufferDesc.ByteWidth = sizeof(float) * 4;
	outputDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	outputDesc.CPUAccessFlags = 0;
	outputDesc.StructureByteStride = sizeof(computeBuffer);
	//bufferDesc.ByteWidth = sizeof(float) * 4;
	outputDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

	hr = 0;
	hr = DX::g_device->CreateBuffer(&outputDesc, 0, &m_computeOutputBuffer);
	if (FAILED(hr))
	{
		// handle the error, could be fatal or a warning...
		exit(-1);
	}
	//SAME SHIT ASS ABOVE BUT FOR OTHER
	outputDesc.Usage = D3D11_USAGE_STAGING;
	outputDesc.BindFlags = 0;
	outputDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	hr = DX::g_device->CreateBuffer(&outputDesc, 0, &m_computeReadWriteBuffer);
	if (FAILED(hr))
	{
		// handle the error, could be fatal or a warning...
		exit(-1);
	}

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.Flags = 0;
	//uavDesc.Buffer.NumElements = NUM_PARTICLES; //Number of "particles"
	uavDesc.Buffer.NumElements = 1;
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;

	hr = DX::g_device->CreateUnorderedAccessView(m_computeOutputBuffer, &uavDesc, &m_computeUAV);
	if (FAILED(hr))
	{
		// handle the error, could be fatal or a warning...
		exit(-1);
	}
}

void Window::_runComputeShader() {
	//The computeShader works like every other thingy. 
	//You set the assinged shader
	//Then run it. 
	//then the retarded thing where we get the output 
	DX::g_deviceContext->CSSetShader(m_computeShader, NULL, 0);

	//D3D11_MAPPED_SUBRESOURCE dataPtr;
	//gDeviceContext->Map(computeBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);

	//computeValuesStore.val = 1;
	//computeValuesStore.output = XMFLOAT2(0, 0); //Need Padding
	//computeValuesStore.camPos = XMFLOAT2(XMVectorGetX(cameraPos), XMVectorGetZ(cameraPos));
	//computeValuesStore.objectPos = XMFLOAT2(renderObject->getPosition().x, renderObject->getPosition().z);
	//memcpy(dataPtr.pData, &computeValuesStore, sizeof(computeShader));
	//// UnMap constant buffer so that we can use it again in the GPU
	//gDeviceContext->Unmap(computeBuffer, 0);

	DX::g_deviceContext->CSSetConstantBuffers(0, 1, &m_computeConstantBuffer);
	DX::g_deviceContext->CSSetUnorderedAccessViews(0, 1, &m_computeUAV, NULL);

	DX::g_deviceContext->Dispatch(1, 1, 1);	//This is where we run it. by changing the number we can get more groupes

	ID3D11UnorderedAccessView* nullUAV[] = { NULL };	//NUll everything to please directx
	DX::g_deviceContext->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);
	DX::g_deviceContext->CSSetShader(NULL, NULL, 0);
}

void Window::_loadShadowBuffers()
{
	D3D11_BUFFER_DESC bDesc;
	bDesc.Usage = D3D11_USAGE_DYNAMIC;
	bDesc.ByteWidth = sizeof(SHADOW_MATRIX_BUFFER);
	bDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bDesc.MiscFlags = 0;
	bDesc.StructureByteStride = 0;

	HRESULT hr = DX::g_device->CreateBuffer(&bDesc, nullptr, &m_shadowBuffer);

	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = m_width;
	texDesc.Height = m_height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	hr = DX::g_device->CreateTexture2D(&texDesc, NULL, &m_depthBufferTexShad);

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Flags = 0;
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	hr = DX::g_device->CreateDepthStencilView(m_depthBufferTexShad, &dsvDesc, &m_depthStencilViewShad);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;

	hr = DX::g_device->CreateShaderResourceView(m_depthBufferTexShad, &srvDesc, &m_shadowDepthTexture);

	//m_shadowProjMatrix = XMMatrixPerspectiveFovLH(XM_PI * 0.75f, 16.0f / 9.0f, 0.5f, 500.0f);
	m_shadowProjMatrix = XMMatrixOrthographicLH(m_width * 0.05f, m_height * 0.05f, 1.0f, 50.0f);
}

void Window::_prepareShadow()
{

	//DX::g_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	DX::g_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DX::g_deviceContext->IASetInputLayout(DX::g_inputLayout);
	DX::g_deviceContext->VSSetShader(m_shadowVertex, nullptr, 0);
	DX::g_deviceContext->HSSetShader(nullptr, nullptr, 0);
	DX::g_deviceContext->DSSetShader(nullptr, nullptr, 0);
	DX::g_deviceContext->GSSetShader(nullptr, nullptr, 0);
	DX::g_deviceContext->PSSetShader(nullptr, nullptr, 0);
	DX::g_deviceContext->OMSetRenderTargets(0, nullptr, m_depthStencilViewShad);
}

void Window::_shadowPass(Camera* c)
{

	XMFLOAT4 pos = XMFLOAT4(0, 10, 0, 1);
	XMFLOAT4 lookAt = XMFLOAT4(16, 0, 16, 1);
	XMFLOAT4 up = XMFLOAT4(0, 1, 0, 0);
	XMVECTOR p;
	XMVECTOR l;
	//p = XMLoadFloat4(&pos);
	//l = XMLoadFloat4(&lookAt);

	//p = XMLoadFloat4(&DX::g_lightPos);
	l = XMVector3Normalize(XMLoadFloat4(&DX::g_lightDir));
	XMVECTOR look = XMLoadFloat4(&lookAt);

	XMVECTOR u = XMLoadFloat4(&up);
	p =  look + (-l *20.0f);
	p = XMVectorSetW(p, 1.0f);
	

	XMMATRIX view = XMMatrixLookAtLH(p, look, u);
	
	SHADOW_MATRIX_BUFFER meshBuffer;

	XMStoreFloat4x4A(&meshBuffer.view, XMMatrixTranspose(view));
	XMStoreFloat4x4A(&meshBuffer.projection, XMMatrixTranspose(m_shadowProjMatrix));

	ID3D11Buffer* instanceBuffer = nullptr;
	for (auto& instance : DX::g_InstanceGroupsShadow)
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

		D3D11_MAPPED_SUBRESOURCE dataPtr;
		DX::g_deviceContext->Map(m_shadowBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);
		memcpy(dataPtr.pData, &meshBuffer, sizeof(SHADOW_MATRIX_BUFFER));
		DX::g_deviceContext->Unmap(m_shadowBuffer, 0);
		
		DX::g_deviceContext->VSSetConstantBuffers(9, 1, &m_shadowBuffer);


	

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
		instanceBuffer->Release();
	}

	
}

void Window::_initFonts()
{
	DX::g_fonts.push_back(std::make_unique<SpriteFont>(DX::g_device, L"trolls_inn/Resources/Fonts/myfile.spritefont"));
	DX::g_spriteBatch = std::make_unique<SpriteBatch>(DX::g_deviceContext);
}

void Window::_drawText()
{
	DX::g_spriteBatch->Begin();

	for (auto &t : DX::g_textQueue)
	{
		std::string str = t->getTextString();
		std::wstring widestr = std::wstring(str.begin(), str.end());
		const wchar_t* text = widestr.c_str();

		DirectX::XMVECTOR origin = DX::g_fonts[t->getFontType()]->MeasureString(text);

		float x, y;
		x = DirectX::XMVectorGetX(t->getPosition());
		y = DirectX::XMVectorGetY(t->getPosition());

		DirectX::XMVECTOR pos =
			DirectX::XMVectorSet(x, (float)m_height - y, 0,1);

		switch (t->getAllignment())
		{
		case TXT::ALLIGN::Center:
			origin /= 2;
			break;
		case TXT::ALLIGN::Left:
			origin = XMVectorSetX(origin, 0);
			break;
		}


		DX::g_fonts[t->getFontType()]->DrawString(
			DX::g_spriteBatch.get(),
			text,
			pos,
			t->getColor(),
			t->getRotation(),
			origin,
			t->getScale()
		);
	}

	DX::g_spriteBatch->End();
}

void Window::_setSamplerState()
{
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	DX::g_device->CreateSamplerState(&samplerDesc, &m_samplerState);
	DX::g_deviceContext->PSSetSamplers(0, 1, &m_samplerState);
}

void Window::_createConstantBuffers()
{
	_createMeshConstantBuffer();
	_createPickConstantBuffer();
	_createCameraPosConstantBuffer();
	_createPointLightCollectionBuffer(); 
}

void Window::_createMeshConstantBuffer()
{
	D3D11_BUFFER_DESC bDesc;
	bDesc.Usage = D3D11_USAGE_DYNAMIC;
	bDesc.ByteWidth = sizeof(MESH_BUFFER);
	bDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bDesc.MiscFlags = 0;
	bDesc.StructureByteStride = 0;

	HRESULT hr = DX::g_device->CreateBuffer(&bDesc, nullptr, &m_meshConstantBuffer);
}

void Window::_createPickConstantBuffer()
{
	D3D11_BUFFER_DESC bDesc;
	bDesc.Usage = D3D11_USAGE_DYNAMIC;
	bDesc.ByteWidth = sizeof(PICK_BUFFER);
	bDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bDesc.MiscFlags = 0;
	bDesc.StructureByteStride = 0;

	HRESULT hr = DX::g_device->CreateBuffer(&bDesc, nullptr, &m_pickingBuffer);
}

void Window::_createCameraPosConstantBuffer()
{
	D3D11_BUFFER_DESC bDesc; 
	bDesc.Usage = D3D11_USAGE_DYNAMIC;
	bDesc.ByteWidth = sizeof(CAMERA_POS_BUFFER);
	bDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bDesc.MiscFlags = 0;
	bDesc.StructureByteStride = 0;

	HRESULT hr = DX::g_device->CreateBuffer(&bDesc, nullptr, &m_cameraPosConstantBuffer); 
}

void Window::_createPointLightCollectionBuffer()
{
	D3D11_BUFFER_DESC bufferDesc; 

	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(POINT_LIGHT_COLLECTION);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	HRESULT hr = DX::g_device->CreateBuffer(&bufferDesc, nullptr, &m_pPointLightBuffer);
}

void Window::_createDepthBuffer()
{
	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = m_width;
	depthStencilDesc.Height = m_height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = m_sampleCount;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	//Create the Depth/Stencil View
	HRESULT hr = DX::g_device->CreateTexture2D(&depthStencilDesc, NULL, &m_depthBufferTex);
	hr = DX::g_device->CreateDepthStencilView(m_depthBufferTex, NULL, &m_depthStencilView);



	//hr = DX::g_device->CreateTexture2D(&depthStencilDesc, NULL, &m_depthBufferTexShad);
	//hr = DX::g_device->CreateDepthStencilView(m_depthBufferTexShad, NULL, &m_depthStencilViewShad);
}

void Window::_initGBuffer()
{
	D3D11_TEXTURE2D_DESC tDesc{};
	tDesc.Width = m_width;
	tDesc.Height = m_height;
	tDesc.MipLevels = 1;
	tDesc.ArraySize = 1;
	tDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	tDesc.SampleDesc.Count = m_sampleCount;
	tDesc.Usage = D3D11_USAGE_DEFAULT;
	tDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	D3D11_RENDER_TARGET_VIEW_DESC rDesc{};
	rDesc.Format = tDesc.Format;
	rDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	D3D11_SHADER_RESOURCE_VIEW_DESC sDesc{};
	sDesc.Format = tDesc.Format;
	sDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	sDesc.Texture2D.MipLevels = 1;

	//The GBuffer Has 3 elements. A texture that we can write our output to, a rendertarget that has the texture maped to it
	//And then a shaderResourceView to be able to access it at a later date
	for (auto &g : m_gbuffer)
	{
		DX::g_device->CreateTexture2D(&tDesc, nullptr, &g.TextureMap);
		DX::g_device->CreateRenderTargetView(g.TextureMap, &rDesc, &g.RTV);
		DX::g_device->CreateShaderResourceView(g.TextureMap, &sDesc, &g.SRV);
	}

}

void Window::_prepareGeometryPass()
{
	/*float c[4] = { 0.0f,0.0f,0.0f,1.0f };
	DX::g_deviceContext->ClearRenderTargetView(m_backBufferRTV, c);
	DX::g_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);*/

	//The patchlist is used for tessellation, the tessellator takes patches not points
	DX::g_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	DX::g_deviceContext->IASetInputLayout(DX::g_inputLayout);

	ID3D11RenderTargetView* renderTargets[GBUFFER_COUNT];

	for (int i = 0; i < GBUFFER_COUNT; i++)
	{
		renderTargets[i] = m_gbuffer[i].RTV;
	}
	DX::g_deviceContext->OMSetRenderTargets(GBUFFER_COUNT, renderTargets, m_depthStencilView);
}

void Window::_geometryPass(const Camera &cam)
{

	DirectX::XMMATRIX view = cam.getViewMatrix();			//Getting the view matrix from the camera.
	DirectX::XMMATRIX viewProj = view * m_projectionMatrix;	//The smashing it with projection

	MESH_BUFFER meshBuffer;

	if (m_WireFrameDebug == true)
	{
		D3D11_RASTERIZER_DESC wfdesc;
		ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
		wfdesc.FillMode = D3D11_FILL_WIREFRAME;
		wfdesc.CullMode = D3D11_CULL_NONE;
		DX::g_device->CreateRasterizerState(&wfdesc, &m_WireFrame);
		DX::g_deviceContext->RSSetState(m_WireFrame);
	}

	ID3D11Buffer* instanceBuffer = nullptr;

	for (auto& instance : DX::g_instanceGroups)
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
		//We copy the data into the attribute part of the layout.
		//This is what makes instancing special
		
		

		DirectX::XMMATRIX vp = DirectX::XMMatrixTranspose(viewProj);
		DirectX::XMStoreFloat4x4A(&meshBuffer.VP, vp);
		meshBuffer.gridscale = static_cast<float>(instance.shape->getGridScale());

		D3D11_MAPPED_SUBRESOURCE dataPtr;
		DX::g_deviceContext->Map(m_meshConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);
		memcpy(dataPtr.pData, &meshBuffer, sizeof(MESH_BUFFER));
		DX::g_deviceContext->Unmap(m_meshConstantBuffer, 0);
		DX::g_deviceContext->DSSetConstantBuffers(0, 1, &m_meshConstantBuffer);
		DX::g_deviceContext->VSSetConstantBuffers(0, 1, &m_meshConstantBuffer);
		
		// Apply shaders
		instance.shape->ApplyShaders(); //ApplyShaders will set the special shaders

		for (int i = 0; i < instance.shape->getMesh()->getNumberOfParts(); i++)
		{
			instance.shape->ApplyMaterials(i);

			UINT32 vertexSize = sizeof(VERTEX);
			UINT offset = 0;
			ID3D11Buffer* v = instance.shape->getMesh()->getVertices(i);
			ID3D11Buffer * bufferPointers[2];
			bufferPointers[0] = v;
			bufferPointers[1] = instanceBuffer;

			unsigned int strides[2];
			strides[0] = sizeof(VERTEX);
			strides[1] = sizeof(DX::INSTANCE_ATTRIB);

			unsigned int offsets[2];
			offsets[0] = 0;
			offsets[1] = 0;

			ID3D11Buffer* indices = instance.shape->getMesh()->getIndicesBuffer(i);

			DX::g_deviceContext->IASetIndexBuffer(indices, DXGI_FORMAT_R32_UINT, offset);
			DX::g_deviceContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);
																					
			DX::g_deviceContext->DrawIndexedInstanced(instance.shape->getMesh()->getNrOfIndices(i), (UINT)instance.attribs.size(), 0, 0, 0);
		}
		
		instanceBuffer->Release();
	}

	if (m_WireFrameDebug == true)
	{
		D3D11_RASTERIZER_DESC wfdesc;
		ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
		wfdesc.FillMode = D3D11_FILL_SOLID;
		wfdesc.CullMode = D3D11_CULL_BACK;
		DX::g_device->CreateRasterizerState(&wfdesc, &m_WireFrame);
		DX::g_deviceContext->RSSetState(m_WireFrame);
	}

}

void Window::_skyBoxPass(const Camera& cam)
{
	
	DX::g_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	DirectX::XMMATRIX view = XMMatrixLookToLH(
		DirectX::XMVectorSet(0, 0, 0, 1),
		DirectX::XMLoadFloat3(&cam.getLookAt()),
		DirectX::XMVectorSet(0, 1, 0, 0)
	);//Getting the view matrix from the camera.
	DirectX::XMMATRIX viewProj = view * m_projectionMatrix;	//The smashing it with projection

	MESH_BUFFER meshBuffer;

	
	ID3D11Buffer* instanceBuffer = nullptr;

	for (auto& instance : DX::g_instanceGroupsSkyBox)
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
		//We copy the data into the attribute part of the layout.
		//This is what makes instancing special

		DirectX::XMMATRIX vp = DirectX::XMMatrixTranspose(viewProj);
		DirectX::XMStoreFloat4x4A(&meshBuffer.VP, vp);

		D3D11_MAPPED_SUBRESOURCE dataPtr;
		DX::g_deviceContext->Map(m_meshConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);
		memcpy(dataPtr.pData, &meshBuffer, sizeof(MESH_BUFFER));
		DX::g_deviceContext->Unmap(m_meshConstantBuffer, 0);
		DX::g_deviceContext->VSSetConstantBuffers(0, 1, &m_meshConstantBuffer);

		instance.shape->ApplyShaders(); 
		instance.shape->ApplyMaterials();

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
		instanceBuffer->Release();
	}
}


void Window::_clearTargets()
{
	ID3D11RenderTargetView* renderTargets[GBUFFER_COUNT] = { nullptr };
	DX::g_deviceContext->OMSetRenderTargets(GBUFFER_COUNT, renderTargets, NULL);
}

void Window::_preparePostLight()
{
	DX::g_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DX::g_deviceContext->IASetInputLayout(DX::g_inputLayout);
	DX::g_deviceContext->OMSetRenderTargets(1, &m_backBufferRTV, m_depthStencilView);
}

void Window::_lightPass(Camera& cam /*std::vector<Light*> lightQueue*/)
{
	DX::g_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	DX::g_deviceContext->VSSetShader(m_deferredVertexShader, nullptr, 0);
	DX::g_deviceContext->HSSetShader(nullptr, nullptr, 0);
	DX::g_deviceContext->DSSetShader(nullptr, nullptr, 0);
	DX::g_deviceContext->GSSetShader(nullptr, nullptr, 0);
	DX::g_deviceContext->PSSetShader(m_deferredPixelShader, nullptr, 0);
	DX::g_deviceContext->OMSetRenderTargets(1, &m_backBufferRTV, nullptr);

	int adress = 0;
	for (auto &srv : m_gbuffer)
	{
		DX::g_deviceContext->PSSetShaderResources(adress++, 1, &srv.SRV);
	}
	DX::g_deviceContext->PSSetShaderResources(4, 1, &m_shadowDepthTexture);
	DX::g_deviceContext->PSSetConstantBuffers(9, 1, &m_shadowBuffer);
	
	//Send lights to GPU
	POINT_LIGHT_COLLECTION pointLightCollectionBuffer; 
	int nrOfLights = static_cast<int>(DX::g_lightQueue.size());

	for (int i = 0; i < nrOfLights; i++)
	{
		if (DX::g_lightQueue[i] != nullptr)
		{
			pointLightCollectionBuffer.positionArray[i] = DX::g_lightQueue[i]->getPosition();
			pointLightCollectionBuffer.colorArray[i] = DX::g_lightQueue[i]->getColor();
			pointLightCollectionBuffer.colorArray[i].w = static_cast<float>(DX::g_lightQueue[i]->getIndex());
			pointLightCollectionBuffer.lightSetup[i] = DX::g_lightQueue[i]->getLightSetup();
		}
	}
	pointLightCollectionBuffer.nrOfLights = XMFLOAT4A(static_cast<float>(nrOfLights), static_cast<float>(nrOfLights), static_cast<float>(nrOfLights), static_cast<float>(nrOfLights));

	D3D11_MAPPED_SUBRESOURCE lightData;
	DX::g_deviceContext->Map(m_pPointLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &lightData);
	memcpy(lightData.pData, &pointLightCollectionBuffer, sizeof(POINT_LIGHT_COLLECTION));
	DX::g_deviceContext->Unmap(m_pPointLightBuffer, 0);
	DX::g_deviceContext->PSSetConstantBuffers(6, 1, &m_pPointLightBuffer);
	
	//Throw in camera values into buffer
	CAMERA_POS_BUFFER cameraBuffer;
	cameraBuffer.pos.x = cam.getPosition().x;
	cameraBuffer.pos.y = cam.getPosition().y;
	cameraBuffer.pos.z = cam.getPosition().z;

	//Update camera buffer values in the GPU
	D3D11_MAPPED_SUBRESOURCE camData;
	DX::g_deviceContext->Map(m_cameraPosConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &camData);
	memcpy(camData.pData, &cameraBuffer, sizeof(CAMERA_POS_BUFFER));
	DX::g_deviceContext->Unmap(m_cameraPosConstantBuffer, 0);
	DX::g_deviceContext->PSSetConstantBuffers(1, 1, &m_cameraPosConstantBuffer); 

	DX::g_deviceContext->IASetInputLayout(nullptr);

	DX::g_deviceContext->Draw(4, 0);
}

void Window::_transparencyPass(const Camera & cam)
{
	
	DX::g_deviceContext->OMSetBlendState(m_transBlendState, 0, 0xffffffff);

	DX::g_deviceContext->VSSetShader(m_transVertexShader, nullptr, 0);
	DX::g_deviceContext->HSSetShader(nullptr, nullptr, 0);
	DX::g_deviceContext->DSSetShader(nullptr, nullptr, 0);
	DX::g_deviceContext->GSSetShader(nullptr, nullptr, 0);
	DX::g_deviceContext->PSSetShader(m_transPixelShader, nullptr, 0);

	DirectX::XMMATRIX view = cam.getViewMatrix();
	DirectX::XMMATRIX viewProj = view * m_projectionMatrix;

	MESH_BUFFER meshBuffer;


	ID3D11Buffer* instanceBuffer = nullptr;

	for (auto& instance : DX::g_instanceGroupsTransparancy)
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
		DirectX::XMStoreFloat4x4A(&meshBuffer.VP, vp);

		D3D11_MAPPED_SUBRESOURCE dataPtr;
		DX::g_deviceContext->Map(m_meshConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);
		memcpy(dataPtr.pData, &meshBuffer, sizeof(MESH_BUFFER));
		DX::g_deviceContext->Unmap(m_meshConstantBuffer, 0);
		DX::g_deviceContext->VSSetConstantBuffers(0, 1, &m_meshConstantBuffer);

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
		instanceBuffer->Release();
	}
}

void Window::_initTransparency()
{
	D3D11_BLEND_DESC omDesc;
	ZeroMemory(&omDesc,

		sizeof(D3D11_BLEND_DESC));
	omDesc.RenderTarget[0].BlendEnable = true;
	omDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	omDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	omDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	omDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	omDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	omDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	omDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;


	DX::g_device->CreateBlendState(&omDesc, &m_transBlendState);
}

Window::Window(HINSTANCE h)
{
	m_hInstance = h;
	m_hwnd	= nullptr;
	m_width	= 0;
	m_height = 0;
	m_title	= "";
	m_sampleCount = 1;
	m_fullscreen = FALSE;
	DX::g_device = nullptr;
	DX::g_deviceContext	 = nullptr;
	m_swapChain = nullptr;
	m_backBufferRTV	 = nullptr;
	m_depthStencilView = nullptr;
	m_depthBufferTex = nullptr;
	m_samplerState = nullptr;
	Input::Instance();
}

Window::~Window()
{
	m_backBufferRTV->Release();
	m_swapChain->Release();

	m_depthStencilView->Release();
	m_depthBufferTex->Release();

	//m_projectionMatrix->Release();

	m_samplerState->Release();

	m_meshConstantBuffer->Release();
	//m_pointLightsConstantBuffer->Release();
	if (m_pointLightsConstantBuffer != nullptr)
	{
		m_pointLightsConstantBuffer->Release();
	}
	m_cameraPosConstantBuffer->Release();
	if (m_lightBuffer != nullptr)
	{
		m_lightBuffer->Release();
	}

	for (size_t i = 0; i < GBUFFER_COUNT; i++)
	{
		m_gbuffer[i].SRV->Release();
		m_gbuffer[i].RTV->Release();
		m_gbuffer[i].TextureMap->Release();
	}
	m_deferredVertexShader->Release();
	m_deferredPixelShader->Release();
	m_transVertexShader->Release();
	m_transPixelShader->Release();
	m_transBlendState->Release();

	if(m_pickingTexture.SRV) m_pickingTexture.SRV->Release();
	if(m_pickingTexture.RTV) m_pickingTexture.RTV->Release();
	if(m_pickingTexture.TextureMap) m_pickingTexture.TextureMap->Release();

	m_pickingVertexShader->Release();
	m_pickingPixelShader->Release();
	m_pickingBuffer->Release();
	if(m_pickingReadBuffer) m_pickingReadBuffer->Release();

	m_computeConstantBuffer->Release();
	m_computeOutputBuffer->Release();
	m_computeReadWriteBuffer->Release();
	m_computeUAV->Release();
	m_computeShader->Release();

	DX::CleanUp();
	
	//This is for leaking, I have no idea
	ID3D11Debug* DebugDevice = nullptr;
	HRESULT Result = DX::g_device->QueryInterface(__uuidof(ID3D11Debug), (void**)&DebugDevice);
	Result = DebugDevice->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	DX::g_device->Release();
	DX::g_device->Release();
	
	
}

bool Window::Init(int width, int height, LPCSTR title, BOOL fullscreen, const bool working)
{
	m_width = width;
	m_height = height;
	Input::m_windowSize = DirectX::XMINT2(m_width, m_height);
	m_title = title;
	m_fullscreen = fullscreen;
	_initWindow();
	if (working == false)
	{
		MessageBoxA(m_hwnd, "NO SETTINGS FILE WAS WOUND", "NO SETTINGS FILE FOUND", MB_ICONWARNING);
	}
	HRESULT hr = _initDirect3DContext();
	_initFonts();
	_initViewPort();
	_setViewport();
	
	std::thread t1(&Window::_compileShaders, this); //_compileShaders();
	std::thread t2(&Window::_initGBuffer, this);	//_initGBuffer();
	_createConstantBuffers(); 
	Picking::InitPickingTexture(width, height, m_sampleCount, m_pickingTexture.TextureMap, m_pickingTexture.RTV, m_pickingTexture.SRV, m_pickingReadBuffer);
	_setSamplerState();
	m_HUDviewProj = DirectX::XMMatrixLookToLH(
		DirectX::XMVectorSet(0.0f, 0.0f, -0.1f, 1.0f),
		DirectX::XMVectorSet(0, 0, 1.0f, 0),
		DirectX::XMVectorSet(0, 1.0f, 0, 0)
	);
	_initTransparency();

	_initComputeShader();

	m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45), static_cast<float>(m_width) / m_height, 0.1f, 200.0f); 
	m_HUDviewProj = m_HUDviewProj * m_projectionMatrix;
	t1.join();
	t2.join();
	ShowWindow(m_hwnd, 10);
	return true;
}

void Window::PollEvents()
{
	if (PeekMessage(&m_msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&m_msg);
		DispatchMessage(&m_msg);
	}
}

bool Window::isOpen()
{
	return WM_QUIT != m_msg.message;
}

void Window::Clear()
{
	float c[4] = { 0.0f,0.0f,0.0f,1.0f };
	DX::g_pickingQueue.clear();
	DX::g_skyBoxQueue.clear();
	DX::g_instanceGroups.clear();
	DX::g_instanceGroupsSkyBox.clear();
	DX::g_instanceGroupsHUD.clear();
	DX::g_instanceGroupsTransparancy.clear();
	DX::g_instanceGroupsPicking.clear();
	DX::g_InstanceGroupsShadow.clear();
	DX::g_textQueue.clear();
	DX::g_deviceContext->ClearRenderTargetView(m_backBufferRTV, c);
	DX::g_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	DX::g_deviceContext->ClearDepthStencilView(m_depthStencilViewShad, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);
	DX::g_deviceContext->OMSetBlendState(nullptr, 0, 0xffffffff);
	//DX::g_lightsPos.clear();
	//DX::g_lightsDir.clear();
	for (int i = 0; i < GBUFFER_COUNT; i++)
	{
		DX::g_deviceContext->ClearRenderTargetView(m_gbuffer[i].RTV, c);
	}

	ID3D11ShaderResourceView* renderTargets[GBUFFER_COUNT] = { nullptr };
	DX::g_deviceContext->PSSetShaderResources(0, GBUFFER_COUNT, renderTargets);
}

void Window::loadActiveLights(GameTime& gameTime)
{

}

void Window::Flush(Camera* c)
{
	//ReportLiveObjects();
	_prepareShadow();
	_shadowPass(c);
	if (DEBUG == 1)
	{
		if (Input::isKeyPressed('O'))
		{
			m_WireFrameDebug = true;
		}
		else if (Input::isKeyPressed('I'))
		{
			m_WireFrameDebug = false;
		}
	}
	

	_prepareGeometryPass();
	_geometryPass(*c);
	_clearTargets();
	_lightPass(*c);

	_preparePostLight();
	_transparencyPass(*c);

	if(DX::g_instanceGroupsSkyBox.size() > 0) 
		_skyBoxPass(*c);
	if(DX::g_instanceGroupsHUD.size() > 0)
		_drawHUD();
	if(m_computeShader != nullptr)
		_runComputeShader();
	if(DX::g_textQueue.size() > 0)
		_drawText();
}

void Window::FullReset()
{
	DX::g_deviceContext->ClearState();
	_setViewport();
	DX::g_deviceContext->PSSetSamplers(0, 1, &m_samplerState);
}

Shape * Window::getPicked(Camera* c)
{
	return Picking::getPicked(c, m_pickingTexture.RTV, m_depthStencilView, m_projectionMatrix, DirectX::XMMatrixIdentity(), m_pickingBuffer, m_pickingVertexShader, m_pickingPixelShader, m_pickingTexture.TextureMap, m_pickingReadBuffer, m_meshConstantBuffer, m_computeConstantBuffer);
}

void Window::Present()
{
	m_swapChain->Present(0, 0);
}

LRESULT Window::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		m_width = LOWORD(lParam);
		m_height = HIWORD(lParam);
		Input::m_windowSize = DirectX::XMINT2(m_width, m_height);

		//	// Release all outstanding references to the swap chain's buffers.
		//	m_backBufferRTV->Release();

		//	HRESULT hr;
		//	// Preserve the existing buffer count and format.
		//	// Automatically choose the width and height to match the client rect for HWNDs.
		//	hr = m_swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

		//	// Perform error handling here!

		//	// Get buffer and create a render-target-view.
		//	ID3D11Texture2D* pBuffer;
		//	hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		//		(void**)&pBuffer);
		//	// Perform error handling here!

		//	hr = DX::g_device->CreateRenderTargetView(pBuffer, NULL,
		//		&m_backBufferRTV);
		//	// Perform error handling here!
		//	pBuffer->Release();
		//	_initGBuffer();
		//	_createDepthBuffer();
		//	DX::g_deviceContext->OMSetRenderTargets(1, &m_backBufferRTV, m_depthStencilView);

		//	// Set up the viewport.
		//	_setViewport();

		//	
		//	
		//}
		break;
	// ----- Keyboard Button -----
	case WM_KEYDOWN:
		
		// --------------------------------Subject for change!--------------------------------
		if (wParam == VK_ESCAPE)
			PostQuitMessage(0);

		Input::m_keys[wParam] = true;
		Input::lastPressed = static_cast<int>(wParam);
		break;
	case WM_KEYUP:
		Input::m_keys[wParam] = false;
		Input::lastPressed = -1;
		break;

	// ----- Left Mouse Button -----
	case WM_LBUTTONDOWN:
		Input::m_mouseKeys[0] = true;
		break;
	case WM_LBUTTONUP:
		Input::m_mouseKeys[0] = false;
		break;

	// ----- Middle Mouse Button -----
	case WM_MBUTTONDOWN:
		Input::m_mouseKeys[1] = true;
		break;
	case WM_MBUTTONUP:
		Input::m_mouseKeys[1] = false;
		break;

	// ----- Right Mouse Button -----
	case WM_RBUTTONDOWN:
		Input::m_mouseKeys[2] = true;
		break;
	case WM_RBUTTONUP:
		Input::m_mouseKeys[2] = false;
		break;

	// ----- Mouse Position -----
	case WM_MOUSEMOVE:
		Input::m_mousePos.x = LOWORD(lParam);
		Input::m_mousePos.y = HIWORD(lParam);
		//SetCursorPos(Input::m_mousePos.x, Input::m_mousePos.y);
		break;

	// ----- Mouse Wheel -----
	case WM_MOUSEWHEEL:
		Input::m_scrollDelta = GET_WHEEL_DELTA_WPARAM(wParam) / 120.0f;
	//	std::cout << GET_WHEEL_DELTA_WPARAM(wParam) << std::endl;
		break;
	}

	return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}

LRESULT Window::StaticWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Window* pParent;
	if (uMsg == WM_CREATE)
	{
		pParent = (Window*)((LPCREATESTRUCT)lParam)->lpCreateParams;
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pParent);
	}
	else
	{
		pParent = (Window*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		if (!pParent) return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	pParent->m_hwnd = hWnd;
	return pParent->WndProc(uMsg, wParam, lParam);
}

void Window::setMouseMiddleScreen()
{
	POINT pt = { (LONG)m_width / 2, (LONG)m_height / 2 };
	ClientToScreen(m_hwnd, &pt);
	SetCursorPos(pt.x, pt.y);
}

void Window::setTitle(LPCSTR string)
{
	SetWindowText(m_hwnd, string);
}

DirectX::XMFLOAT2 Window::getSize() const
{
	DirectX::XMFLOAT2 sizeVec;
	sizeVec.x = (float)m_width;
	sizeVec.y = (float)m_height;
	return sizeVec;
}

DirectX::XMFLOAT2 Window::getMousePos()
{
	return m_mousePos;
}
