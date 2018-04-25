#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#include <string>
#include <vector>
#include "../../Structs.h"
#include "Material.h"

class Mesh
{
private:
	std::vector<ID3D11Buffer*>				m_vertexBuffers;
	std::vector<ID3D11Buffer*>				m_indexBuffers;
	std::vector<Material*>					m_materials;
	std::vector<int>						m_nrOfVerticesPerMaterials;
	int										m_nrOfVertices;
	static int								m_idCounter;
	int										m_uniqueID;
public:
	Mesh();
	Mesh(const std::string& path);
	Mesh(const Mesh& mesh);
	
	~Mesh();
	void LoadModel(const std::string &path);
	void LoadModel(std::vector<VERTEX> &v);
	void LoadModelInverted(const std::string &path);
	void MakeRectangle();
	void setDiffuseTexture(const std::string &path);
	void setNormalTexture(const std::string &path);
	void setHighlightTexture(const std::string &path);
	
	Material* getMaterial(int i = 0);
	std::vector<Material*> getMaterials() const;

	ID3D11Buffer*				getVertices(int i = 0) const;
	std::vector<ID3D11Buffer*>	getVerticesBuffers()const;
	std::vector<int>			getNrOfVerticesPerMaterials() const;

	ID3D11Buffer* getIndicesBuffer(int i = 0) const;	//But you only get 1 buffer :c
	std::vector<ID3D11Buffer*> getIndicesBuffers() const;

	int getNrOfVertices() const;
	int getIdCounter() const;
	int getUniqueID() const;

	int getNrOfIndices(int i = 0) const;
	int getNumberOfParts() const;

	bool CheckID(const Mesh& other) const;
};