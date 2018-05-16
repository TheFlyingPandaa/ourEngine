#include "OurEngine.h"
#include "PointLight.h"



void PointLight::_init()
{
	m_position.w = 1.0f;
	m_color.w = 0.0f;
}

PointLight::PointLight()
{
	this->setPosition(0.0f, 0.0f, 0.0f);
	this->setColor(1.0f, 1.0f, 1.0f);
	this->setSettingsForLight(1.0f, 0.125f);
	_init();
}

PointLight::~PointLight()
{
	
}

void PointLight::Draw()
{	
	DX::g_lightQueue.push_back(this); 	
}

DirectX::XMFLOAT4A PointLight::getPosition() const
{
	return m_position; 
}

DirectX::XMFLOAT4A PointLight::getColor() const
{
	DirectX::XMFLOAT4A c = m_color;
	c.w = 1.0f;
	return m_color; 
}

DirectX::XMFLOAT4A PointLight::getLightSetup() const
{
	return m_lightSetup; 
}

int PointLight::getIndex() const
{
	return static_cast<int>(m_color.w);
}

void PointLight::setIndex(int index)
{
	m_color.w = static_cast<float>(index);
}

void PointLight::setPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

void PointLight::setColor(float r, float g, float b)
{
	m_color.x = r;
	m_color.y = g;
	m_color.z = b;
}

void PointLight::setSettingsForLight(float range, float IA, float CA, float SIA)
{
	m_lightSetup.x = range;
	m_lightSetup.y = CA;
	m_lightSetup.z = IA;
	m_lightSetup.w = SIA;
}

