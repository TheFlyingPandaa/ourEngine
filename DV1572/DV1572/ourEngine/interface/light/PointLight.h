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
	int roomIndex;
	void _init();
public:
	PointLight(); 
	~PointLight();  
	
	void addToLightQueue(); 

	DirectX::XMFLOAT4A	getPosition(); 
	DirectX::XMFLOAT4A	getColor();
	DirectX::XMFLOAT4A	getLightSetup(); 
	void				setPosition(float x, float y, float z);
	void				setColor(float r, float g, float b);
	// Range, Inverse Attenuation, Constant Attenuation, Square Inverse Attenuation
	void				setSettingsForLight(float range, float IA, float CA = 0.0f, float SIA = 0.0f);
};
#endif
