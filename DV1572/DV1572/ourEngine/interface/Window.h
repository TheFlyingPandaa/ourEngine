#pragma once
#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#include "../core/Camera.h"


class Window
{
private:
	HINSTANCE				m_hInstance;
	HWND					m_hwnd;
	INT						m_width;
	INT						m_height;
	LPCSTR					m_title;
	MSG						m_msg = { 0 };
	
	BOOL					m_fullscreen;
	IDXGISwapChain*			m_swapChain;
	ID3D11RenderTargetView*	m_backBufferRTV;

	DirectX::XMMATRIX		m_projectionMatrix; 

	ID3D11Buffer*			m_meshConstantBuffer;
	ID3D11Buffer*			m_pointLightsConstantBuffer;
	ID3D11Buffer*			m_CameraConstantBuffer;


private:
	bool	_initWindow();
	HRESULT _initDirect3DContext();
	void	_setViewport();
	bool	_compileShaders();
	void	_createConstantBuffers();

public:
	Window(HINSTANCE h);
	~Window();

	bool Init(int width, int height, LPCSTR title);
	void PollEvents();
	bool isOpen();
	void Clear();
	void Flush(const Camera& c); 
	void Present();
};