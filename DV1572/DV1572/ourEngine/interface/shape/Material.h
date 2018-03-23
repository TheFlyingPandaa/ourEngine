#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#include "gdiplus.h"
#pragma comment (lib, "gdiplus.lib")

class Material
{
private:
	ID3D11ShaderResourceView*	m_diffuseTexture;	//map_Ka
	ID3D11Resource*				m_diffuseResource;	
	
	ID3D11ShaderResourceView*	m_normalTexture;	//map_bump or bump
	ID3D11Resource*				m_normalResource;
	
	ID3D11ShaderResourceView*	m_highlightTexture;	//map_Ns
	ID3D11Resource*				m_highlightResource;

	float						m_transparancy;
public:
	Material();
	~Material();
	void setDiffuseMap(ID3D11ShaderResourceView* srv, ID3D11Resource * res);
	void setNormalMap(ID3D11ShaderResourceView* srv, ID3D11Resource * res);
	void setHighlightMap(ID3D11ShaderResourceView* srv, ID3D11Resource * res);
	void setTransparency(float alpha);

	ID3D11ShaderResourceView* getDiffuseMap() const;
	ID3D11ShaderResourceView* getNormalMap() const;
	ID3D11ShaderResourceView* getHighlightMap() const;
	float getTransparency() const;

	bool isTransparent() const;
};