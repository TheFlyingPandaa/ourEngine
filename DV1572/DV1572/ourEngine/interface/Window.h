#pragma once
#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#include "../core/Camera/Camera.h"
#include "OurMath.h"
#include "shape\Shape.h"

using namespace OurMath;

const UINT GBUFFER_COUNT = 3;

class Window
{
private:
	struct GBUFFER
	{
		ID3D11Texture2D*			TextureMap;
		ID3D11RenderTargetView*		RTV;
		ID3D11ShaderResourceView*	SRV;
	};

private:
	HINSTANCE				m_hInstance;
	HWND					m_hwnd;
	INT						m_width;
	INT						m_height;
	LPCSTR					m_title;
	MSG						m_msg = { 0 };
	
	BOOL					m_fullscreen;
	ID3D11RenderTargetView*	m_backBufferRTV;
	IDXGISwapChain*			m_swapChain;
	
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11Texture2D*		m_depthBufferTex;

	DirectX::XMMATRIX		m_projectionMatrix; 

	ID3D11SamplerState*		m_samplerState;

	ID3D11Buffer*			m_meshConstantBuffer;
	ID3D11Buffer*			m_pointLightsConstantBuffer;
	ID3D11Buffer*			m_CameraConstantBuffer;
	

	INT						m_sampleCount;

	//Deferred Rendering
	GBUFFER					m_gbuffer[GBUFFER_COUNT];
	ID3D11VertexShader*		m_deferredVertexShader;
	ID3D11PixelShader*		m_deferredPixelShader;

	//Picking
	GBUFFER					m_pickingTexture;
	ID3D11VertexShader*		m_pickingVertexShader;
	ID3D11PixelShader*		m_pickingPixelShader;
	ID3D11Buffer*			m_pickingBuffer;
	ID3D11Texture2D*		m_pickingReadBuffer;

	//Tessellation
	ID3D11HullShader*		m_standardHullShader;
	ID3D11DomainShader*		m_standardDomainShader;


	// Input
	Vec2					m_mousePos;
	
	// Function callback pointers
	void(*m_windowSizeCallbackFunc)(int, int);
	
	void(Camera::*m_mousePositionFunc)(Vec2);
	Camera* m_cameraFuncCaller;
	DirectX::XMMATRIX m_HUDview;

private:
	bool	_initWindow();
	HRESULT _initDirect3DContext();
	void	_setViewport();
	void	_compileShaders();
	void	_setSamplerState();
	void	_createConstantBuffers();
	void	_createMeshConstantBuffer();
	void	_createPickConstantBuffer();


	void	_createDepthBuffer();
	
	// Deferred Rendering
	void	_initGBuffer();
	void	_prepareGeometryPass();
	void	_geometryPass(const Camera & cam);
	void	_clearTargets();
	void	_lightPass();


	//	Picking
	void	_initPickingTexture();
	void	_initPickingShaders();
	
	// Tessellation
	void	_initTessellationShaders();

	// HUD
	void	_drawHUD();


public:
	Window(HINSTANCE h);
	~Window();

	bool Init(int width, int height, LPCSTR title, BOOL fullscreen = 0);
	void PollEvents();
	bool isOpen();
	void Clear();
	void Flush(Camera* c);

	//Do this once per loop and if pressed
	Shape* getPicked(Camera* c);


	void Present();
	LRESULT WndProc(UINT, WPARAM, LPARAM);
	static LRESULT CALLBACK StaticWndProc(HWND, UINT, WPARAM, LPARAM);
	void setMouseMiddleScreen();

	void setWindowSizeCallback(void(*func)(int, int));
	void setMousePositionCallback(Camera* object, void(Camera::*func)(Vec2));

	Vec2 getSize() const;
	Vec2 getMousePos();
};