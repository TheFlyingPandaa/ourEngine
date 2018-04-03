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
	ID3D11Buffer *		m_vertexBufferNonIndexed;
	ID3D11Buffer*		m_vertexBufferIndexed;
	ID3D11Buffer*		m_indexBuffer;
	Material			m_material;
	int					m_nrOfVerticesIndexed;
	int					m_nrOfVerticesNonIndexed;
public:
	Mesh();
	void LoadModel(const std::string &path);
	void LoadModel(const std::vector<VERTEX> &v);
	void setDiffuseTexture(const std::string &path);
	void setNormalTexture(const std::string &path);
	void setHighlightTexture(const std::string &path);
	
	Material* getMaterial();

	ID3D11Buffer* getVerticesIndexed() const;
	ID3D11Buffer* getVerticesNonIndexed() const;
	ID3D11Buffer* getIndicies() const;
	int getNumberOfVerticesIndexed() const;
	int getNumberOfVerticesNonIndexed() const;
};