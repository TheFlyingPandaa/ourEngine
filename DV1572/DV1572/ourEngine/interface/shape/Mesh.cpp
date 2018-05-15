#include "Mesh.h"
#include "../../core/Dx.h"
#include "../../core/ObjLoader.h"
int Mesh::m_idCounter = 0;
Mesh::Mesh()
{
	m_nrOfVertices = 0;
	m_uniqueID = m_idCounter++;
}

Mesh::Mesh(const std::string & path)
{
	LoadModel(path);
	m_nrOfVertices = 0;
	m_uniqueID = m_idCounter++;
}

Mesh::Mesh(const Mesh & mesh)
{
	m_vertexBuffers = mesh.getVerticesBuffers();
	m_indexBuffers = mesh.getIndicesBuffers();
	m_materials = mesh.getMaterials();
	m_nrOfVerticesPerMaterials = mesh.getNrOfVerticesPerMaterials();
	m_nrOfVertices = mesh.getNrOfVertices();
	m_idCounter = mesh.getIdCounter();
	m_uniqueID = mesh.getUniqueID();

}

Mesh::~Mesh()
{	
	// Sometimes are there copies of the same materials. Better solutions please!

	for (int i = 0; i < m_materials.size(); i++)
	{
		std::vector<int> copiesIndexes;
		bool found = false;
		for (int k = i + 1; k <= m_materials.size() - 1; k++)
		{
			found = m_materials.at(i) == m_materials.at(k);
			if (found)
			{
				copiesIndexes.push_back(k);
			}
		}
		if (copiesIndexes.size() == 0)
		{
			delete m_materials.at(i);
			m_materials.erase(m_materials.begin() + i);
			i = -1;
		}
		else
		{
			delete m_materials.at(i);
			m_materials.erase(m_materials.begin() + i);
			for (int deleteIndex = 0; deleteIndex < copiesIndexes.size(); deleteIndex++)
				m_materials.erase(m_materials.begin() + (copiesIndexes[deleteIndex] - (deleteIndex + 1)));
			i = -1;
		}
	}
	for (auto& vBuffer : m_vertexBuffers)
		vBuffer->Release();
	for (auto& iBuffer : m_indexBuffers)
		iBuffer->Release();
}

void Mesh::LoadModelStr(const std::string & path)
{
	LoadModel(path);
}

void Mesh::LoadModel(const std::string & path)
{
	std::vector<Material*> tempMaterials = DX::getMaterials(path);
	bool* used = new bool[tempMaterials.size()];
	for (int i = 0; i < tempMaterials.size(); i++)
	{
		used[i] = false;
	}
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
			return;
		}
		while (fptr)
		{
			std::vector<VERTEX> tempVertices;
			std::vector<unsigned int> indices;
			std::vector<VERTEX> tempIndexed;
			std::string mttlibName;

			DX::loadOBJContinue(fptr, tempVertices, globalvertices, globalNormals, globalUVs, mttlibName);
			if (mttlibName == "")
				break; //This is happening with faulty OBJs, lol
			DX::CalculateTangents(tempVertices);
			DX::indexVertices(tempVertices, indices, tempIndexed);

			

			for (int i = 0; i < tempMaterials.size(); i ++)
				if (tempMaterials[i]->getName() == mttlibName)
				{
					m_materials.push_back(tempMaterials[i]);
					used[i] = true;
					break;
				}

			// Vertex Buffer Indexed
			ID3D11Buffer* vertexBuffer = nullptr;
			D3D11_BUFFER_DESC vBufferDescIndexed;
			memset(&vBufferDescIndexed, 0, sizeof(vBufferDescIndexed));
			vBufferDescIndexed.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vBufferDescIndexed.Usage = D3D11_USAGE_DEFAULT;
			vBufferDescIndexed.ByteWidth = sizeof(VERTEX) * static_cast<UINT>(tempIndexed.size());

			D3D11_SUBRESOURCE_DATA vDataIndexed;
			vDataIndexed.pSysMem = tempIndexed.data();
			HRESULT hr = DX::g_device->CreateBuffer(&vBufferDescIndexed, &vDataIndexed, &vertexBuffer);
			m_vertexBuffers.push_back(vertexBuffer);

			// Index buffer
			ID3D11Buffer* indexBuffer = nullptr;
			D3D11_BUFFER_DESC vIndexBufferDesc;
			memset(&vIndexBufferDesc, 0, sizeof(vIndexBufferDesc));
			vIndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			vIndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			vIndexBufferDesc.ByteWidth = sizeof(unsigned int) * static_cast<UINT>(indices.size());

			D3D11_SUBRESOURCE_DATA iData;
			iData.pSysMem = indices.data();
			hr = DX::g_device->CreateBuffer(&vIndexBufferDesc, &iData, &indexBuffer);
			m_indexBuffers.push_back(indexBuffer);

			m_nrOfVerticesPerMaterials.push_back(static_cast<int>(indices.size()));			
		}
		
		
	}

	for (int i = 0; i < tempMaterials.size(); i++)
	{
		if (!used[i])
			delete tempMaterials[i];

	}

	delete used;

}

void Mesh::LoadModel(std::vector<VERTEX>& v)
{
	std::vector<VERTEX> indexedVertices;
	std::vector<unsigned int> indices;

	DX::CalculateTangents(v);
	DX::indexVertices(v, indices, indexedVertices);
	
	// Vertex Buffer Indexed
	ID3D11Buffer* vertexBuffer;

	D3D11_BUFFER_DESC vBufferDescIndexed;
	memset(&vBufferDescIndexed, 0, sizeof(vBufferDescIndexed));
	vBufferDescIndexed.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vBufferDescIndexed.Usage = D3D11_USAGE_DEFAULT;
	vBufferDescIndexed.ByteWidth = sizeof(VERTEX) * static_cast<UINT>(indexedVertices.size());

	D3D11_SUBRESOURCE_DATA vDataIndexed;
	vDataIndexed.pSysMem = indexedVertices.data();
	HRESULT hr = DX::g_device->CreateBuffer(&vBufferDescIndexed, &vDataIndexed, &vertexBuffer);
	m_vertexBuffers.push_back(vertexBuffer);

	// Index buffer
	ID3D11Buffer* indexBuffer;
	D3D11_BUFFER_DESC vIndexBufferDesc;
	memset(&vIndexBufferDesc, 0, sizeof(vIndexBufferDesc));
	vIndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	vIndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vIndexBufferDesc.ByteWidth = sizeof(unsigned int) * static_cast<UINT>(indices.size());

	D3D11_SUBRESOURCE_DATA iData;
	iData.pSysMem = indices.data();
	hr = DX::g_device->CreateBuffer(&vIndexBufferDesc, &iData, &indexBuffer);
	m_indexBuffers.push_back(indexBuffer);

	m_nrOfVerticesPerMaterials.push_back(static_cast<int>(indices.size()));
	
}

void Mesh::LoadModelInverted(const std::string & path)
{
	std::vector<Material*> tempMaterials = DX::getMaterials(path);
	
	// Used to remove materials which have been read but not used. Idiot OBJ
	bool* used = new bool[tempMaterials.size()];
	for (int i = 0; i < tempMaterials.size(); i++)
	{
		used[i] = false;
	}

	std::vector<V> globalvertices;
	std::vector<VT> globalUVs;
	std::vector<VN> globalNormals;

	if (tempMaterials.size() == 0)
		m_materials.push_back(new Material());
	
	std::ifstream fptr;
	fptr.open(path);
	if (!fptr)
	{
		std::cout << "Can not open " << path << " dickhead!\n";
		return;
	}
	
	while (fptr)
	{
		std::vector<VERTEX> tempVertices;
		std::vector<unsigned int> indices;
		std::vector<VERTEX> tempIndexed;
		std::string mttlibName;

		DX::loadOBJContinue(fptr, tempVertices, globalvertices, globalNormals, globalUVs, mttlibName, true);
		DX::CalculateTangents(tempVertices);
		DX::indexVertices(tempVertices, indices, tempIndexed);

		for (int i = 0; i < tempMaterials.size(); i++)
			if (tempMaterials[i]->getName() == mttlibName)
			{
				m_materials.push_back(tempMaterials[i]);
				used[i] = true;
				break;
			}



		// Vertex Buffer Indexed
		ID3D11Buffer* vertexBuffer = nullptr;
		D3D11_BUFFER_DESC vBufferDescIndexed;
		memset(&vBufferDescIndexed, 0, sizeof(vBufferDescIndexed));
		vBufferDescIndexed.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vBufferDescIndexed.Usage = D3D11_USAGE_DEFAULT;
		vBufferDescIndexed.ByteWidth = sizeof(VERTEX) * static_cast<UINT>(tempIndexed.size());

		D3D11_SUBRESOURCE_DATA vDataIndexed;
		vDataIndexed.pSysMem = tempIndexed.data();
		HRESULT hr = DX::g_device->CreateBuffer(&vBufferDescIndexed, &vDataIndexed, &vertexBuffer);
		m_vertexBuffers.push_back(vertexBuffer);

		// Index buffer
		ID3D11Buffer* indexBuffer = nullptr;
		D3D11_BUFFER_DESC vIndexBufferDesc;
		memset(&vIndexBufferDesc, 0, sizeof(vIndexBufferDesc));
		vIndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		vIndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vIndexBufferDesc.ByteWidth = sizeof(unsigned int) * static_cast<UINT>(indices.size());

		D3D11_SUBRESOURCE_DATA iData;
		iData.pSysMem = indices.data();
		hr = DX::g_device->CreateBuffer(&vIndexBufferDesc, &iData, &indexBuffer);
		m_indexBuffers.push_back(indexBuffer);

		m_nrOfVerticesPerMaterials.push_back(static_cast<int>(indices.size()));


	}



	for (int i = 0; i < tempMaterials.size(); i++)
	{
		if (!used[i])
			delete tempMaterials[i];

	}

	delete used;
}

void Mesh::MakeRectangle()
{
	m_materials.push_back(new Material());
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
	
	if(DX::loadTexture(path, texture, textureView))
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

std::vector<Material*> Mesh::getMaterials() const
{
	return m_materials;
}

ID3D11Buffer * Mesh::getVertices(int i) const
{
	return m_vertexBuffers[i];
}

std::vector<ID3D11Buffer*> Mesh::getVerticesBuffers() const
{
	return m_vertexBuffers;
}

std::vector<int> Mesh::getNrOfVerticesPerMaterials() const
{
	return m_nrOfVerticesPerMaterials;
}

ID3D11Buffer * Mesh::getIndicesBuffer(int i) const
{
	return m_indexBuffers[i];
}

std::vector<ID3D11Buffer*> Mesh::getIndicesBuffers() const
{
	return m_indexBuffers;
}

int Mesh::getNrOfVertices() const
{
	return m_nrOfVertices;
}

int Mesh::getIdCounter() const
{
	return m_idCounter;
}

int Mesh::getUniqueID() const
{
	return m_uniqueID;
}

int Mesh::getNrOfIndices(int i) const
{
	return m_nrOfVerticesPerMaterials[i];
}

int Mesh::getNumberOfParts() const
{
	return static_cast<int>(m_materials.size());
}

bool Mesh::CheckID(const Mesh& other) const
{
	return other.m_uniqueID == m_uniqueID;
}
