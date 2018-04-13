#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#include "gdiplus.h"
#pragma comment (lib, "gdiplus.lib")
#include <string>


class Material
{
private:
	ID3D11ShaderResourceView*	m_diffuseTexture;	//map_Kd
	ID3D11Resource*				m_diffuseResource;	
	
	ID3D11ShaderResourceView*	m_normalTexture;	//map_bump or bump
	ID3D11Resource*				m_normalResource;
	
	ID3D11ShaderResourceView*	m_highlightTexture;	//map_Ks
	ID3D11Resource*				m_highlightResource;

	float						m_transparancy;
	float						m_specularExponent;	//Ns
	std::string					m_name;
public:
	Material(std::string name = "NoName");
	~Material();
	void setDiffuseMap(ID3D11ShaderResourceView* srv, ID3D11Resource * res);
	void setDiffuseMap(const std::string& path);
	void setNormalMap(ID3D11ShaderResourceView* srv, ID3D11Resource * res);
	void setNormalMap(const std::string& path);
	void setHighlightMap(ID3D11ShaderResourceView* srv, ID3D11Resource * res);
	void setHighlightMap(const std::string& path);

	void setTransparency(float alpha);
	void setSpecularExponent(float specular);

	const std::string& getName() const;

	ID3D11ShaderResourceView* getDiffuseMap() const;
	ID3D11ShaderResourceView* getNormalMap() const;
	ID3D11ShaderResourceView* getHighlightMap() const;
	float getTransparency() const;

	bool isTransparent() const;
	bool operator==(const Material& other) const;
};