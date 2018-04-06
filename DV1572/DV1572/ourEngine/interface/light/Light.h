#ifndef LIGHT_H 
#define LIGHT_H
#include "../../Structs.h"
#include <DirectXMath.h>
#include <d3d11.h>
#include "../../core/Camera/Camera.h"

class Light
{
private: 
	DIRECTIONAL_LIGHT_BUFFER		m_lightBuffer;
	DirectX::XMFLOAT4A				m_pos;
	DirectX::XMFLOAT4A				m_dir;
	DirectX::XMFLOAT4A				m_color;

	DirectX::XMMATRIX				m_viewProj;
	DirectX::XMMATRIX				m_proj;

	const DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	float						m_width; 
	float						m_height; 

	ID3D11Texture2D*			m_shadowTexture; 
	ID3D11DepthStencilView*		m_shadowDepthStencilView; 
	ID3D11ShaderResourceView*	m_shadowShaderResourceView; 

	ID3D11Buffer*				m_pLightBuffer;

	void _createResources(); 

public:
	Light(); 
	~Light();

	DirectX::XMFLOAT4A getPos() const;
	DirectX::XMFLOAT4A getDir() const;
	DirectX::XMFLOAT4A getColor() const; 

	void Init(DirectX::XMFLOAT4A pos, DirectX::XMFLOAT4A dir, DirectX::XMFLOAT4A color, float width, float height); 

	void setPos(DirectX::XMFLOAT4A pos); 
	void setDir(DirectX::XMFLOAT4A dir); 
	void setColor(DirectX::XMFLOAT4A color); 

	void Move(DirectX::XMFLOAT4A move); 

	void updateMatrix();

	DirectX::XMMATRIX getViewProjMatrix() const;

	ID3D11DepthStencilView*& getDepthView(); 
	ID3D11ShaderResourceView*& getResourceView();

	DIRECTIONAL_LIGHT_BUFFER& getBuffer(); 
	ID3D11Buffer* getBufferPointer(); 
};


#endif

