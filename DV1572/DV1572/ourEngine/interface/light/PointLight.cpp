#include "PointLight.h"


PointLight::PointLight(DirectX::XMFLOAT4A position, DirectX::XMFLOAT4A color, DirectX::XMFLOAT4A lightSetup)
{
	m_position = position; 
	m_color = color; 
	m_lightSetup = lightSetup; 
}

PointLight::~PointLight()
{
}

void PointLight::addToLightQueue(std::vector<PointLight>& lightQueue)
{
	lightQueue.push_back(*this); 
}

DirectX::XMFLOAT4A PointLight::getPosition()
{
	return m_position; 
}

DirectX::XMFLOAT4A PointLight::getColor()
{
	return m_color; 
}

DirectX::XMFLOAT4A PointLight::getLightSetup()
{
	return m_lightSetup; 
}

