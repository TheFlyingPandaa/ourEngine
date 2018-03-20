#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#include <string>
#include <vector>
#include "../../Structs.h"

class Mesh
{
private:
	ID3D11Buffer* m_vertexBuffer;
	ID3D11VertexShader* m_vs;
	ID3D11GeometryShader* m_gs;
	ID3D11PixelShader* m_ps;

public:
	Mesh();
	void LoadModel(const std::string &path);
	ID3D11Buffer* getVertices() const;
};