#include "GameTime.h"
#include <DirectXMath.h>
#include <iostream>
#define PI 3.14159265358979323846f 

GameTime::GameTime()
{
	m_currentClockValue = 0.0f;
	m_currentTime = MORNINGTONOON; 
	m_minutes = 0;
	m_seconds = 0.0f; 
	m_totalSeconds = 0; 
	m_divider = 255.0f; 
	
	m_colorScaleFactor = 0.0001f;
	m_rotationFactor = 0.0001f;
	
	m_sunStartInterpolate = DirectX::XMVECTOR{ 182.0f / m_divider,126.0f / m_divider,91.0f / m_divider };
	m_sunTargetInterpolate = DirectX::XMVECTOR{ 0,0,0 };
	
	m_sunCurrentFinalColor = DirectX::XMVECTOR{ 0,0,0 }; 

	m_sunRotationStart = DirectX::XMVECTOR{ 0,1,0 };
	m_sunRotationTarget = DirectX::XMVECTOR{ 0,0,0 };

	DirectX::XMFLOAT3 tempPos = { 10.0f,0.0f,0.0 };
	m_sunPos = XMLoadFloat3(&tempPos);
	//DirectX::XMStoreFloat3(&tempPos, m_sunPos);
	m_sunAngle = 0.0f;
	
	
	m_createSunBuffer(); 

	m_sun.InitDirectional(
		DirectX::XMFLOAT4A(0, 100, 0, 0), 
		DirectX::XMFLOAT4A(-1, -1, -1, 0), 
		DirectX::XMFLOAT4A(1, 1, 1, 1), 420, 420);

	m_vUp = XMVECTOR{ 0,1,0 }; 
}

GameTime::~GameTime()
{
}

void GameTime::updateCurrentTime(float refreshRate)
{
	using namespace DirectX;
	m_currentClockValue = 1.0f;// (1.0f / refreshRate);
	m_seconds += m_currentClockValue; 
	
	switch (m_currentTime)
	{
		//Fr�n 06:00 -> 12:00
	case MORNINGTONOON:
		if (m_colorScaleFactor < 1.0f)
		{

			m_sunTargetInterpolate = DirectX::XMVECTOR{ 192.0f / m_divider,191.0f / m_divider, 173.0f / m_divider };
			m_colorScaleFactor = m_seconds / (6.0f * 60.0f);
			m_sunCurrentFinalColor = DirectX::XMVectorLerp(m_sunStartInterpolate, m_sunTargetInterpolate, m_colorScaleFactor);
			DirectX::XMStoreFloat4A(&m_fFinalColor, m_sunCurrentFinalColor);

			m_sunAngleTarget = 90.0f;
			XMFLOAT3 startAngle = { m_sunAngle, m_sunAngle, m_sunAngle };
			XMFLOAT3 targetAngle = { m_sunAngleTarget,m_sunAngleTarget ,m_sunAngleTarget };
			m_currentAngle = XMVectorGetX(XMVectorLerp(XMLoadFloat3(&startAngle), XMLoadFloat3(&targetAngle), m_colorScaleFactor));


		}
		else
		{
			m_seconds = 0;
			m_currentTime = NOONTOEVENING;
			m_sunStartInterpolate = m_sunCurrentFinalColor;
			m_sunRotationStart = m_sunFinalRotation;
			m_colorScaleFactor = 0.0f;
			m_sunAngle = 90.0f;
		}
		break;

		//Fr�n 12:00 -> 17:00
	case NOONTOEVENING:
		if (m_colorScaleFactor < 1.0f)
		{
			//std::cout << "Noon to Evening" << std::endl;

			m_sunTargetInterpolate = DirectX::XMVECTOR{ 230.0f / m_divider,120.0f / m_divider, 120.0f / m_divider };
			m_colorScaleFactor = m_seconds / (6.0f * 60.0f);
			m_sunCurrentFinalColor = DirectX::XMVectorLerp(m_sunStartInterpolate, m_sunTargetInterpolate, m_colorScaleFactor);
			DirectX::XMStoreFloat4A(&m_fFinalColor, m_sunCurrentFinalColor);

			m_sunAngleTarget = 135.0f;
			XMFLOAT3 startAngle = { m_sunAngle, m_sunAngle, m_sunAngle };
			XMFLOAT3 targetAngle = { m_sunAngleTarget,m_sunAngleTarget ,m_sunAngleTarget };
			m_currentAngle = XMVectorGetX(XMVectorLerp(XMLoadFloat3(&startAngle), XMLoadFloat3(&targetAngle), m_colorScaleFactor));


		}
		else
		{
			m_seconds = 0;
			m_currentTime = EVENINGTONIGHT;
			m_sunStartInterpolate = m_sunCurrentFinalColor;
			m_sunRotationStart = m_sunFinalRotation;
			m_sunAngle = 135.0f;
			m_colorScaleFactor = 0.0f;
		}
		break;
		//Fr�n 17:00 -> 21:00
	case EVENINGTONIGHT:
		if (m_colorScaleFactor < 1.0f)
		{
			//std::cout << "Evening to Night" << std::endl;
			m_sunTargetInterpolate = DirectX::XMVECTOR{ 90.0f / m_divider,70.0f / m_divider, 190.0f / m_divider };
			m_colorScaleFactor = m_seconds / (6.0f * 60.0f);
			m_sunCurrentFinalColor = DirectX::XMVectorLerp(m_sunStartInterpolate, m_sunTargetInterpolate, m_colorScaleFactor);
			DirectX::XMStoreFloat4A(&m_fFinalColor, m_sunCurrentFinalColor);

			m_sunAngleTarget = 180.0f;
			XMFLOAT3 startAngle = { m_sunAngle, m_sunAngle, m_sunAngle };
			XMFLOAT3 targetAngle = { m_sunAngleTarget,m_sunAngleTarget ,m_sunAngleTarget };
			m_currentAngle = XMVectorGetX(XMVectorLerp(XMLoadFloat3(&startAngle), XMLoadFloat3(&targetAngle), m_colorScaleFactor));

		}
		else
		{

			m_seconds = 0;
			m_currentTime = MORNINGTONOON;
			m_sunStartInterpolate = DirectX::XMVECTOR{ 182.0f / m_divider,126.0f / m_divider,91.0f / m_divider };

			m_totalSeconds = 0;
			m_minutes = 0;
			m_sunAngle = 0.0f;
			m_colorScaleFactor = 0.0f;

		}
		break;
	}
		
}

GameTime::TIMEOFDAY GameTime::getTimePeriod()
{
	return m_currentTime;
}

Light & GameTime::getSun()
{
	return m_sun; 
}

void GameTime::m_createSunBuffer()
{
	HRESULT hr; 
	D3D11_BUFFER_DESC sunBdesc;
	sunBdesc.Usage = D3D11_USAGE_DYNAMIC;
	sunBdesc.ByteWidth = sizeof(DIRECTIONAL_LIGHT_BUFFER);
	sunBdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	sunBdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	sunBdesc.MiscFlags = 0;
	sunBdesc.StructureByteStride = 0;

	hr = DX::g_device->CreateBuffer(&sunBdesc, nullptr, &m_pSunBuffer); 
}

void GameTime::m_cpyLightToGPU()
{
	
	m_sun.setColor(m_fFinalColor);
	m_sunBuffer.color = m_sun.getColor(); 
		
	XMMATRIX rot = XMMatrixRotationZ(XMConvertToRadians(m_currentAngle));
		
	XMVECTOR newpos = XMVector3Transform(m_sunPos, rot);
	XMFLOAT4A sunPos4f;

	XMStoreFloat4A(&sunPos4f, newpos);
	sunPos4f.w = 1.0f;

	m_sunBuffer.pos = sunPos4f;

	m_sunBuffer.dir.x = -m_sunBuffer.pos.x;
	m_sunBuffer.dir.y = -m_sunBuffer.pos.y;
	m_sunBuffer.dir.z = -m_sunBuffer.pos.z;
	m_sunBuffer.dir.w = m_sunBuffer.pos.w;

	std::cout << "\rAngle " << m_currentAngle << std::flush;
	/*std::cout << "Position (" << m_sunBuffer.pos.x << "," << m_sunBuffer.pos.y << "," << m_sunBuffer.pos.z << ")\n";
	std::cout << "Direciton (" << m_sunBuffer.dir.x << "," << m_sunBuffer.dir.y << "," << m_sunBuffer.dir.z << ")\n\n";*/

	m_sun.cpyDataDir(m_sunBuffer,m_pSunBuffer); 
}
