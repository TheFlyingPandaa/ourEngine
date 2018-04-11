#include "Material.h"
#include "../../core/ObjLoader.h"

Material::Material(std::string name)
	: m_name(name)
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

void Material::setDiffuseMap(const std::string & path)
{
	DX::loadTexture(path, m_diffuseResource, m_diffuseTexture);
}

void Material::setNormalMap(ID3D11ShaderResourceView* srv, ID3D11Resource* res)
{
	m_normalTexture = srv;
	m_normalResource = res;
}

void Material::setNormalMap(const std::string & path)
{
	DX::loadTexture(path, m_normalResource, m_normalTexture);
}

void Material::setHighlightMap(ID3D11ShaderResourceView* srv, ID3D11Resource* res)
{
	m_highlightTexture = srv;
	m_highlightResource = res;
}

void Material::setHighlightMap(const std::string & path)
{
	DX::loadTexture(path, m_highlightResource, m_highlightTexture);
}

void Material::setTransparency(float alpha)
{
	m_transparancy = alpha;
}

void Material::setSpecularExponent(float specular)
{
	m_specularExponent = specular;
}

const std::string & Material::getName() const
{
	return m_name;
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
