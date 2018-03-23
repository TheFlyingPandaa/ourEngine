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
	if (m_diffuseTexture)
		m_diffuseTexture->Release();
	if (m_diffuseResource)
		m_diffuseResource->Release();
	if (m_normalTexture)
		m_normalTexture->Release();
	if (m_normalResource)
		m_normalResource->Release();
	if (m_highlightTexture)
		m_highlightTexture->Release();
	if (m_highlightResource)
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
