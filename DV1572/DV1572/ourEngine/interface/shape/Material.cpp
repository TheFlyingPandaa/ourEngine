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
	DX::SafeRelease(&m_diffuseTexture);
	DX::SafeRelease(&m_diffuseResource);
	DX::SafeRelease(&m_normalTexture);
	DX::SafeRelease(&m_normalResource);
	DX::SafeRelease(&m_highlightTexture);
	DX::SafeRelease(&m_highlightResource);

}

void Material::setDiffuseMap(ID3D11ShaderResourceView* srv, ID3D11Resource* res)
{
	if (m_diffuseResource) std::cout << "Diffuse already set before!" << std::endl;
	m_diffuseTexture = srv;
	m_diffuseResource = res;
}

bool Material::setDiffuseMap(const std::string & path)
{
	if (m_diffuseResource) std::cout << "Diffuse already set before!" << std::endl;
	return DX::loadTexture(path, m_diffuseResource, m_diffuseTexture);
}

void Material::setNormalMap(ID3D11ShaderResourceView* srv, ID3D11Resource* res)
{
	if (m_normalResource) std::cout << "Normal already set before!" << std::endl;
	m_normalTexture = srv;
	m_normalResource = res;
}

bool Material::setNormalMap(const std::string & path)
{
	if (m_normalResource) std::cout << "Normal already set before!" << std::endl;
	return DX::loadTexture(path, m_normalResource, m_normalTexture);
}

void Material::setHighlightMap(ID3D11ShaderResourceView* srv, ID3D11Resource* res)
{
	if (m_highlightResource) std::cout << "Highlight already set before!" << std::endl;
	m_highlightTexture = srv;
	m_highlightResource = res;
}

bool Material::setHighlightMap(const std::string & path)
{
	if (m_highlightResource) std::cout << "Highlight already set before!" << std::endl;
	return DX::loadTexture(path, m_highlightResource, m_highlightTexture);
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

bool Material::operator==(const Material & other) const
{
	return m_name == other.m_name;
}
