#include "Mesh.h"
#include "../../core/Dx.h"
#include "../../core/ObjLoader.h"
int Mesh::m_idCounter = 0;
Mesh::Mesh()
{
	m_nrOfVertices = 0;
	m_uniqueID = m_idCounter++;
}

Mesh::~Mesh()
{	
	for (auto& vBuffer : m_vertexBuffers)
		vBuffer.buffer->Release();
	for (auto& iBuffer : m_indexBuffers)
		iBuffer.buffer->Release();
}

void Mesh::LoadModel(const std::string & path)
{
	std::vector<Material*> tempMaterials = DX::getMaterials(path);

	std::vector<V> globalvertices;
	std::vector<VT> globalUVs;
	std::vector<VN> globalNormals;
	
	

	if (tempMaterials.size() >= 1)
	{
		std::ifstream fptr;
		fptr.open(path);
		if (!fptr)
		{
			std::cout << "Can not open " << path << " dickhead!\n";
		}
		std::cout << "\nMesh: Reading OBJ: " << path << std::endl;
		while (fptr)
		{
			std::vector<VERTEX> tempVertices;
			std::vector<unsigned int> indices;
			std::vector<VERTEX> tempIndexed;
			std::string mttlibName;

			DX::loadOBJContinue(fptr, tempVertices, globalvertices, globalNormals, globalUVs, mttlibName);
			DX::CalculateTangents(tempVertices);
			DX::indexVertices(tempVertices, indices, tempIndexed);
			if (mttlibName == "") break;
			for (auto& material : tempMaterials)
				if (material->getName() == mttlibName)
				{
					m_materials.push_back(material);
					break;
				}

			m_vertexBuffers.push_back(Glass());
			m_indexBuffers.push_back(Glass());

			// Vertex Buffer Indexed
			D3D11_BUFFER_DESC vBufferDescIndexed;
			memset(&vBufferDescIndexed, 0, sizeof(vBufferDescIndexed));
			vBufferDescIndexed.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vBufferDescIndexed.Usage = D3D11_USAGE_DEFAULT;
			vBufferDescIndexed.ByteWidth = sizeof(VERTEX) * static_cast<UINT>(tempIndexed.size());

			D3D11_SUBRESOURCE_DATA vDataIndexed;
			vDataIndexed.pSysMem = tempIndexed.data();
			HRESULT hr = DX::g_device->CreateBuffer(&vBufferDescIndexed, &vDataIndexed, &m_vertexBuffers.back().buffer);

			// Index buffer
			D3D11_BUFFER_DESC vIndexBufferDesc;
			memset(&vIndexBufferDesc, 0, sizeof(vIndexBufferDesc));
			vIndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			vIndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			vIndexBufferDesc.ByteWidth = sizeof(unsigned int) * static_cast<UINT>(indices.size());

			D3D11_SUBRESOURCE_DATA iData;
			iData.pSysMem = indices.data();
			hr = DX::g_device->CreateBuffer(&vIndexBufferDesc, &iData, &m_indexBuffers.back().buffer);

			m_nrOfVerticesPerMaterials.push_back(static_cast<int>(indices.size()));

			std::cout << "MTL: " << mttlibName << std::endl;
			std::cout << "\tVertices: " << tempVertices.size() << std::endl;
			std::cout << "\tVertices(i): " << tempIndexed.size() << std::endl;
			std::cout << "\tIndices(i): " << indices.size() << std::endl;
			
		}
		
	}



	

	//m_nrOfVertices = static_cast<int>(indices.size());
}

void Mesh::LoadModel(std::vector<VERTEX>& v)
{
	std::vector<VERTEX> indexedVertices;
	std::vector<unsigned int> indices;
	DX::CalculateTangents(v);
	DX::indexVertices(v, indices, indexedVertices);

	m_vertexBuffers.push_back(Glass());
	m_indexBuffers.push_back(Glass());
	// Vertex Buffer Indexed
	D3D11_BUFFER_DESC vBufferDescIndexed;
	memset(&vBufferDescIndexed, 0, sizeof(vBufferDescIndexed));
	vBufferDescIndexed.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vBufferDescIndexed.Usage = D3D11_USAGE_DEFAULT;
	vBufferDescIndexed.ByteWidth = sizeof(VERTEX) * static_cast<UINT>(indexedVertices.size());

	D3D11_SUBRESOURCE_DATA vDataIndexed;
	vDataIndexed.pSysMem = indexedVertices.data();
	HRESULT hr = DX::g_device->CreateBuffer(&vBufferDescIndexed, &vDataIndexed, &m_vertexBuffers[0].buffer);

	// Index buffer
	D3D11_BUFFER_DESC vIndexBufferDesc;
	memset(&vIndexBufferDesc, 0, sizeof(vIndexBufferDesc));
	vIndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	vIndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vIndexBufferDesc.ByteWidth = sizeof(unsigned int) * static_cast<UINT>(indices.size());

	D3D11_SUBRESOURCE_DATA iData;
	iData.pSysMem = indices.data();
	hr = DX::g_device->CreateBuffer(&vIndexBufferDesc, &iData, &m_indexBuffers[0].buffer);

	m_nrOfVertices = static_cast<int>(indices.size());
}

void Mesh::LoadModelInverted(const std::string & path)
{
	std::vector<VERTEX> vertices;
	std::vector<VERTEX> indexedVertices;
	std::vector<unsigned int> indices;
	DX::loadOBJInvert(path, vertices);
	DX::CalculateTangents(vertices);
	DX::indexVertices(vertices, indices, indexedVertices);

	m_vertexBuffers.push_back(Glass());
	m_indexBuffers.push_back(Glass());
	// Vertex Buffer Indexed
	D3D11_BUFFER_DESC vBufferDescIndexed;
	memset(&vBufferDescIndexed, 0, sizeof(vBufferDescIndexed));
	vBufferDescIndexed.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vBufferDescIndexed.Usage = D3D11_USAGE_DEFAULT;
	vBufferDescIndexed.ByteWidth = sizeof(VERTEX) * static_cast<UINT>(indexedVertices.size());

	D3D11_SUBRESOURCE_DATA vDataIndexed;
	vDataIndexed.pSysMem = indexedVertices.data();
	HRESULT hr = DX::g_device->CreateBuffer(&vBufferDescIndexed, &vDataIndexed, &m_vertexBuffers[0].buffer);

	// Index buffer
	D3D11_BUFFER_DESC vIndexBufferDesc;
	memset(&vIndexBufferDesc, 0, sizeof(vIndexBufferDesc));
	vIndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	vIndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vIndexBufferDesc.ByteWidth = sizeof(unsigned int) * static_cast<UINT>(indices.size());

	D3D11_SUBRESOURCE_DATA iData;
	iData.pSysMem = indices.data();
	hr = DX::g_device->CreateBuffer(&vIndexBufferDesc, &iData, &m_indexBuffers[0].buffer);

	m_nrOfVertices = static_cast<int>(indices.size());
}

void Mesh::MakeRectangle()
{
	std::vector<VERTEX> vertices;

	VERTEX v = {

		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, 0.0f
	};
	vertices.push_back(v);

	v = {

		0.0f, 0.5f, 0.0f,
		0.0f, 1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, 0.0f
	};
	vertices.push_back(v);

	v = {
		0.5f, 0.5f, 0.0f,
		1.0f, 1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, 0.0f
	};
	vertices.push_back(v);

	v = {
		0.5f, 0.5f, 0.0f,
		1.0f, 1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, 0.0f
	};
	vertices.push_back(v);

	v = {
		0.5f, 0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, 0.0f
	};
	vertices.push_back(v);

	v = {

		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, 0.0f
	};
	vertices.push_back(v);

	this->LoadModel(vertices);
}

void Mesh::setDiffuseTexture(const std::string& path)
{
	ID3D11Resource* texture;
	ID3D11ShaderResourceView* textureView;
	DX::loadTexture(path, texture, textureView);
	m_materials.back()->setDiffuseMap(textureView, texture);
}

void Mesh::setNormalTexture(const std::string & path)
{
	ID3D11Resource* texture;
	ID3D11ShaderResourceView* textureView;
	DX::loadTexture(path, texture, textureView);
	m_materials.back()->setNormalMap(textureView, texture);
}

void Mesh::setHighlightTexture(const std::string & path)
{
	ID3D11Resource* texture;
	ID3D11ShaderResourceView* textureView;
	DX::loadTexture(path, texture, textureView);
	m_materials.back()->setHighlightMap(textureView, texture);
}

Material* Mesh::getMaterial(int i)
{
	return m_materials[i];
}

ID3D11Buffer * Mesh::getVertices(int i) const
{
	return m_vertexBuffers[i].buffer;
}

ID3D11Buffer * Mesh::getIndicesBuffer(int i) const
{
	return m_indexBuffers[i].buffer;
}

int Mesh::getNrOfIndices(int i) const
{
	return m_nrOfVerticesPerMaterials[i];
}

int Mesh::getNumberOfParts() const
{
	return m_materials.size();
}

bool Mesh::CheckID(const Mesh& other) const
{
	return other.m_uniqueID == m_uniqueID;
}
