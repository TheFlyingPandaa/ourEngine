#pragma once
#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#include "../core/Camera/Camera.h"
#include "shape\Rectangle.h"
#include "light\PointLight.h"
#include "Input.h"
#include "../../trolls_inn/Time Management/GameTime.h"

#include "../core/RenderDefine.h"

#if DEFERRED_RENDERING
	const UINT GBUFFER_COUNT = 4;
#endif

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
	ID3D11SamplerState*		m_samplerStatePoint;

	ID3D11Buffer*			m_meshConstantBuffer;
	ID3D11Buffer*			m_billboardConstantBuffer;
#if DEFERRED_RENDERING
	ID3D11Buffer*			m_pointLightsConstantBuffer;
	ID3D11Buffer*			m_cameraPosConstantBuffer;
#elif FORWARD_RENDERING
	ID3D11Buffer*			m_everythingConstantBuffer;
#endif
	ID3D11Buffer*			m_lightBuffer; 
	ID3D11Buffer*			m_pPointLightBuffer; 
	

	INT						m_sampleCount;

#if DEFERRED_RENDERING
	//Deferred Rendering
	GBUFFER					m_gbuffer[GBUFFER_COUNT];
	ID3D11VertexShader*		m_deferredVertexShader;
	ID3D11PixelShader*		m_deferredPixelShader;
#endif
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
	ID3D11Buffer*			m_pickingOffsetBuffer;


	//ComputeShader
	ID3D11Buffer*			m_computeConstantBuffer;
	ID3D11Buffer*			m_computeOutputBuffer;
	ID3D11Buffer*			m_computeReadWriteBuffer;
	ID3D11UnorderedAccessView* m_computeUAV;
	ID3D11ComputeShader*	m_computeShader;

	//HUD
	ID3D11VertexShader*		m_hudVertexShader;
	ID3D11PixelShader*		m_hudPixelShader;

	//ShadowPass
	ID3D11DepthStencilView* m_depthStencilViewShad;
	ID3D11Texture2D*		m_depthBufferTexShad;
	ID3D11VertexShader	*	m_shadowVertex;
	ID3D11PixelShader	*	m_shadowPixel;
	ID3D11Buffer		*	m_shadowBuffer;
	ID3D11ShaderResourceView * m_shadowDepthTexture = nullptr;
	XMMATRIX				m_shadowProjMatrix;

	// SunBuffer
	ID3D11Buffer*			m_pSunBuffer;

	//DEBUG
	ID3D11RasterizerState*	m_WireFrame;
	bool					m_WireFrameDebug = false;;


	DirectX::XMMATRIX m_HUDviewProj;

	D3D11_VIEWPORT m_viewport;

	int m_volume = 65535/2; //Half blast volume

private:
	bool	_initWindow();
	HRESULT _initDirect3DContext();
	void	_initViewPort();
	void	_setViewport();
	void	_compileShaders();
	void	_setSamplerState();
	void	_createConstantBuffers();
	void	_createMeshConstantBuffer();
	void	_createBillboardConstantBuffer();
	void	_createPickConstantBuffer();
#if DEFERRED_RENDERING
	void	_createCameraPosConstantBuffer(); 
	void	_createPointLightCollectionBuffer(); 
#elif FORWARD_RENDERING
	void	_createEverythingConstantBuffer();
#endif


	void	_createDepthBuffer();
	
	// Deferred Rendering
#if DEFERRED_RENDERING
	void	_initGBuffer();
	void	_clearTargets();
	void	_lightPass(Camera& cam);
#elif FORWARD_RENDERING
	void	_renderEverything(const Camera& cam);
#endif
	void	_billboardPass(const Camera & cam);
	void	_geometryPass(const Camera &cam);
	void	_prepareGeometryPass();
	void	_preparePostLight();
	void	_skyBoxPass(const Camera & cam);
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
	void	_loadShadowBuffers();
	void	_prepareShadow();
	void	_shadowPass(Camera* c);

	//Window
	void	_loadWindowBuffers();
	void	_windowPass(Camera* c);

	void	_initFonts();
	void	_drawText();

public:
	Window(HINSTANCE h);
	~Window();

	bool Init(int width, int height, LPCSTR title, BOOL fullscreen = 0);
	void PollEvents();
	bool isOpen();
	void Clear();
	void loadActiveLights(GameTime& gameTime); 
	void Flush(Camera* c);

	void FullReset();

	//Do this once per loop and if pressed
	Shape* getPicked(Camera* c);

	void Present();
	LRESULT WndProc(UINT, WPARAM, LPARAM);
	static LRESULT CALLBACK StaticWndProc(HWND, UINT, WPARAM, LPARAM);
	void setMouseMiddleScreen();
	void setTitle(LPCSTR string);
	DirectX::XMFLOAT2 getSize() const;
};