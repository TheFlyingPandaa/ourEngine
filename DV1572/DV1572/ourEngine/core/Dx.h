#pragma once
#include "ShaderCreator.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")


/*
	This klass needs to be included in the corrisbonding c++ files to be used.
	The globals in this klass is not really globals, but rather semi globals.
	This klass may be reached within the engine
*/
namespace DX
{
	
	extern ID3D11Device* g_device;
	extern ID3D11DeviceContext* g_deviceContext;

	extern ID3D11VertexShader* g_vertexShader;
	extern ID3D11PixelShader* g_pixelShader;
	extern ID3D11InputLayout* g_inputLayout;


	extern void CleanUp();
}
