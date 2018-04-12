#ifndef POINTLIGHT_H 
#define POINTLIGHT_H
#include "../../Structs.h"
#include <DirectXMath.h>
#include <vector>

class PointLight
{
private:
	DirectX::XMFLOAT4A m_position; 
	DirectX::XMFLOAT4A m_color;
	DirectX::XMFLOAT4A m_lightSetup; 

public:
	PointLight(DirectX::XMFLOAT4A position, DirectX::XMFLOAT4A color, DirectX::XMFLOAT4A lightSetup); 
	~PointLight();  
	
	void addToLightQueue(); 

	DirectX::XMFLOAT4A getPosition(); 
	DirectX::XMFLOAT4A getColor();
	DirectX::XMFLOAT4A getLightSetup(); 
};
#endif
