#include "Mesh.h"
#include "../../core/Dx.h"

Mesh::Mesh()
{
	m_vertexBuffer = nullptr;
	m_nrOfVertices = 0;
}

void Mesh::LoadModel(const std::string & path)
{
	std::vector<VERTEX> vertices;

	VERTEX v = {

		0.0f, 0.5f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f
	};
	vertices.push_back(v);

	v = {

		0.5f, -0.5f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f
	};
	vertices.push_back(v);
	
	v = {
		-0.5f, -0.5f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f
	};
	vertices.push_back(v);
	m_nrOfVertices = 3;
	

	// Vertex Buffer
	D3D11_BUFFER_DESC vBufferDesc;
	memset(&vBufferDesc, 0, sizeof(vBufferDesc));
	vBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vBufferDesc.ByteWidth = sizeof(VERTEX) * static_cast<UINT>(vertices.size());

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

ID3D11Buffer * Mesh::getVertices() const
{
	return m_vertexBuffer;
}

int Mesh::getNumberOfVertices() const
{
	return m_nrOfVertices;
}
