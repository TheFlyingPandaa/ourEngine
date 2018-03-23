#include "Material.h"
Material::Material()
{
	m_diffuseTexture = nullptr;
	m_diffuseResource = nullptr;

	m_normalTexture = nullptr;
	m_normalResource = nullptr;

	m_highlightTexture = nullptr;
	m_highlightResource = nullptr;

	m_transparancy = 1.0f;
}

Material::~Material()
{
	m_diffuseTexture->Release();
	m_diffuseResource->Release();

	m_normalTexture->Release();
	m_normalResource->Release();

	m_highlightTexture->Release();
	m_highlightResource->Release();
}

void Material::setDiffuseMap(ID3D11ShaderResourceView* srv, ID3D11Resource* res)
{
	m_diffuseTexture = srv;
	m_diffuseResource = res;
}

void Material::setNormalMap(ID3D11ShaderResourceView* srv, ID3D11Resource* res)
{
	m_normalTexture = srv;
	m_normalResource = res;
}

void Material::setHighlightMap(ID3D11ShaderResourceView* srv, ID3D11Resource* res)
{
	m_highlightTexture = srv;
	m_highlightResource = res;
}

void Material::setTransparency(float alpha)
{
	m_transparancy = alpha;
}

ID3D11ShaderResourceView* Material::getDiffuseMap() const
{
	return m_diffuseTexture;
}

ID3D11ShaderResourceView* Material::getNormalMap() const
{
	return m_normalTexture;
}

ID3D11ShaderResourceView* Material::getHighlightMap() const
{
	return m_highlightTexture;
}

float Material::getTransparency() const
{
	return m_transparancy;
}

bool Material::isTransparent() const
{
	return m_transparancy < 1.0f;
}
