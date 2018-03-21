#pragma once
#include "Dx.h"
#include <vector>

/*
	Example of use:

	ShaderInfo shaders[] =
	{
	{ VERTEXSHADER, L"ourEngine/shaders/testVertex.hlsl", "vs_5_0" },
	{ PIXELSHADER, L"ourEngine/shaders/testPixel.hlsl", "ps_5_0" },
	{ NONE }
	};

	std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc;
	inputDesc.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });

	m_shader = new Shader(shaders, inputDesc);

	---Before using you must bind the shader, which enables it---
	
	m_shader->bind();


*/

enum ShaderType
{
	VERTEXSHADER,
	HULLSHADER,
	DOMAINSHADER,
	GEOMETRYSHADER,
	PIXELSHADER,
	NONE
};

struct ShaderInfo
{
	ShaderType type; // Type of shader
	LPCWSTR source; // Located on HDD
	LPCSTR pTarget; // Set to 5_0 and correct prefix
};

class Shader
{

private:

	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11HullShader> m_hullShader;
	Microsoft::WRL::ComPtr<ID3D11DomainShader> m_domainShader;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

public:
	Shader(ShaderInfo* shaderInfo, std::vector<D3D11_INPUT_ELEMENT_DESC> desc);

	void Bind();
private:
	void _compileShaders(ShaderInfo* shaders, std::vector<D3D11_INPUT_ELEMENT_DESC> desc);

	
};