#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <vector>
#include <DirectXMath.h>
#include "WICTextureLoader.h"
#include <fstream>
#include "FroggeDSA.h"
#include "QuadTree.h"
#include "Application.h"

#define SCREEN_DEPTH 15.f
#define SCREEN_NEAR 0.01f
#define TOP_DOWN_DEPTH 1000.f
#define TOP_DOWN_WIDTH 300
#define TOP_DOWN_HEIGHT 300
#define TOP_DOWN_X 980
#define TOP_DOWN_Y 0
#define UPDATE_CALL_TIME 30.0

using namespace DirectX;
using namespace std;

class Application;

class HeightMap
{
public:
	HeightMap();
	~HeightMap();

	bool initialize(Application *pApp, HWND hwnd, int mapSize, float offset);
	void update();
	void Release();
	void Render(Application * pApp, XMMATRIX world, XMMATRIX view, XMMATRIX projection);
	int getNrOfTriangles() const;

private:

	struct MatrixBufferHeightMap
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};
	//members
	int m_mapSize;  //this is the maps size (the size of one side, nr of vertices is then size times size)
	float m_offset; //this offset determines how far vertices are apart from each other

	ViewFrustum m_viewFrustum;
	QuadTree m_quadTree;

	vector<float> m_heightValues;
	vector<VertexTypeHeightMap> m_vertices;
	vector<unsigned int> m_indices;

	ID3D11Buffer * m_vertexBuffer;
	ID3D11Buffer * m_MatrixBuffer;

	ID3D11InputLayout * m_InputLayout;
	ID3D11VertexShader * m_VertexShader;
	ID3D11PixelShader * m_PixelShader;

	ID3D11Resource * m_textureResource;
	ID3D11Resource * m_normalResource;

	ID3D11ShaderResourceView * m_colorTexture;
	ID3D11ShaderResourceView * m_normalMap;
	ID3D11SamplerState * m_Sampler;

	ID3D11RasterizerState * m_rasterState; //this is soley for wireframe drawing
	D3D11_VIEWPORT m_Viewport;
	D3D11_VIEWPORT m_TopDownView;

	XMMATRIX m_top_down_view;
	XMMATRIX m_top_down_projection;

	Sphere m_sphere;
	high_resolution_clock::time_point start_time, current_time;

	//methods
	bool initShaders(ID3D11Device * pDev, HWND hwnd, WCHAR * vtx_path, WCHAR * px_path);
	bool initTextures(ID3D11Device * pDev, WCHAR * colorTex, WCHAR * normalTex);
	bool initRasterizer(ID3D11Device * pDev);
	bool setShaderParameters(ID3D11DeviceContext * pDevCon, XMMATRIX world, XMMATRIX view, XMMATRIX projection);

	void generateHeightValues();
	bool buildHeightMap(ID3D11Device * pDev);
	void calculateNormalTangentBinormal(int vtx1, int vtx2, int vtx3);
	void constructViewProjectionMatrices(Application *pApp);

	void outputErrorMessage(ID3D10Blob * error, HWND hwnd, WCHAR * file);

};