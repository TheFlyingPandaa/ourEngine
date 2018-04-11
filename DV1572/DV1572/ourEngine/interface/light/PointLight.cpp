#include "PointLight.h"


PointLight::PointLight(DirectX::XMFLOAT4A position, DirectX::XMFLOAT4A color)
{
	m_pointLightBuffer.pos = position; 
	m_pointLightBuffer.color = color; 
}

PointLight::~PointLight()
{
}

POINT_LIGHT_BUFFER & PointLight::getBuffer()
{
	return m_pointLightBuffer; 
}

void PointLight::addToLightQueue(std::vector<PointLight>& lightQueue)
{
	lightQueue.push_back(*this); 
}
