#include "Mesh.h"
#include "../../core/Dx.h"
#include "../../core/ObjLoader.h"
int Mesh::m_id = 0;
Mesh::Mesh()
{
	m_vertexBuffer = nullptr;
	m_nrOfVertices = 0;
	m_id++;
}

void Mesh::LoadModel(const std::string & path)
{
	std::vector<VERTEX> vertices;
	DX::loadOBJ(path, vertices);
	DX::CalculateTangents(vertices);
	// Vertex Buffer
	D3D11_BUFFER_DESC vBufferDesc;
	memset(&vBufferDesc, 0, sizeof(vBufferDesc));
	vBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vBufferDesc.ByteWidth = sizeof(VERTEX) * static_cast<UINT>(vertices.size());
	m_nrOfVertices = vertices.size();

	D3D11_SUBRESOURCE_DATA vData;
	vData.pSysMem = vertices.data();
	HRESULT hr = DX::g_device->CreateBuffer(&vBufferDesc, &vData, &m_vertexBuffer);
}

void Mesh::LoadModel(const std::vector<VERTEX>& v)
{
	// Vertex Buffer
	D3D11_BUFFER_DESC vBufferDesc;
	memset(&vBufferDesc, 0, sizeof(vBufferDesc));
	vBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vBufferDesc.ByteWidth = sizeof(VERTEX) * static_cast<UINT>(v.size());

	D3D11_SUBRESOURCE_DATA vData;
	vData.pSysMem = v.data();
	m_nrOfVertices = v.size();
	HRESULT hr = DX::g_device->CreateBuffer(&vBufferDesc, &vData, &m_vertexBuffer);
}

void Mesh::setDiffuseTexture(const std::string& path)
{
	ID3D11Resource* texture;
	ID3D11ShaderResourceView* textureView;
	DX::loadTexture(path, texture, textureView);
	m_material.setDiffuseMap(textureView, texture);
}

void Mesh::setNormalTexture(const std::string & path)
{
	ID3D11Resource* texture;
	ID3D11ShaderResourceView* textureView;
	DX::loadTexture(path, texture, textureView);
	m_material.setNormalMap(textureView, texture);
}

void Mesh::setHighlightTexture(const std::string & path)
{
	ID3D11Resource* texture;
	ID3D11ShaderResourceView* textureView;
	DX::loadTexture(path, texture, textureView);
	m_material.setHighlightMap(textureView, texture);
}

Material* Mesh::getMaterial()
{
	return &m_material;
}

ID3D11Buffer * Mesh::getVertices() const
{
	return m_vertexBuffer;
}

int Mesh::getNumberOfVertices() const
{
	return m_nrOfVertices;
}

bool Mesh::CheckID(const Mesh& other) const
{
	return other.m_id == m_id;
}
