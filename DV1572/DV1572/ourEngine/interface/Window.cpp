#include "Window.h"
#include "../core/Dx.h"
ID3D11Device* DX::g_device;
ID3D11DeviceContext* DX::g_deviceContext;
ID3D11VertexShader* DX::g_3DVertexShader;
ID3D11PixelShader* DX::g_3DPixelShader;
ID3D11InputLayout* DX::g_inputLayout;
std::vector<Shape*> DX::g_renderQueue;
std::vector<Shape*> DX::g_shadowQueue;
std::vector<Shape*> DX::g_transQueue;

void DX::CleanUp()
{
	DX::g_device->Release();
	DX::g_deviceContext->Release();
	DX::g_3DVertexShader->Release();
	DX::g_3DPixelShader->Release();
	DX::g_inputLayout->Release();
}


bool Window::_initWindow()
{
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = StaticWndProc;
	wcex.hInstance = m_hInstance;
	wcex.lpszClassName = "ourEngine";
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
	scd.OutputWindow = m_hwnd;                           // the window to be used
	scd.SampleDesc.Count = m_sampleCount;                               // how many multisamples
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
		_createDepthBuffer();
		DX::g_deviceContext->OMSetRenderTargets(1, &m_backBufferRTV, m_depthStencilView);
		pBackBuffer->Release();
	}
	return hr;
}

void Window::_setViewport()
{
	D3D11_VIEWPORT vp;
	vp.Width = static_cast<float>(m_width);
	vp.Height = static_cast<float>(m_height);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	DX::g_deviceContext->RSSetViewports(1, &vp);
}

bool Window::_compileShaders()
{
	// Vertex Shader
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXELS", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 }
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


	return true;
}

void Window::_createConstantBuffers()
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

	for (auto &g : m_gbuffer)
	{
		DX::g_device->CreateTexture2D(&tDesc, nullptr, &g.TextureMap);
		DX::g_device->CreateRenderTargetView(g.TextureMap, &rDesc, &g.RTV);
		DX::g_device->CreateShaderResourceView(g.TextureMap, &sDesc, &g.SRV);
	}

}

void Window::_prepareGeometryPass()
{
	DX::g_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DX::g_deviceContext->IASetInputLayout(DX::g_inputLayout);

	float color[4]{ 1.0f, 0.0f, 1.0f, 1.0f };

	ID3D11RenderTargetView* renderTargets[GBUFFER_COUNT];

	for (int i = 0; i < GBUFFER_COUNT; i++)
	{
		renderTargets[i] = m_gbuffer[i].RTV;
	}
	DX::g_deviceContext->OMSetRenderTargets(GBUFFER_COUNT, renderTargets, m_depthStencilView);
}

void Window::_geometryPass(const Camera &cam)
{
	DirectX::XMMATRIX view = cam.getViewMatrix();
	DirectX::XMMATRIX viewProj = view * m_projectionMatrix;

	MESH_BUFFER meshBuffer;
	for (size_t i = 0; i < DX::g_renderQueue.size(); i++)
	{
		DirectX::XMMATRIX world = DX::g_renderQueue[i]->getWorld();
		DirectX::XMStoreFloat4x4A(&meshBuffer.world, DirectX::XMMatrixTranspose(world));
		DirectX::XMMATRIX wvp = DirectX::XMMatrixTranspose(world * viewProj);
		DirectX::XMStoreFloat4x4A(&meshBuffer.MVP, wvp);

		D3D11_MAPPED_SUBRESOURCE dataPtr;
		DX::g_deviceContext->Map(m_meshConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);
		memcpy(dataPtr.pData, &meshBuffer, sizeof(MESH_BUFFER));
		DX::g_deviceContext->Unmap(m_meshConstantBuffer, 0);
		DX::g_deviceContext->VSSetConstantBuffers(0, 1, &m_meshConstantBuffer);

		DX::g_renderQueue[i]->ApplyShaders();

		UINT32 vertexSize = sizeof(VERTEX);
		UINT offset = 0;

		ID3D11Buffer* v = DX::g_renderQueue[i]->getVertices();
		DX::g_deviceContext->IASetVertexBuffers(0, 1, &v, &vertexSize, &offset);
		DX::g_deviceContext->Draw(DX::g_renderQueue[i]->getMesh()->getNumberOfVertices(), 0);
	}
}

void Window::_clearTargets()
{
	ID3D11RenderTargetView* renderTargets[GBUFFER_COUNT] = { nullptr };
	DX::g_deviceContext->OMSetRenderTargets(GBUFFER_COUNT, renderTargets, NULL);
}

void Window::_lightPass()
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

	DX::g_deviceContext->IASetInputLayout(nullptr);

	DX::g_deviceContext->Draw(4, 0);
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
}

Window::~Window()
{
	m_swapChain->Release();
	m_backBufferRTV->Release();
	
	DX::CleanUp();
	
	//This is for leaking, I have no idea
	ID3D11Debug* DebugDevice = nullptr;
	HRESULT Result = DX::g_device->QueryInterface(__uuidof(ID3D11Debug), (void**)&DebugDevice);
	Result = DebugDevice->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	DX::g_device->Release();
}

bool Window::Init(int width, int height, LPCSTR title, BOOL fullscreen)
{
	m_width = width;
	m_height = height;
	m_title = title;
	m_fullscreen = fullscreen;
	_initWindow();
	HRESULT hr = _initDirect3DContext();
	_setViewport();
	_compileShaders();
	_createConstantBuffers(); 
	_initGBuffer();

	m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45), static_cast<float>(m_width) / m_height, 0.1f, 200.0f); 
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
	float c[4] = { 1.0f,0.1f,0.9f,1.0f };
	DX::g_renderQueue.clear(); 
	DX::g_deviceContext->ClearRenderTargetView(m_backBufferRTV, c);
	DX::g_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);

	for (int i = 0; i < GBUFFER_COUNT; i++)
	{
		DX::g_deviceContext->ClearRenderTargetView(m_gbuffer[i].RTV, c);
	}

}

void Window::Flush(const Camera & c)
{
	_prepareGeometryPass();
	_geometryPass(c);
	_clearTargets();
	_lightPass();
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
		m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45), static_cast<float>(m_width) / m_height, 0.1f, 200.0f);
		if (m_swapChain)
		{
			DX::g_deviceContext->OMSetRenderTargets(0, 0, 0);

			// Release all outstanding references to the swap chain's buffers.
			m_backBufferRTV->Release();

			HRESULT hr;
			// Preserve the existing buffer count and format.
			// Automatically choose the width and height to match the client rect for HWNDs.
			hr = m_swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

			// Perform error handling here!

			// Get buffer and create a render-target-view.
			ID3D11Texture2D* pBuffer;
			hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
				(void**)&pBuffer);
			// Perform error handling here!

			hr = DX::g_device->CreateRenderTargetView(pBuffer, NULL,
				&m_backBufferRTV);
			// Perform error handling here!
			pBuffer->Release();
			
			_createDepthBuffer();
			DX::g_deviceContext->OMSetRenderTargets(1, &m_backBufferRTV, m_depthStencilView);

			// Set up the viewport.
			_setViewport();

			
			
		}
		break;
	case WM_KEYDOWN:
		// --------------------------------Subject for change!--------------------------------
		if (wParam == VK_ESCAPE)
			exit(0);
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
