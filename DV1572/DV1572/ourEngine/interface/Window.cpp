﻿#include "Window.h"
#include "../core/Dx.h"
#include <thread>
#include "../core/Picking.h"
#include <chrono>
#include <iostream>
#include <algorithm> // For std::find_if
#include "../interface/shape/Billboard.h"
#define DEBUG 1

#pragma comment(lib, "winmm")
//Devices
ID3D11Device* DX::g_device;
ID3D11DeviceContext* DX::g_deviceContext;

//Standard Shaders and Input
ID3D11VertexShader* DX::g_3DVertexShader;
#if DEFERRED_RENDERING
ID3D11PixelShader* DX::g_3DPixelShader;
ID3D11PixelShader* DX::g_billboardPixelShader;
#elif FORWARD_RENDERING
ID3D11PixelShader* DX::g_forwardPixelShader;
#endif
ID3D11InputLayout* DX::g_inputLayout;
ID3D11InputLayout* DX::g_billInputLayout;

ID3D11VertexShader* DX::g_billboardVertexShader;

//Rendering Queues
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

std::deque<DX::INSTANCE_GROUP>				DX::g_instanceGroups;
std::deque<DX::INSTANCE_GROUP>				DX::g_instanceGroupsSkyBox;
std::deque<DX::INSTANCE_GROUP>				DX::g_instanceGroupsHUD;
std::deque<DX::INSTANCE_GROUP>				DX::g_instanceGroupsTransparancy;
std::vector<DX::INSTANCE_GROUP_INDEXED>		DX::g_instanceGroupsPicking;
std::deque<DX::INSTANCE_GROUP>				DX::g_InstanceGroupsShadow;
std::deque<DX::INSTANCE_GROUP_BILL>				DX::g_instanceGroupsBillboard;
std::deque<DX::INSTANCE_GROUP>				DX::g_instanceGroupWindows;

//TEXT
std::vector<std::unique_ptr<DirectX::SpriteFont>>	DX::g_fonts;
std::vector<Text*>									DX::g_textQueue;
std::unique_ptr<DirectX::SpriteBatch>				DX::g_spriteBatch;

XMFLOAT4A											DX::g_lightPos;
XMFLOAT4A											DX::g_lightDir;

void DX::submitToInstance(Shape* shape, std::deque<DX::INSTANCE_GROUP>& queue)
{
	auto exisitingEntry = std::find_if(queue.begin(), queue.end(), [&](const INSTANCE_GROUP& item) {
		return shape->GetMesh()->CheckID(*item.shape->GetMesh());
	});

	//Converting The worldMatrix into a instanced world matrix.
	//This allowes us to send in the matrix in the layout and now a constBuffer
	INSTANCE_ATTRIB attribDesc;

	XMMATRIX xmWorldMat = shape->getWorld();
	XMFLOAT4X4A worldMat;

	XMStoreFloat4x4A(&worldMat, xmWorldMat);
	ZeroMemory(&attribDesc, sizeof(INSTANCE_ATTRIB));
	memcpy(&attribDesc.u.rows,&worldMat.m[0][0], 16 * sizeof(float));
	
	attribDesc.highLightColor = shape->getColor(); //This allowes us to use a "click highlight"
	attribDesc.lightIndex = static_cast<float>(shape->getLightIndex());
	attribDesc.gridScaleX = static_cast<float>(shape->getGridScaleX());
	attribDesc.gridScaleY = static_cast<float>(shape->getGridScaleY());
	// Unique Mesh
	if (exisitingEntry == queue.end())
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
	
		exisitingEntry->attribs.push_back(attribDesc);
		
	}
	
}

void DX::submitToInstance(Shape* shape, std::vector<DX::INSTANCE_GROUP_INDEXED>& queue)
{
	auto exisitingEntry = std::find_if(queue.begin(), queue.end(), [&](const INSTANCE_GROUP_INDEXED& item) {
			return shape->GetMesh()->CheckID(*item.shape->GetMesh());
	});


	//Converting The worldMatrix into a instanced world matrix.
	//This allowes us to send in the matrix in the layout and now a constBuffer
	INSTANCE_ATTRIB attribDesc;

	XMMATRIX xmWorldMat = shape->getWorld();
	XMFLOAT4X4A worldMat;
	long index =static_cast<long>(DX::g_pickingQueue.size() - 1);

	XMStoreFloat4x4A(&worldMat, xmWorldMat);

	memcpy(&attribDesc.u.rows, &worldMat.m[0][0], 16 * sizeof(float));

	attribDesc.highLightColor = shape->getColor(); //This allowes us to use a "click highlight"


												   // Unique Mesh
	if (exisitingEntry == queue.end())
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
		exisitingEntry->attribs.push_back(attribDesc);
		exisitingEntry->index.push_back(index);
	}

}

void DX::submitToInstance(Character * character)
{
	auto exisitingEntry = std::find_if(DX::g_instanceGroupsBillboard.begin(), DX::g_instanceGroupsBillboard.end(), [&](const INSTANCE_GROUP_BILL& item) {
		return character->getShape()->GetMesh()->CheckID(*item.shape->GetMesh());
	});

	//Converting The worldMatrix into a instanced world matrix.
	//This allowes us to send in the matrix in the layout and now a constBuffer
	INSTANCE_ATTRIB_BILL attribDesc;
	
	XMMATRIX xmWorldMat = character->getShape()->getWorld();
	XMFLOAT4X4A worldMat;

	XMStoreFloat4x4A(&worldMat, xmWorldMat);

	XMFLOAT4A rows;

	rows.x = worldMat.m[3][0];
	rows.y = worldMat.m[3][1];
	rows.z = worldMat.m[3][2];
	rows.w = worldMat.m[3][3];

	attribDesc.w4 = rows;

	attribDesc.highLightColor = character->getShape()->getColor(); //This allowes us to use a "click highlight"
	attribDesc.lightIndex = static_cast<float>(character->getShape()->getLightIndex());
	XMFLOAT3 tempPos = character->getDirection3f();
	
	XMFLOAT4A chararcterDireciton = { tempPos.x,tempPos.y,tempPos.z,1.0f };
	attribDesc.charDir = chararcterDireciton;
	attribDesc.spriteIndex = static_cast<float>(character->getModelSpriteIndex());

	// Unique Mesh
	if (exisitingEntry == DX::g_instanceGroupsBillboard.end())
	{
		//If the queue dose not exist we create a new queue.
		//This is what allows the instancing to work
		INSTANCE_GROUP_BILL newGroup;
		newGroup.attribs.push_back(attribDesc);
		newGroup.shape = character->getShape();
		DX::g_instanceGroupsBillboard.push_back(newGroup);
	}
	else
	{
		//If the mesh allready exists we just push it into a exsiting queue
		exisitingEntry->attribs.push_back(attribDesc);
	}
}
void DX::submitToInstance(Billboard* bill)
{
	auto exisitingEntry = std::find_if(DX::g_instanceGroupsBillboard.begin(), DX::g_instanceGroupsBillboard.end(), [&](const INSTANCE_GROUP_BILL& item) {
		return bill->GetMesh()->CheckID(*item.shape->GetMesh());
	});


	//Converting The worldMatrix into a instanced world matrix.
	//This allowes us to send in the matrix in the layout and now a constBuffer
	INSTANCE_ATTRIB_BILL attribDesc;

	XMFLOAT3 bPos = bill->getPosition();
	XMFLOAT4A rows = { bPos.x, bPos.y, bPos.z,1.0f };

	attribDesc.w4 = rows;

	attribDesc.highLightColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	attribDesc.lightIndex = 0.0f;
	
	XMFLOAT4A chararcterDireciton = { -1, -1, -1, -1 };
	attribDesc.charDir = chararcterDireciton;
	attribDesc.spriteIndex = static_cast<float>(bill->getSpriteIndex());

	// Unique Mesh
	if (exisitingEntry == DX::g_instanceGroupsBillboard.end())
	{
		//If the queue dose not exist we create a new queue.
		//This is what allows the instancing to work
		INSTANCE_GROUP_BILL newGroup;
		newGroup.attribs.push_back(attribDesc);
		newGroup.shape = bill;
		DX::g_instanceGroupsBillboard.push_back(newGroup);
	}
	else
	{
		//If the mesh allready exists we just push it into a exsiting queue
		exisitingEntry->attribs.push_back(attribDesc);
	}
}
void DX::CleanUp()
{
	DX::SafeRelease(&DX::g_3DVertexShader);
#if DEFERRED_RENDERING
	DX::SafeRelease(&DX::g_3DPixelShader);
	DX::SafeRelease(&DX::g_billboardPixelShader);
#endif
	DX::SafeRelease(&DX::g_inputLayout);
	DX::SafeRelease(&DX::g_forwardPixelShader);
	DX::SafeRelease(&DX::g_billInputLayout);
	DX::SafeRelease(&DX::g_standardHullShader);
	DX::SafeRelease(&DX::g_standardDomainShader);
	DX::SafeRelease(&DX::g_skyBoxVertexShader);
	DX::SafeRelease(&DX::g_skyBoxPixelShader);
	DX::SafeRelease(&DX::g_inputLayout);
	DX::SafeRelease(&DX::g_standardHullShader);
	DX::SafeRelease(&DX::g_standardDomainShader);
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
		_loadWindowBuffers();
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
		{ "LIGHTINDEX", 0, DXGI_FORMAT_R32_FLOAT, 1, 80, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "GRIDSCALEX", 0, DXGI_FORMAT_R32_FLOAT, 1, 84, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "GRIDSCALEY", 0, DXGI_FORMAT_R32_FLOAT, 1, 88, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
	};
	
	// Billboarding

	D3D11_INPUT_ELEMENT_DESC inputDescBill[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXELS", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		// INSTANCE ATTRIBUTES
		{ "INSTANCEWORLDFOUR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		//This is the attribute that allows the color change without constant buffer
		{ "HIGHLIGHTCOLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "CHARDIR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "SPRITEINDEX", 0, DXGI_FORMAT_R32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "LIGHTINDEX", 0, DXGI_FORMAT_R32_FLOAT, 1, 52, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};

	ShaderCreator::CreateVertexShader(DX::g_device, DX::g_billboardVertexShader, 
		L"ourEngine/shaders/billboardVertex.hlsl", "main",
		inputDescBill, ARRAYSIZE(inputDescBill), DX::g_billInputLayout);
#if DEFERRED_RENDERING
	ShaderCreator::CreatePixelShader(DX::g_device, DX::g_billboardPixelShader,
		L"ourEngine/shaders/billboardPixel.hlsl", "main");
	ShaderCreator::CreatePixelShader(DX::g_device, DX::g_3DPixelShader,
		L"ourEngine/shaders/3DPixel.hlsl", "main");
	ShaderCreator::CreateVertexShader(DX::g_device, m_deferredVertexShader,
		L"ourEngine/Shaders/deferredVertexShader.hlsl", "main");
	ShaderCreator::CreatePixelShader(DX::g_device, m_deferredPixelShader,
		L"ourEngine/shaders/deferredPixelShader.hlsl", "main");
#elif FORWARD_RENDERING
	ShaderCreator::CreatePixelShader(DX::g_device, DX::g_forwardPixelShader,
		L"ourEngine/shaders/ForwardPixel.hlsl", "main");
#endif
	// Billboarding end
	ShaderCreator::CreateVertexShader(DX::g_device, DX::g_3DVertexShader,
		L"ourEngine/shaders/3DVertex.hlsl", "main",
		inputDesc, ARRAYSIZE(inputDesc), DX::g_inputLayout);

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

	while (!DX::g_instanceGroupsHUD.empty())	//Every instance has it's own queue
	{
		auto instance = DX::g_instanceGroupsHUD.front();
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

		
		UINT offset = 0;
		ID3D11Buffer* v = instance.shape->GetMesh()->getVertices();
		ID3D11Buffer * bufferPointers[2];
		bufferPointers[0] = v;
		bufferPointers[1] = instanceBuffer;

		unsigned int strides[2];
		strides[0] = sizeof(VERTEX);
		strides[1] = sizeof(DX::INSTANCE_ATTRIB);

		unsigned int offsets[2];
		offsets[0] = 0;
		offsets[1] = 0;


		Mesh* mesh = instance.shape->GetMesh();
		ID3D11Buffer* indices = mesh->getIndicesBuffer();

		DX::g_deviceContext->IASetIndexBuffer(indices, DXGI_FORMAT_R32_UINT, offset);
		DX::g_deviceContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);

		DX::g_deviceContext->DrawIndexedInstanced(instance.shape->GetMesh()->getNrOfIndices(), (UINT)instance.attribs.size(), 0, 0, 0);
		instanceBuffer->Release();
		DX::g_instanceGroupsHUD.pop_front();
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
	texDesc.Width = 2048;
	texDesc.Height = 2048;
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
	m_shadowProjMatrix = XMMatrixOrthographicLH(10, 10, 1.0f, 50.0f);
}

void Window::_prepareShadow()
{

	DX::g_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DX::g_deviceContext->IASetInputLayout(DX::g_inputLayout);
	DX::g_deviceContext->VSSetShader(m_shadowVertex, nullptr, 0);
	DX::g_deviceContext->HSSetShader(nullptr, nullptr, 0);
	DX::g_deviceContext->DSSetShader(nullptr, nullptr, 0);
	DX::g_deviceContext->GSSetShader(nullptr, nullptr, 0);
	DX::g_deviceContext->PSSetShader(nullptr, nullptr, 0);
	DX::g_deviceContext->OMSetRenderTargets(0, nullptr, m_depthStencilViewShad);

	D3D11_RASTERIZER_DESC wfdesc;
	ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
	wfdesc.FillMode = D3D11_FILL_SOLID;
	wfdesc.CullMode = D3D11_CULL_BACK;
	wfdesc.DepthClipEnable = true;
	DX::SafeRelease(&m_WireFrame);
	DX::g_device->CreateRasterizerState(&wfdesc, &m_WireFrame);
	DX::g_deviceContext->RSSetState(m_WireFrame);

	m_viewport.Height = 2048;
	m_viewport.Width = 2048;
	m_viewport.MinDepth = 0.f;
	m_viewport.MaxDepth = 1.f;
	
	DX::g_deviceContext->RSSetViewports(1, &m_viewport);
}

void Window::_shadowPass(Camera* c)
{

	static XMMATRIX lightPerspectiveMatrix = XMMatrixOrthographicLH(50, 50, 1.0f, 200.0f);

	const XMVECTORF32 eye = { DX::g_lightPos.x, DX::g_lightPos.y, DX::g_lightPos.z, 0.0f };
	static const XMVECTORF32 at = { 16.0f, 0.0f, 16.0f, 0.0f };
	static const XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

	SHADOW_MATRIX_BUFFER meshBuffer;

	XMStoreFloat4x4A(&meshBuffer.view, XMMatrixTranspose(XMMatrixLookAtLH(eye, at, up)));
	XMStoreFloat4x4A(&meshBuffer.projection, XMMatrixTranspose(lightPerspectiveMatrix));


	D3D11_MAPPED_SUBRESOURCE dataPtr;
	DX::g_deviceContext->Map(m_shadowBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);
	memcpy(dataPtr.pData, &meshBuffer, sizeof(SHADOW_MATRIX_BUFFER));
	DX::g_deviceContext->Unmap(m_shadowBuffer, 0);

	DX::g_deviceContext->VSSetConstantBuffers(9, 1, &m_shadowBuffer);

	ID3D11Buffer* instanceBuffer = nullptr;
	while (!DX::g_InstanceGroupsShadow.empty())
	{
		auto instance = DX::g_InstanceGroupsShadow.front();
		D3D11_BUFFER_DESC instBuffDesc;
		memset(&instBuffDesc, 0, sizeof(instBuffDesc));
		instBuffDesc.Usage = D3D11_USAGE_DEFAULT;
		instBuffDesc.ByteWidth = sizeof(DX::INSTANCE_ATTRIB) * (UINT)instance.attribs.size();
		instBuffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA instData;
		memset(&instData, 0, sizeof(instData));
		instData.pSysMem = &instance.attribs[0];
		HRESULT hr = DX::g_device->CreateBuffer(&instBuffDesc, &instData, &instanceBuffer);

		UINT32 vertexSize = sizeof(VERTEX);
		UINT offset = 0;
		ID3D11Buffer* v = instance.shape->GetMesh()->getVertices();
		ID3D11Buffer * bufferPointers[2];
		bufferPointers[0] = v;
		bufferPointers[1] = instanceBuffer;

		unsigned int strides[2];
		strides[0] = sizeof(VERTEX);
		strides[1] = sizeof(DX::INSTANCE_ATTRIB);

		unsigned int offsets[2];
		offsets[0] = 0;
		offsets[1] = 0;

		Mesh* mesh = instance.shape->GetMesh();
		ID3D11Buffer* indices = mesh->getIndicesBuffer();

		DX::g_deviceContext->IASetIndexBuffer(indices, DXGI_FORMAT_R32_UINT, offset);
		DX::g_deviceContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);

		DX::g_deviceContext->DrawIndexedInstanced(instance.shape->GetMesh()->getNrOfIndices(), (UINT)instance.attribs.size(), 0, 0, 0);
		DX::SafeRelease(&instanceBuffer);
		DX::g_InstanceGroupsShadow.pop_front();
	}

	
}

void Window::_loadWindowBuffers()
{
	/*D3D11_BUFFER_DESC bDesc;
	bDesc.Usage = D3D11_USAGE_DYNAMIC;
	bDesc.ByteWidth = sizeof(SHADOW_MATRIX_BUFFER);
	bDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bDesc.MiscFlags = 0;
	bDesc.StructureByteStride = 0;

	HRESULT hr = DX::g_device->CreateBuffer(&bDesc, nullptr, &m_windowsBuffer);

	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = 2048;
	texDesc.Height = 2048;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	hr = DX::g_device->CreateTexture2D(&texDesc, NULL, &m_depthBufferTexWindow);

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Flags = 0;
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	hr = DX::g_device->CreateDepthStencilView(m_depthBufferTexWindow, &dsvDesc, &m_depthStencilViewWindow);


	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;

	hr = DX::g_device->CreateShaderResourceView(m_depthBufferTexWindow, &srvDesc, &m_windowDepthTexture);*/
}

void Window::_windowPass(Camera * c)
{
	/*DX::g_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DX::g_deviceContext->IASetInputLayout(DX::g_inputLayout);
	DX::g_deviceContext->VSSetShader(m_shadowVertex, nullptr, 0);
	DX::g_deviceContext->HSSetShader(nullptr, nullptr, 0);
	DX::g_deviceContext->DSSetShader(nullptr, nullptr, 0);
	DX::g_deviceContext->GSSetShader(nullptr, nullptr, 0);
	DX::g_deviceContext->PSSetShader(nullptr, nullptr, 0);
	DX::g_deviceContext->OMSetRenderTargets(0, nullptr, m_depthStencilViewWindow);

	D3D11_RASTERIZER_DESC wfdesc;
	ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
	wfdesc.FillMode = D3D11_FILL_SOLID;
	wfdesc.CullMode = D3D11_CULL_BACK;
	wfdesc.DepthClipEnable = true;
	DX::g_device->CreateRasterizerState(&wfdesc, &m_WireFrame);
	DX::g_deviceContext->RSSetState(m_WireFrame);

	m_viewport.Height = 2048;
	m_viewport.Width = 2048;
	m_viewport.MinDepth = 0.f;
	m_viewport.MaxDepth = 1.f;

	DX::g_deviceContext->RSSetViewports(1, &m_viewport);

	static XMMATRIX lightPerspectiveMatrix = XMMatrixOrthographicLH(50, 50, 1.0f, 200.0f);

	const XMVECTORF32 eye = { DX::g_lightPos.x, DX::g_lightPos.y, DX::g_lightPos.z, 0.0f };
	static const XMVECTORF32 at = { 16.0f, 0.0f, 16.0f, 0.0f };
	static const XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

	SHADOW_MATRIX_BUFFER meshBuffer;

	XMStoreFloat4x4A(&meshBuffer.view, XMMatrixTranspose(XMMatrixLookAtLH(eye, at, up)));
	XMStoreFloat4x4A(&meshBuffer.projection, XMMatrixTranspose(lightPerspectiveMatrix));


	D3D11_MAPPED_SUBRESOURCE dataPtr;
	DX::g_deviceContext->Map(m_windowsBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);
	memcpy(dataPtr.pData, &meshBuffer, sizeof(SHADOW_MATRIX_BUFFER));
	DX::g_deviceContext->Unmap(m_windowsBuffer, 0);

	DX::g_deviceContext->VSSetConstantBuffers(9, 1, &m_windowsBuffer);

	ID3D11Buffer* instanceBuffer = nullptr;
	for (auto& instance : DX::g_instanceGroupWindows)
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

		UINT offset = 0;
		ID3D11Buffer* v = instance.shape->GetMesh()->getVertices();
		ID3D11Buffer * bufferPointers[2];
		bufferPointers[0] = v;
		bufferPointers[1] = instanceBuffer;

		unsigned int strides[2];
		strides[0] = sizeof(VERTEX);
		strides[1] = sizeof(DX::INSTANCE_ATTRIB);

		unsigned int offsets[2];
		offsets[0] = 0;
		offsets[1] = 0;

		Mesh* mesh = instance.shape->GetMesh();
		ID3D11Buffer* indices = mesh->getIndicesBuffer();

		DX::g_deviceContext->IASetIndexBuffer(indices, DXGI_FORMAT_R32_UINT, offset);
		DX::g_deviceContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);

		DX::g_deviceContext->DrawIndexedInstanced(instance.shape->GetMesh()->getNrOfIndices(), (UINT)instance.attribs.size(), 0, 0, 0);
		instanceBuffer->Release();
	}
*/

}

void Window::_initFonts()
{
	DX::g_fonts.push_back(std::make_unique<SpriteFont>(DX::g_device, L"trolls_inn/Resources/Fonts/consolas32.spritefont"));
	DX::g_fonts.push_back(std::make_unique<SpriteFont>(DX::g_device, L"trolls_inn/Resources/Fonts/constantia32.spritefont"));
	DX::g_fonts.push_back(std::make_unique<SpriteFont>(DX::g_device, L"trolls_inn/Resources/Fonts/constantia50.spritefont"));
	DX::g_fonts.push_back(std::make_unique<SpriteFont>(DX::g_device, L"trolls_inn/Resources/Fonts/arialBlack32.spritefont"));
	DX::g_spriteBatch = std::make_unique<SpriteBatch>(DX::g_deviceContext);
}

void Window::_drawText()
{
	DX::g_spriteBatch->Begin(SpriteSortMode_Immediate, m_transBlendState);
	//DX::g_spriteBatch->Begin();
	//DX::g_deviceContext->OMSetBlendState(m_transBlendState, 0, 0xffffffff);
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
	

	D3D11_SAMPLER_DESC samplerDescPoint;
	samplerDescPoint.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDescPoint.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDescPoint.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDescPoint.BorderColor[0] = 1.0f;
	samplerDescPoint.BorderColor[1] = 1.0f;
	samplerDescPoint.BorderColor[2] = 1.0f;
	samplerDescPoint.BorderColor[3] = 1.0f;
	samplerDescPoint.MinLOD = 0.f;
	samplerDescPoint.MaxLOD = D3D11_FLOAT32_MAX;
	samplerDescPoint.MipLODBias = 0.f;
	samplerDescPoint.MaxAnisotropy = 0;
	samplerDescPoint.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	samplerDescPoint.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;

	DX::g_device->CreateSamplerState(&samplerDescPoint, &m_samplerStatePoint);

}

void Window::_createConstantBuffers()
{
	_createBillboardConstantBuffer();
	_createMeshConstantBuffer();
	_createPickConstantBuffer();
#if DEFERRED_RENDERING
	_createCameraPosConstantBuffer();
	_createPointLightCollectionBuffer(); 
#elif FORWARD_RENDERING
	_createEverythingConstantBuffer();
#endif
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

void Window::_createBillboardConstantBuffer()
{
	D3D11_BUFFER_DESC bDesc;
	bDesc.Usage = D3D11_USAGE_DYNAMIC;
	bDesc.ByteWidth = sizeof(BILLBOARD_MESH_BUFFER);
	bDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bDesc.MiscFlags = 0;
	bDesc.StructureByteStride = 0;

	HRESULT hr = DX::g_device->CreateBuffer(&bDesc, nullptr, &m_billboardConstantBuffer);
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
#if DEFERRED_RENDERING
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
#elif FORWARD_RENDERING
void Window::_createEverythingConstantBuffer()
{
	D3D11_BUFFER_DESC bDesc;
	bDesc.Usage = D3D11_USAGE_DYNAMIC;
	bDesc.ByteWidth = sizeof(EVERYTHING_BUFFER);
	bDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bDesc.MiscFlags = 0;
	bDesc.StructureByteStride = 0;

	HRESULT hr = DX::g_device->CreateBuffer(&bDesc, nullptr, &m_everythingConstantBuffer);

	D3D11_BUFFER_DESC sunBdesc;
	sunBdesc.Usage = D3D11_USAGE_DYNAMIC;
	sunBdesc.ByteWidth = sizeof(DIRECTIONAL_LIGHT_BUFFER);
	sunBdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	sunBdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	sunBdesc.MiscFlags = 0;
	sunBdesc.StructureByteStride = 0;

	DX::g_device->CreateBuffer(&sunBdesc, nullptr, &m_pSunBuffer);
}
#endif
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

}

#if DEFERRED_RENDERING
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
#endif

void Window::_prepareGeometryPass()
{
#if DEFERRED_RENDERING
	ID3D11RenderTargetView* renderTargets[GBUFFER_COUNT];

	for (int i = 0; i < GBUFFER_COUNT; i++)
	{
		renderTargets[i] = m_gbuffer[i].RTV;
	}
	DX::g_deviceContext->OMSetRenderTargets(GBUFFER_COUNT, renderTargets, m_depthStencilView);
#endif
	_initViewPort();
	_setViewport();

	DX::g_deviceContext->PSSetSamplers(0, 1, &m_samplerState);
	DX::g_deviceContext->PSSetSamplers(1, 1, &m_samplerStatePoint);
}

void Window::_billboardPass(const Camera & cam)
{
	// No tessellation
	DX::g_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	
	DX::g_deviceContext->IASetInputLayout(DX::g_billInputLayout);

	DirectX::XMMATRIX View = cam.getViewMatrix();
	DirectX::XMMATRIX Proj = m_projectionMatrix;
	View = XMMatrixTranspose(View);
	Proj = XMMatrixTranspose(Proj);

	BILLBOARD_MESH_BUFFER buffer;
	DirectX::XMStoreFloat4x4A(&buffer.View, View);
	DirectX::XMStoreFloat4x4A(&buffer.Projection, Proj);
	DirectX::XMFLOAT3 p = cam.getPosition();
	DirectX::XMFLOAT3 la = cam.getLookAt();
	buffer.CamPos = DirectX::XMFLOAT4A(p.x, p.y, p.z, 1.0f);
	buffer.CamDir = DirectX::XMFLOAT4A(la.x, la.y, la.z, 0.0f);
	

	if (m_WireFrameDebug == true)
	{
		D3D11_RASTERIZER_DESC wfdesc;
		ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
		wfdesc.FillMode = D3D11_FILL_WIREFRAME;
		wfdesc.CullMode = D3D11_CULL_NONE;
		DX::SafeRelease(&m_WireFrame);
		DX::g_device->CreateRasterizerState(&wfdesc, &m_WireFrame);
		DX::g_deviceContext->RSSetState(m_WireFrame);
	}

	ID3D11Buffer* instanceBuffer = nullptr;

	while (!DX::g_instanceGroupsBillboard.empty())
	{
		
		auto instance = DX::g_instanceGroupsBillboard.front();
		D3D11_BUFFER_DESC instBuffDesc;
		memset(&instBuffDesc, 0, sizeof(instBuffDesc));
		instBuffDesc.Usage = D3D11_USAGE_DEFAULT;
		instBuffDesc.ByteWidth = sizeof(DX::INSTANCE_ATTRIB_BILL) * (UINT)instance.attribs.size();
		instBuffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;


		D3D11_SUBRESOURCE_DATA instData;
		memset(&instData, 0, sizeof(instData));
		instData.pSysMem = &instance.attribs[0];
		HRESULT hr = DX::g_device->CreateBuffer(&instBuffDesc, &instData, &instanceBuffer);		

		D3D11_MAPPED_SUBRESOURCE dataPtr;
		DX::g_deviceContext->Map(m_billboardConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);
		memcpy(dataPtr.pData, &buffer, sizeof(BILLBOARD_MESH_BUFFER));
		DX::g_deviceContext->Unmap(m_billboardConstantBuffer, 0);
		DX::g_deviceContext->VSSetConstantBuffers(0, 1, &m_billboardConstantBuffer);

		// Apply shaders
		instance.shape->ApplyShaders(); //ApplyShaders will set the special shaders

		for (int i = 0; i < instance.shape->GetMesh()->getNumberOfParts(); i++)
		{
			instance.shape->ApplyMaterials(i);

			UINT32 vertexSize = sizeof(VERTEX);
			UINT offset = 0;
			ID3D11Buffer* v = instance.shape->GetMesh()->getVertices(i);
			ID3D11Buffer * bufferPointers[2];
			bufferPointers[0] = v;
			bufferPointers[1] = instanceBuffer;

			unsigned int strides[2];
			strides[0] = sizeof(VERTEX);
			strides[1] = sizeof(DX::INSTANCE_ATTRIB_BILL);

			unsigned int offsets[2];
			offsets[0] = 0;
			offsets[1] = 0;

			ID3D11Buffer* indices = instance.shape->GetMesh()->getIndicesBuffer(i);

			DX::g_deviceContext->IASetIndexBuffer(indices, DXGI_FORMAT_R32_UINT, offset);
			DX::g_deviceContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);

			DX::g_deviceContext->DrawIndexedInstanced(instance.shape->GetMesh()->getNrOfIndices(i), (UINT)instance.attribs.size(), 0, 0, 0);
		}

		instanceBuffer->Release();
		DX::g_instanceGroupsBillboard.pop_front();
	}

	if (m_WireFrameDebug == true)
	{
		D3D11_RASTERIZER_DESC wfdesc;
		ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
		wfdesc.FillMode = D3D11_FILL_SOLID;
		wfdesc.CullMode = D3D11_CULL_BACK;
		DX::SafeRelease(&m_WireFrame);
		DX::g_device->CreateRasterizerState(&wfdesc, &m_WireFrame);
		DX::g_deviceContext->RSSetState(m_WireFrame);
	}
}

void Window::_geometryPass(const Camera &cam)
{
	DX::g_deviceContext->IASetInputLayout(DX::g_inputLayout);
	DX::g_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	DirectX::XMMATRIX view = cam.getViewMatrix();			//Getting the view matrix from the camera.
	DirectX::XMMATRIX viewProj = view * m_projectionMatrix;	//The smashing it with projection

	MESH_BUFFER meshBuffer;

	DirectX::XMMATRIX vp = DirectX::XMMatrixTranspose(viewProj);
	DirectX::XMStoreFloat4x4A(&meshBuffer.VP, vp);

	DX::g_deviceContext->DSSetConstantBuffers(0, 1, &m_meshConstantBuffer);
	DX::g_deviceContext->VSSetConstantBuffers(0, 1, &m_meshConstantBuffer);
	
	if (m_WireFrameDebug == true)
	{
		D3D11_RASTERIZER_DESC wfdesc;
		ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
		wfdesc.FillMode = D3D11_FILL_WIREFRAME;
		wfdesc.CullMode = D3D11_CULL_NONE;
		DX::SafeRelease(&m_WireFrame);
		DX::g_device->CreateRasterizerState(&wfdesc, &m_WireFrame);
		DX::g_deviceContext->RSSetState(m_WireFrame);
	}

	ID3D11Buffer* instanceBuffer = nullptr;

	while (!DX::g_instanceGroups.empty())
	{
		auto instance = DX::g_instanceGroups.front();
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
	

		D3D11_MAPPED_SUBRESOURCE dataPtr;
		DX::g_deviceContext->Map(m_meshConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);
		memcpy(dataPtr.pData, &meshBuffer, sizeof(MESH_BUFFER));
		DX::g_deviceContext->Unmap(m_meshConstantBuffer, 0);
		


		// Apply shaders
		instance.shape->ApplyShaders(); //ApplyShaders will set the special shaders

		for (int i = 0; i < instance.shape->GetMesh()->getNumberOfParts(); i++)
		{
			instance.shape->ApplyMaterials(i);

			UINT offset = 0;
			ID3D11Buffer* v = instance.shape->GetMesh()->getVertices(i);
			ID3D11Buffer * bufferPointers[2];
			bufferPointers[0] = v;
			bufferPointers[1] = instanceBuffer;

			unsigned int strides[2];
			strides[0] = sizeof(VERTEX);
			strides[1] = sizeof(DX::INSTANCE_ATTRIB);

			unsigned int offsets[2];
			offsets[0] = 0;
			offsets[1] = 0;

			ID3D11Buffer* indices = instance.shape->GetMesh()->getIndicesBuffer(i);

			DX::g_deviceContext->IASetIndexBuffer(indices, DXGI_FORMAT_R32_UINT, offset);
			DX::g_deviceContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);
																					
			DX::g_deviceContext->DrawIndexedInstanced(instance.shape->GetMesh()->getNrOfIndices(i), (UINT)instance.attribs.size(), 0, 0, 0);
		}
		DX::SafeRelease(&instanceBuffer);
		DX::g_instanceGroups.pop_front();
	}

	if (m_WireFrameDebug == true)
	{
		D3D11_RASTERIZER_DESC wfdesc;
		ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
		wfdesc.FillMode = D3D11_FILL_SOLID;
		wfdesc.CullMode = D3D11_CULL_BACK;
		DX::SafeRelease(&m_WireFrame);
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

	while (!DX::g_instanceGroupsSkyBox.empty())
	{
		auto instance = DX::g_instanceGroupsSkyBox.front();
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

		UINT offset = 0;
		ID3D11Buffer* v = instance.shape->GetMesh()->getVertices();
		ID3D11Buffer * bufferPointers[2];
		bufferPointers[0] = v;
		bufferPointers[1] = instanceBuffer;

		unsigned int strides[2];
		strides[0] = sizeof(VERTEX);
		strides[1] = sizeof(DX::INSTANCE_ATTRIB);

		unsigned int offsets[2];
		offsets[0] = 0;
		offsets[1] = 0;



		Mesh* mesh = instance.shape->GetMesh();
		ID3D11Buffer* indices = mesh->getIndicesBuffer();

		DX::g_deviceContext->IASetIndexBuffer(indices, DXGI_FORMAT_R32_UINT, offset);
		DX::g_deviceContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);

		DX::g_deviceContext->DrawIndexedInstanced(instance.shape->GetMesh()->getNrOfIndices(), (UINT)instance.attribs.size(), 0, 0, 0);
		instanceBuffer->Release();
		DX::g_instanceGroupsSkyBox.pop_front();
	}
}

#if DEFERRED_RENDERING
void Window::_clearTargets()
{
	ID3D11RenderTargetView* renderTargets[GBUFFER_COUNT] = { nullptr };
	DX::g_deviceContext->OMSetRenderTargets(GBUFFER_COUNT, renderTargets, NULL);
}
#endif

void Window::_preparePostLight()
{
	DX::g_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DX::g_deviceContext->IASetInputLayout(DX::g_inputLayout);
	DX::g_deviceContext->OMSetRenderTargets(1, &m_backBufferRTV, m_depthStencilView);
}
#if DEFERRED_RENDERING
void Window::_lightPass(Camera& cam)
{
	// WE DO NOT PASS sun buffer correctly
	static bool lol = false;
	static ID3D11Buffer* m_pSunBuffer;

	static DIRECTIONAL_LIGHT_BUFFER m_sunBuffer;
	if (!lol)
	{
		D3D11_BUFFER_DESC sunBdesc;
		sunBdesc.Usage = D3D11_USAGE_DYNAMIC;
		sunBdesc.ByteWidth = sizeof(DIRECTIONAL_LIGHT_BUFFER);
		sunBdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		sunBdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		sunBdesc.MiscFlags = 0;
		sunBdesc.StructureByteStride = 0;

		DX::g_device->CreateBuffer(&sunBdesc, nullptr, &m_pSunBuffer);
		lol = true;

	}

	m_sunBuffer.pos = DX::g_lightPos;
	m_sunBuffer.color = DX::g_lightDir;

	D3D11_MAPPED_SUBRESOURCE sunLightData;
	DX::g_deviceContext->Map(m_pSunBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &sunLightData);
	memcpy(sunLightData.pData, &(m_sunBuffer), sizeof(DIRECTIONAL_LIGHT_BUFFER));
	DX::g_deviceContext->Unmap(m_pSunBuffer, 0);
	DX::g_deviceContext->PSSetConstantBuffers(2, 1, &m_pSunBuffer);

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
	//DX::g_deviceContext->PSSetShaderResources(5, 1, &m_windowDepthTexture);
	DX::g_deviceContext->PSSetConstantBuffers(9, 1, &m_shadowBuffer);
	
	//Send lights to GPU
	POINT_LIGHT_COLLECTION pointLightCollectionBuffer; 
	int nrOfLights = static_cast<int>(DX::g_lightQueue.size());

	for (int i = 0; i < nrOfLights; i++)
	{
		pointLightCollectionBuffer.positionArray[i] = DX::g_lightQueue[i]->getPosition();
		pointLightCollectionBuffer.colorArray[i] =	DX::g_lightQueue[i]->getColor(); 
		pointLightCollectionBuffer.colorArray[i].w = static_cast<float>(DX::g_lightQueue[i]->getIndex());
		pointLightCollectionBuffer.lightSetup[i] = DX::g_lightQueue[i]->getLightSetup();

	}
	memset(&pointLightCollectionBuffer.positionArray[nrOfLights], 0, 16 * (100 - (nrOfLights)));
	memset(&pointLightCollectionBuffer.colorArray[nrOfLights], 0, 16 * (100 - (nrOfLights)));
	memset(&pointLightCollectionBuffer.lightSetup[nrOfLights], 0, 16 * (100 - (nrOfLights)));

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

#elif FORWARD_RENDERING
void Window::_renderEverything(const Camera & cam)
{
	

	DX::g_deviceContext->OMSetRenderTargets(1, &m_backBufferRTV, m_depthStencilView);

	DIRECTIONAL_LIGHT_BUFFER m_sunBuffer;

	m_sunBuffer.pos = DX::g_lightPos;
	m_sunBuffer.color = DX::g_lightDir;

	D3D11_MAPPED_SUBRESOURCE sunLightData;
	DX::g_deviceContext->Map(m_pSunBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &sunLightData);
	memcpy(sunLightData.pData, &(m_sunBuffer), sizeof(DIRECTIONAL_LIGHT_BUFFER));
	DX::g_deviceContext->Unmap(m_pSunBuffer, 0);
	DX::g_deviceContext->PSSetConstantBuffers(2, 1, &m_pSunBuffer);

	EVERYTHING_BUFFER eb;
	memset(&eb, 0, sizeof(EVERYTHING_BUFFER));
	XMFLOAT3 _camPos = cam.getPosition();
	eb.camPos.x = _camPos.x;
	eb.camPos.y = _camPos.y;
	eb.camPos.z = _camPos.z;

	int nrOfLights = static_cast<int>(DX::g_lightQueue.size());
	for (int i = 0; i < nrOfLights; i++)
	{
		eb.positionArray[i] = DX::g_lightQueue[i]->getPosition();
		eb.colorArray[i] = DX::g_lightQueue[i]->getColor();
		eb.colorArray[i].w = static_cast<float>(DX::g_lightQueue[i]->getIndex());
		eb.lightSetup[i] = DX::g_lightQueue[i]->getLightSetup();

	}
	eb.nrOfLights = XMFLOAT4A(static_cast<float>(nrOfLights), static_cast<float>(nrOfLights), static_cast<float>(nrOfLights), static_cast<float>(nrOfLights));
	D3D11_MAPPED_SUBRESOURCE everythingData;
	DX::g_deviceContext->Map(m_everythingConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &everythingData);
	memcpy(everythingData.pData, &eb, sizeof(EVERYTHING_BUFFER));
	DX::g_deviceContext->Unmap(m_everythingConstantBuffer, 0);
	DX::g_deviceContext->PSSetConstantBuffers(1, 1, &m_everythingConstantBuffer);

	DX::g_deviceContext->PSSetShaderResources(4, 1, &m_shadowDepthTexture);
	DX::g_deviceContext->PSSetConstantBuffers(9, 1, &m_shadowBuffer);

	_geometryPass(cam);
	_billboardPass(cam);
}
#endif
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

	while (!DX::g_instanceGroupsTransparancy.empty())
	{
		auto instance = DX::g_instanceGroupsTransparancy.front();
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

		UINT offset = 0;
		ID3D11Buffer* v = instance.shape->GetMesh()->getVertices();
		ID3D11Buffer * bufferPointers[2];
		bufferPointers[0] = v;
		bufferPointers[1] = instanceBuffer;

		unsigned int strides[2];
		strides[0] = sizeof(VERTEX);
		strides[1] = sizeof(DX::INSTANCE_ATTRIB);

		unsigned int offsets[2];
		offsets[0] = 0;
		offsets[1] = 0;



		Mesh* mesh = instance.shape->GetMesh();
		ID3D11Buffer* indices = mesh->getIndicesBuffer();

		DX::g_deviceContext->IASetIndexBuffer(indices, DXGI_FORMAT_R32_UINT, offset);
		DX::g_deviceContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);

		DX::g_deviceContext->DrawIndexedInstanced(instance.shape->GetMesh()->getNrOfIndices(), (UINT)instance.attribs.size(), 0, 0, 0);
		DX::SafeRelease(&instanceBuffer);
		DX::g_instanceGroupsTransparancy.pop_front();
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
	DX::SafeRelease(&m_backBufferRTV);
	DX::SafeRelease(&m_backBufferRTV);
	DX::SafeRelease(&m_swapChain);
	DX::SafeRelease(&m_depthStencilView);	
	DX::SafeRelease(&m_depthBufferTex);
	
	DX::SafeRelease(&m_samplerState);
	DX::SafeRelease(&m_samplerStatePoint);

	DX::SafeRelease(&m_meshConstantBuffer);
	DX::SafeRelease(&m_billboardConstantBuffer);

	DX::SafeRelease(&m_everythingConstantBuffer);

#if DEFERRED_RENDERING
	//m_pointLightsConstantBuffer->Release();
	if (m_pointLightsConstantBuffer != nullptr)
	{
		m_pointLightsConstantBuffer->Release();
	}
	m_cameraPosConstantBuffer->Release();
#endif
	DX::SafeRelease(&m_lightBuffer);
	DX::SafeRelease(&m_pPointLightBuffer);
#if DEFERRED_RENDERING
	for (size_t i = 0; i < GBUFFER_COUNT; i++)
	{
		m_gbuffer[i].SRV->Release();
		m_gbuffer[i].RTV->Release();
		m_gbuffer[i].TextureMap->Release();
	}
	m_deferredVertexShader->Release();
	m_deferredPixelShader->Release();
#endif
	DX::SafeRelease(&m_transVertexShader);
	DX::SafeRelease(&m_transPixelShader);
	DX::SafeRelease(&m_transBlendState);

	DX::SafeRelease(&m_pickingTexture.SRV);
	DX::SafeRelease(&m_pickingTexture.RTV);
	DX::SafeRelease(&m_pickingTexture.TextureMap);

	DX::SafeRelease(&m_pickingVertexShader);
	DX::SafeRelease(&m_pickingPixelShader);
	DX::SafeRelease(&m_pickingBuffer);
	DX::SafeRelease(&m_pickingReadBuffer);
	DX::SafeRelease(&m_pickingOffsetBuffer);

	DX::SafeRelease(&m_computeConstantBuffer);
	DX::SafeRelease(&m_computeOutputBuffer);
	DX::SafeRelease(&m_computeReadWriteBuffer);
	DX::SafeRelease(&m_computeUAV);
	DX::SafeRelease(&m_computeShader);

	DX::SafeRelease(&m_hudVertexShader);
	DX::SafeRelease(&m_hudPixelShader);

	DX::SafeRelease(&m_depthStencilViewShad);
	DX::SafeRelease(&m_depthBufferTexShad);
	DX::SafeRelease(&m_shadowVertex);
	DX::SafeRelease(&m_shadowPixel);
	DX::SafeRelease(&m_shadowBuffer);
	DX::SafeRelease(&m_shadowDepthTexture);
	
	DX::SafeRelease(&m_pSunBuffer);

	DX::SafeRelease(&m_WireFrame);
	DX::CleanUp();
	
	//This is for leaking, I have no idea
	ID3D11Debug* DebugDevice = nullptr;
	HRESULT Result = DX::g_device->QueryInterface(__uuidof(ID3D11Debug), (void**)&DebugDevice);
	Result = DebugDevice->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);

	DX::SafeRelease(&DX::g_deviceContext);
	DX::SafeRelease(&DX::g_device);
	
	
}

bool Window::Init(int width, int height, LPCSTR title, BOOL fullscreen)
{
	m_width = width;
	m_height = height;
	Input::m_windowSize = DirectX::XMINT2(m_width, m_height);
	m_title = title;
	m_fullscreen = fullscreen;
	_initWindow();
	HRESULT hr = _initDirect3DContext();
	_initFonts();
	_initViewPort();
	_setViewport();
	
	std::thread t1(&Window::_compileShaders, this); //_compileShaders();
#if DEFERRED_RENDERING
	std::thread t2(&Window::_initGBuffer, this);
#endif
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
#if DEFERRED_RENDERING
	t2.join();
#endif
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
	DX::g_instanceGroupsPicking.clear();
	DX::g_lightQueue.clear();
	// WINDOW Test
	DX::g_instanceGroupWindows.clear();
	// end window test

	DX::g_textQueue.clear();
	DX::g_deviceContext->ClearRenderTargetView(m_backBufferRTV, c);
	DX::g_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	DX::g_deviceContext->ClearDepthStencilView(m_depthStencilViewShad, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);
	//DX::g_deviceContext->ClearDepthStencilView(m_depthStencilViewWindow, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	DX::g_deviceContext->OMSetBlendState(nullptr, 0, 0xffffffff);
	//DX::g_lightsPos.clear();
	//DX::g_lightsDir.clear();
#if DEFERRED_RENDERING
	for (int i = 0; i < GBUFFER_COUNT; i++)
	{
		DX::g_deviceContext->ClearRenderTargetView(m_gbuffer[i].RTV, c);
	}

	ID3D11ShaderResourceView* renderTargets[GBUFFER_COUNT] = { nullptr };
	DX::g_deviceContext->PSSetShaderResources(0, GBUFFER_COUNT, renderTargets);
#endif
}

void Window::loadActiveLights(GameTime& gameTime)
{

}

void Window::Flush(Camera* c)
{

	//_windowPass(c);
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
	
	if (Input::isKeyPressed(Input::F5))
	{
		if (m_volume >= 100) {
			m_volume -= 100;
		}
		
		waveOutSetVolume(NULL, m_volume);
	}
	if (Input::isKeyPressed(Input::F6))
	{
		if (m_volume <= 65435) {
			m_volume += 100;
		}
		waveOutSetVolume(NULL, m_volume);
	}
	

	_prepareGeometryPass();
#if DEFERRED_RENDERING
	_billboardPass(*c);
	_geometryPass(*c);
	_clearTargets();
	_lightPass(*c);
#elif FORWARD_RENDERING
	_renderEverything(*c);
#endif

	_preparePostLight();
	_transparencyPass(*c);

	if(DX::g_instanceGroupsSkyBox.size() > 0) 
		_skyBoxPass(*c);
	if(DX::g_instanceGroupsHUD.size() > 0)
		_drawHUD();

	/*if(m_computeShader != nullptr)
		_runComputeShader();*/

	if(DX::g_textQueue.size() > 0)
		_drawText();
}

void Window::FullReset()
{
	//DX::g_lightQueue.clear(); 
	DX::g_deviceContext->ClearState();
	_setViewport();
	DX::g_deviceContext->PSSetSamplers(0, 1, &m_samplerState);
	DX::g_deviceContext->PSSetSamplers(1, 1, &m_samplerStatePoint);
}

Shape * Window::getPicked(Camera* c)
{
	if (!DX::g_pickingQueue.empty())
		return Picking::getPicked(c, m_pickingTexture.RTV, m_depthStencilView, m_projectionMatrix, DirectX::XMMatrixIdentity(), m_pickingBuffer, m_pickingVertexShader, m_pickingPixelShader, m_pickingTexture.TextureMap, m_pickingReadBuffer, m_meshConstantBuffer, m_computeConstantBuffer);
	return nullptr;
}

void Window::Present()
{
	m_swapChain->Present(1, 0);
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

