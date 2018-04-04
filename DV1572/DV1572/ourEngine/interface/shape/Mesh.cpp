#include "Mesh.h"
#include "../../core/Dx.h"
#include "../../core/ObjLoader.h"

// Unique ID 
int Mesh::m_uniqueID = 0;

Mesh::Mesh()
{
	m_vertexBufferIndexed = nullptr;
	m_vertexBufferNonIndexed = nullptr;
	m_indexBuffer = nullptr;
	m_nrOfVerticesIndexed = 0;
	m_nrOfVerticesNonIndexed = 0;
	m_id = m_uniqueID++;
}

void Mesh::LoadModel(const std::string & path)
{
	std::vector<VERTEX> vertices;
	std::vector<VERTEX> out_vertices;
	std::vector<unsigned int> indices;
	DX::loadOBJ(path, vertices);
	DX::CalculateTangents(vertices);
	DX::indexVertices(vertices, indices, out_vertices);

	// Vertex Buffer non indexed
	D3D11_BUFFER_DESC vBufferDesc;
	memset(&vBufferDesc, 0, sizeof(vBufferDesc));
	vBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vBufferDesc.ByteWidth = sizeof(VERTEX) * static_cast<UINT>(vertices.size());

	D3D11_SUBRESOURCE_DATA vData;
	vData.pSysMem = vertices.data();
	HRESULT hr = DX::g_device->CreateBuffer(&vBufferDesc, &vData, &m_vertexBufferNonIndexed);
	m_nrOfVerticesNonIndexed = vertices.size();

	// Vertex Buffer Indexed
	D3D11_BUFFER_DESC vBufferDescIndexed;
	memset(&vBufferDescIndexed, 0, sizeof(vBufferDescIndexed));
	vBufferDescIndexed.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vBufferDescIndexed.Usage = D3D11_USAGE_DEFAULT;
	vBufferDescIndexed.ByteWidth = sizeof(VERTEX) * static_cast<UINT>(out_vertices.size());

	D3D11_SUBRESOURCE_DATA vDataIndexed;
	vDataIndexed.pSysMem = out_vertices.data();
	hr = DX::g_device->CreateBuffer(&vBufferDescIndexed, &vDataIndexed, &m_vertexBufferIndexed);

	// Index buffer
	D3D11_BUFFER_DESC vIndexBufferDesc;
	memset(&vIndexBufferDesc, 0, sizeof(vIndexBufferDesc));
	vIndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	vIndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vIndexBufferDesc.ByteWidth = sizeof(unsigned int) * static_cast<UINT>(indices.size());

	D3D11_SUBRESOURCE_DATA iData;
	iData.pSysMem = indices.data();
	hr = DX::g_device->CreateBuffer(&vIndexBufferDesc, &iData, &m_indexBuffer);

	m_nrOfVerticesIndexed = indices.size();

	
}

void Mesh::LoadModel(const std::vector<VERTEX>& v)
{
	std::vector<unsigned int> indices;
	std::vector<VERTEX> outVertices;
	DX::indexVertices(v, indices, outVertices);
	
	// Vertex Buffer nonIndexed
	D3D11_BUFFER_DESC vBufferDescNonIndexed;
	memset(&vBufferDescNonIndexed, 0, sizeof(vBufferDescNonIndexed));
	vBufferDescNonIndexed.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vBufferDescNonIndexed.Usage = D3D11_USAGE_DEFAULT;
	vBufferDescNonIndexed.ByteWidth = sizeof(VERTEX) * static_cast<UINT>(v.size());

	D3D11_SUBRESOURCE_DATA vDataNonIndexed;
	vDataNonIndexed.pSysMem = outVertices.data();
	m_nrOfVerticesNonIndexed = v.size();
	HRESULT hr = DX::g_device->CreateBuffer(&vBufferDescNonIndexed, &vDataNonIndexed, &m_vertexBufferNonIndexed);


	// Vertex Buffer Indexed
	D3D11_BUFFER_DESC vBufferDesc;
	memset(&vBufferDesc, 0, sizeof(vBufferDesc));
	vBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vBufferDesc.ByteWidth = sizeof(VERTEX) * static_cast<UINT>(outVertices.size());

	D3D11_SUBRESOURCE_DATA vData;
	vData.pSysMem = outVertices.data();
	m_nrOfVerticesIndexed = indices.size();
	hr = DX::g_device->CreateBuffer(&vBufferDesc, &vData, &m_vertexBufferIndexed);

	// Index buffer
	D3D11_BUFFER_DESC vIndexBufferDesc;
	memset(&vIndexBufferDesc, 0, sizeof(vIndexBufferDesc));
	vIndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	vIndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vIndexBufferDesc.ByteWidth = sizeof(unsigned int) * static_cast<UINT>(indices.size());

	D3D11_SUBRESOURCE_DATA iData;
	iData.pSysMem = indices.data();
	hr = DX::g_device->CreateBuffer(&vIndexBufferDesc, &iData, &m_indexBuffer);
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

ID3D11Buffer * Mesh::getVerticesIndexed() const
{
	return m_vertexBufferIndexed;
}

ID3D11Buffer * Mesh::getVerticesNonIndexed() const
{
	return m_vertexBufferNonIndexed;
}

ID3D11Buffer * Mesh::getIndicies() const
{
	return m_indexBuffer;
}

int Mesh::getNumberOfVerticesIndexed() const
{
	return m_nrOfVerticesIndexed;
}

int Mesh::getNumberOfVerticesNonIndexed() const
{
	return m_nrOfVerticesNonIndexed;
}

bool Mesh::CheckID(const Mesh & other) const
{
	return other.m_id == m_id;
}
