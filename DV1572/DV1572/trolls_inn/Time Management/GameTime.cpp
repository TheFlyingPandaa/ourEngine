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
	m_currentClockValue = 1.0f;// (1.0f / refreshRate);
	m_seconds += m_currentClockValue; 
	
	if (m_seconds >= 1.0f)
	{
		m_totalSeconds++;
		m_minutes = m_totalSeconds / 60;
	}
	
	switch (m_currentTime)
	{
	//Från 06:00 -> 12:00
	case MORNINGTONOON:
		if (m_minutes <= 6) 
		{
			//std::cout << "Morning to Noon" << std::endl;
			m_sunTargetInterpolate = DirectX::XMVECTOR{ 192.0f / m_divider,191.0f / m_divider, 173.0f / m_divider };
			m_colorScaleFactor = m_seconds / (6.0f * 60.0f); 
			m_sunCurrentFinalColor = DirectX::XMVectorLerp(m_sunStartInterpolate, m_sunTargetInterpolate, m_colorScaleFactor); 
			DirectX::XMStoreFloat4A(&m_fFinalColor, m_sunCurrentFinalColor); 
 
			m_sunRotationTarget = DirectX::XMVECTOR{ 0, -1, 0 };
			m_rotationFactor = m_colorScaleFactor; 
			m_sunFinalRotation = DirectX::XMVectorLerp(m_sunRotationStart, m_sunRotationTarget, m_rotationFactor); 
			DirectX::XMVector3Normalize(m_sunFinalRotation);
			XMStoreFloat4A(&m_fFinalRotation, m_sunFinalRotation); 
		}
		else
		{
			m_seconds = 0; 
			m_currentTime = NOONTOEVENING; 
			m_sunStartInterpolate = m_sunCurrentFinalColor;
			m_sunRotationStart = m_sunFinalRotation; 
		}
		break; 
	
	//Från 12:00 -> 17:00
	case NOONTOEVENING:
		if (m_minutes <= 12) 
		{
			//std::cout << "Noon to Evening" << std::endl;

			m_sunTargetInterpolate = DirectX::XMVECTOR{ 230.0f / m_divider,120.0f / m_divider, 120.0f / m_divider };
			m_colorScaleFactor = m_seconds / (6.0f * 60.0f);
			m_sunCurrentFinalColor = DirectX::XMVectorLerp(m_sunStartInterpolate, m_sunTargetInterpolate, m_colorScaleFactor);
			DirectX::XMStoreFloat4A(&m_fFinalColor, m_sunCurrentFinalColor);

			//m_sunRotationTarget = DirectX::XMVECTOR{ -0.5f, -0.3f, 0 };
			m_sunRotationTarget = DirectX::XMVECTOR{ -0.5f, -0.5f, 0 };
			m_rotationFactor = m_colorScaleFactor;
			m_sunFinalRotation = DirectX::XMVectorLerp(m_sunRotationStart, m_sunRotationTarget, m_rotationFactor);
			DirectX::XMVector3Normalize(m_sunFinalRotation); 
			XMStoreFloat4A(&m_fFinalRotation, m_sunFinalRotation);
		}
		else
		{
			m_seconds = 0;
			m_currentTime = EVENINGTONIGHT;
			m_sunStartInterpolate = m_sunCurrentFinalColor;
			m_sunRotationStart = m_sunFinalRotation;
		}
		break; 
	//Från 17:00 -> 21:00
	case EVENINGTONIGHT:
		if (m_minutes <= 18) 
		{
			//std::cout << "Evening to Night" << std::endl;
			m_sunTargetInterpolate = DirectX::XMVECTOR{ 90.0f / m_divider,70.0f / m_divider, 190.0f / m_divider };
			m_colorScaleFactor = m_seconds / (6.0f * 60.0f);
			m_sunCurrentFinalColor = DirectX::XMVectorLerp(m_sunStartInterpolate, m_sunTargetInterpolate, m_colorScaleFactor);
			DirectX::XMStoreFloat4A(&m_fFinalColor, m_sunCurrentFinalColor);

			//m_sunRotationTarget = DirectX::XMVECTOR{ 1.0f, 0.0f, 0.0f };
			m_sunRotationTarget = DirectX::XMVECTOR{ -1.0f, 0.0f, 0.0f };
			m_rotationFactor = m_colorScaleFactor;
			m_sunFinalRotation = DirectX::XMVectorLerp(m_sunRotationStart, m_sunRotationTarget, m_rotationFactor);
			DirectX::XMVector3Normalize(m_sunFinalRotation);
			XMStoreFloat4A(&m_fFinalRotation, m_sunFinalRotation);
		}
		else
		{
			m_seconds = 0;
			m_currentTime = NIGHTTOMID;
			m_sunStartInterpolate = m_sunCurrentFinalColor;
			m_sunRotationStart = m_sunFinalRotation; 
		}
		break;
	//Från 21:00 -> 00:00
	case NIGHTTOMID:

		if (m_minutes <= 21)
		{
			//std::cout << "Night to Mid" << std::endl;
			m_sunTargetInterpolate = DirectX::XMVECTOR{ 50.0f / m_divider,50.0f / m_divider, 220.0f / m_divider };
			m_colorScaleFactor = m_seconds / (3.0f * 60.0f);
			m_sunCurrentFinalColor = DirectX::XMVectorLerp(m_sunStartInterpolate, m_sunTargetInterpolate, m_colorScaleFactor);
			DirectX::XMStoreFloat4A(&m_fFinalColor, m_sunCurrentFinalColor);
				

			m_sunRotationTarget = DirectX::XMVECTOR{ 0, 1, 0 };			
			m_rotationFactor = m_colorScaleFactor;
			m_sunFinalRotation = DirectX::XMVectorLerp(m_sunRotationStart, m_sunRotationTarget, m_rotationFactor);
			DirectX::XMVector3Normalize(m_sunFinalRotation);
			XMStoreFloat4A(&m_fFinalRotation, m_sunFinalRotation);
		}
		else
		{
			m_seconds = 0;
			m_currentTime = NIGHTTOMORNING;
			m_sunStartInterpolate = m_sunCurrentFinalColor;
			m_sunRotationStart = m_sunFinalRotation;
			std::cout << m_fFinalRotation.y << std::endl; 
		}
		break;
	//Fårn 00:00 -> 06:00
	case NIGHTTOMORNING: 

		if (m_minutes <= 24)
		{
			//std::cout << "Night to Morning" << std::endl;
			m_sunTargetInterpolate = DirectX::XMVECTOR{ 30.0f / m_divider,30.0f / m_divider, 85.0f / m_divider };
			m_colorScaleFactor = m_seconds / (3.0f * 60.0f);
			m_sunCurrentFinalColor = DirectX::XMVectorLerp(m_sunStartInterpolate, m_sunTargetInterpolate, m_colorScaleFactor);
			DirectX::XMStoreFloat4A(&m_fFinalColor, m_sunCurrentFinalColor);

			m_sunRotationTarget = DirectX::XMVECTOR{ 0.3f, -0.2f, 0 };
			m_rotationFactor = m_colorScaleFactor;
			m_sunFinalRotation = DirectX::XMVectorLerp(m_sunRotationStart, m_sunRotationTarget, m_rotationFactor);
			DirectX::XMVector3Normalize(m_sunFinalRotation);
			XMStoreFloat4A(&m_fFinalRotation, m_sunFinalRotation);
		}
		else
		{
			m_seconds = 0;
			m_currentTime = MORNINGTONOON;
			m_sunStartInterpolate = DirectX::XMVECTOR{ 182.0f / m_divider,126.0f / m_divider,91.0f / m_divider };
			m_sunRotationStart = m_sunFinalRotation; 
			m_totalSeconds = 0;
			m_minutes = 0; 
		}
	}
	m_sun.setDir(m_fFinalRotation);
	m_sun.setColor(m_fFinalColor);


	m_cpyLightToGPU(); 
	
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
	m_sunBuffer.color = m_sun.getColor(); 
	m_sunBuffer.dir = m_sun.getDir(); 
	DX::g_deviceContext->PSSetConstantBuffers(2, 1, &m_pSunBuffer); 
	m_sun.cpyDataDir(m_sunBuffer,m_pSunBuffer); 
}
