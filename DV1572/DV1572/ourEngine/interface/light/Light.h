#ifndef LIGHT_H 
#define LIGHT_H
#include "../../Structs.h"
#include <DirectXMath.h>
#include <d3d11.h>
#include "../../core/Camera/Camera.h"
using namespace DirectX; 
class Light
{
private: 
	DIRECTIONAL_LIGHT_BUFFER m_lightBuffer; 
	XMFLOAT4A m_pos;
	XMFLOAT4A m_dir; 
	XMFLOAT4A m_color;

	//DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT3 pos, lookAt;
	const DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	float m_width; 
	float m_height; 

	ID3D11Texture2D* m_pShadowTexture; 
	ID3D11DepthStencilView* m_pDepthStencilView; 
	ID3D11ShaderResourceView* m_pShaderResourceView; 

	void _createResources(); 

public:
	Light(); 
	~Light(); 

	ID3D11Buffer* m_pLightBuffer;

	XMFLOAT4A getPos() const; 
	XMFLOAT4A getDir() const;
	XMFLOAT4A getColor() const; 

	void Init(XMFLOAT4A pos, XMFLOAT4A dir, XMFLOAT4A color, float width, float height); 

	void setPos(XMFLOAT4A pos); 
	void setDir(XMFLOAT4A dir); 
	void setColor(XMFLOAT4A color); 

	void Move(XMFLOAT4A move); 

	void updateMatrix(const Camera& cam); 

	ID3D11DepthStencilView*& getDepthView(); 
	DIRECTIONAL_LIGHT_BUFFER& getBuffer(); 
	ID3D11Buffer* getBufferPointer(); 
};


#endif

