#pragma once
#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#include "../core/Camera/Camera.h"
#include "shape\Rectangle.h"
#include "light\Light.h"
#include "Input.h"

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

	struct computeBuffer {
		float temp;
		float temp2;
		float temp3;
		float temp4;
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
	ID3D11Buffer*			m_cameraPosConstantBuffer;
	ID3D11Buffer*			m_lightBuffer; 
	

	INT						m_sampleCount;

	//Deferred Rendering
	GBUFFER					m_gbuffer[GBUFFER_COUNT];
	ID3D11VertexShader*		m_deferredVertexShader;
	ID3D11PixelShader*		m_deferredPixelShader;

	//Transpar
	ID3D11VertexShader*		m_transVertexShader;
	ID3D11PixelShader*		m_transPixelShader;
	ID3D11BlendState*		m_transBlendState;

	//Picking
	GBUFFER					m_pickingTexture;
	ID3D11VertexShader*		m_pickingVertexShader;
	ID3D11PixelShader*		m_pickingPixelShader;
	ID3D11Buffer*			m_pickingBuffer;
	ID3D11Texture2D*		m_pickingReadBuffer;

	//ComputeShader
	ID3D11Buffer*			m_computeConstantBuffer;
	ID3D11Buffer*			m_computeOutputBuffer;
	ID3D11Buffer*			m_computeReadWriteBuffer;
	ID3D11UnorderedAccessView* m_computeUAV;

	//Shadow
	ID3D11Texture2D*			m_shadowDepthTexture;
	ID3D11DepthStencilView*		m_shadowDSV;
	ID3D11ShaderResourceView*	m_shadowSRV;
	ID3D11VertexShader*			m_shadowVertexShader;
	ID3D11PixelShader*			m_shadowPixelShader;

	ID3D11ComputeShader*	m_computeShader;

	// Input
	DirectX::XMFLOAT2 m_mousePos;

	DirectX::XMMATRIX m_HUDviewProj;

	float tempp = 0;

private:
	bool	_initWindow();
	HRESULT _initDirect3DContext();
	void	_setViewport();
	void	_compileShaders();
	void	_setSamplerState();
	void	_createConstantBuffers();
	void	_createMeshConstantBuffer();
	void	_createPickConstantBuffer();
	void	_createCameraPosConstantBuffer(); 


	void	_createDepthBuffer();
	
	// Deferred Rendering
	void	_shadowPass(const Camera & cam,const Light& light);
	void	_initGBuffer();
	void	_prepareGeometryPass();
	void	_geometryPass(const Camera & cam);
	void	_clearTargets();
	void	_lightPass(Light& light, Camera& cam);
	//Transparency
	void	_transparencyPass(const Camera & cam);
	void	_initTransparency();

	//	Picking
	void	_initPickingShaders();
	
	// Tessellation
	void	_initTessellationShaders();

	// HUD
	void	_drawHUD();

	//ComputeShader
	void	_initComputeShader();
	void	_runComputeShader();

	//Shadow
	void	_initShadowBuffer();
	void	_clearShaderResources();


public:
	Window(HINSTANCE h);
	~Window();

	bool Init(int width, int height, LPCSTR title, BOOL fullscreen = 0);
	void PollEvents();
	bool isOpen();
	void Clear();
	void Flush(Camera* c, Light& light);

	//Do this once per loop and if pressed
	Shape* getPicked(Camera* c);


	void Present();
	LRESULT WndProc(UINT, WPARAM, LPARAM);
	static LRESULT CALLBACK StaticWndProc(HWND, UINT, WPARAM, LPARAM);
	void setMouseMiddleScreen();

	DirectX::XMFLOAT2 getSize() const;
	DirectX::XMFLOAT2 getMousePos();
};