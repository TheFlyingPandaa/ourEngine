#pragma once
#include "ShaderCreator.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include "../interface/Text.h"
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#include "../interface/shape/Shape.h"
#include "../interface/light/PointLight.h" 
#include <vector>
#include "../Structs.h"
#include "../../ourEngine/core/Font/SpriteBatch.h"
#include "../../ourEngine/core/Font/SpriteFont.h"

/*
	This klass needs to be included in the corrisbonding c++ files to be used.
	The globals in this klass is not really globals, but rather semi globals.
	This klass may be reached within the engine
*/
namespace DX
{
	
	extern ID3D11Device* g_device;
	extern ID3D11DeviceContext* g_deviceContext;

	extern std::vector<Shape*> g_renderQueue;
	extern std::vector<Shape*> g_skyBoxQueue;
	extern std::vector<Shape*> g_shadowQueue;
	extern std::vector<Shape*> g_transQueue;
	extern std::vector<Shape*> g_pickingQueue;
	extern std::vector<Shape*> g_HUDQueue;
	extern std::vector<PointLight*> g_lightQueue; 
	
	// InstanceRender
	struct INSTANCE_ATTRIB
	{
		DirectX::XMFLOAT4A w1;
		DirectX::XMFLOAT4A w2;
		DirectX::XMFLOAT4A w3;
		DirectX::XMFLOAT4A w4;

		DirectX::XMFLOAT4A highLightColor;
	};

	struct INSTANCE_GROUP
	{
		std::vector<INSTANCE_ATTRIB> attribs;
		Shape* shape;
	};
	struct INSTANCE_GROUP_INDEXED
	{
		std::vector<INSTANCE_ATTRIB> attribs;
		Shape* shape;
		std::vector<long> index;
	};


	extern ID3D11VertexShader* g_3DVertexShader;
	extern ID3D11PixelShader* g_3DPixelShader;
	extern ID3D11InputLayout* g_inputLayout;

	extern std::vector<INSTANCE_GROUP> g_instanceGroups;
	extern std::vector<INSTANCE_GROUP> g_instanceGroupsSkyBox;
	extern std::vector<INSTANCE_GROUP> g_instanceGroupsHUD;
	extern std::vector<INSTANCE_GROUP> g_instanceGroupsTransparancy;
	extern std::vector<INSTANCE_GROUP_INDEXED> g_instanceGroupsPicking;
	extern std::vector<INSTANCE_GROUP>	g_InstanceGroupsShadow;

	extern void submitToInstance(Shape* shape, std::vector<INSTANCE_GROUP>& queue);
	extern void submitToInstance(Shape* shape, std::vector<INSTANCE_GROUP_INDEXED>& queue);

	extern DirectX::XMFLOAT4A g_lightPos;
	extern DirectX::XMFLOAT4A g_lightDir;

	//Tesselltion
	extern ID3D11HullShader* g_standardHullShader;
	extern ID3D11DomainShader* g_standardDomainShader;

	//SkyBox
	extern ID3D11VertexShader*	g_skyBoxVertexShader;
	extern ID3D11PixelShader*	g_skyBoxPixelShader;

	// Font
	
	extern std::vector<std::unique_ptr<DirectX::SpriteFont>> g_fonts;
	extern std::vector<Text*> g_textQueue;
	extern std::unique_ptr<DirectX::SpriteBatch> g_spriteBatch;
	


	extern void CleanUp();
	
}
