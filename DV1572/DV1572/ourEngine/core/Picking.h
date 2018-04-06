#pragma once
#include <d3d11.h>
#pragma comment (lib, "d3d11.lib")
#include "../interface/shape/Rectangle.h"
#include "../core/Camera/Camera.h"
class Picking
{
private:


public:
	Picking();
	~Picking();

	static void InitPickingTexture(const int widht, const int height, const int sample, ID3D11Texture2D*& TextureMap, ID3D11RenderTargetView*&RTV, ID3D11ShaderResourceView*&SRV, ID3D11Texture2D*&  m_pickingReadBuffer);
	static Shape* getPicked(Camera* c, ID3D11RenderTargetView*&RTV, ID3D11DepthStencilView*& m_depthStencilView, const DirectX::XMMATRIX m_projectionMatrix, DirectX::XMMATRIX m_HUDview, ID3D11Buffer*& m_pickingBuffer, ID3D11VertexShader*& m_pickingVertexShader, ID3D11PixelShader*& m_pickingPixelShader, ID3D11Texture2D*& TextureMap, ID3D11Texture2D*& m_pickingReadBuffer, ID3D11Buffer *& m_meshConstantBuffer);
};   