#include "Mesh.h"
#include "../../core/Dx.h"
#include "../../core/ObjLoader.h"
int Mesh::m_idCounter = 0;
Mesh::Mesh()
{
	m_vertexBuffer = nullptr;
	m_nrOfVertices = 0;
	m_uniqueID = m_idCounter++;
}

Mesh::~Mesh()
{
	
	if(m_vertexBuffer)
		m_vertexBuffer->Release();
	if (m_indexBuffer)
		m_indexBuffer->Release();
}

void Mesh::LoadModel(const std::string & path)
{
	std::vector<VERTEX> vertices;
	std::vector<VERTEX> indexedVertices;
	std::vector<unsigned int> indices;
	DX::loadOBJ(path, vertices);
	DX::CalculateTangents(vertices);
	DX::indexVertices(vertices, indices, indexedVertices);

	// Vertex Buffer Indexed
	D3D11_BUFFER_DESC vBufferDescIndexed;
	memset(&vBufferDescIndexed, 0, sizeof(vBufferDescIndexed));
	vBufferDescIndexed.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vBufferDescIndexed.Usage = D3D11_USAGE_DEFAULT;
	vBufferDescIndexed.ByteWidth = sizeof(VERTEX) * static_cast<UINT>(indexedVertices.size());

	D3D11_SUBRESOURCE_DATA vDataIndexed;
	vDataIndexed.pSysMem = indexedVertices.data();
	HRESULT hr = DX::g_device->CreateBuffer(&vBufferDescIndexed, &vDataIndexed, &m_vertexBuffer);

	// Index buffer
	D3D11_BUFFER_DESC vIndexBufferDesc;
	memset(&vIndexBufferDesc, 0, sizeof(vIndexBufferDesc));
	vIndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	vIndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vIndexBufferDesc.ByteWidth = sizeof(unsigned int) * static_cast<UINT>(indices.size());

	D3D11_SUBRESOURCE_DATA iData;
	iData.pSysMem = indices.data();
	hr = DX::g_device->CreateBuffer(&vIndexBufferDesc, &iData, &m_indexBuffer);

	m_nrOfVertices = static_cast<int>(indices.size());
}

void Mesh::LoadModel(std::vector<VERTEX>& v)
{
	std::vector<VERTEX> indexedVertices;
	std::vector<unsigned int> indices;
	DX::CalculateTangents(v);
	DX::indexVertices(v, indices, indexedVertices);

	// Vertex Buffer Indexed
	D3D11_BUFFER_DESC vBufferDescIndexed;
	memset(&vBufferDescIndexed, 0, sizeof(vBufferDescIndexed));
	vBufferDescIndexed.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vBufferDescIndexed.Usage = D3D11_USAGE_DEFAULT;
	vBufferDescIndexed.ByteWidth = sizeof(VERTEX) * static_cast<UINT>(indexedVertices.size());

	D3D11_SUBRESOURCE_DATA vDataIndexed;
	vDataIndexed.pSysMem = indexedVertices.data();
	HRESULT hr = DX::g_device->CreateBuffer(&vBufferDescIndexed, &vDataIndexed, &m_vertexBuffer);

	// Index buffer
	D3D11_BUFFER_DESC vIndexBufferDesc;
	memset(&vIndexBufferDesc, 0, sizeof(vIndexBufferDesc));
	vIndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	vIndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vIndexBufferDesc.ByteWidth = sizeof(unsigned int) * static_cast<UINT>(indices.size());

	D3D11_SUBRESOURCE_DATA iData;
	iData.pSysMem = indices.data();
	hr = DX::g_device->CreateBuffer(&vIndexBufferDesc, &iData, &m_indexBuffer);

	m_nrOfVertices = static_cast<int>(indices.size());
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


ID3D11Buffer * Mesh::getIndicesBuffer() const
{
	return m_indexBuffer;
}

int Mesh::getNumberOfVertices() const
{
	return m_nrOfVertices;
}

bool Mesh::CheckID(const Mesh& other) const
{
	return other.m_uniqueID == m_uniqueID;
}
