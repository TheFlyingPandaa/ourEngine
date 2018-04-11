#ifndef POINTLIGHT_H 
#define POINTLIGHT_H
#include "../../Structs.h"
#include <DirectXMath.h>
#include <vector>

class PointLight
{
private:
	POINT_LIGHT_BUFFER m_pointLightBuffer; 
public:
	PointLight(DirectX::XMFLOAT4A position, DirectX::XMFLOAT4A color); 
	~PointLight(); 
	POINT_LIGHT_BUFFER& getBuffer(); 
	void addToLightQueue(std::vector<PointLight>& lightQueue); 
};
#endif
