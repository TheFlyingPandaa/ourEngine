#include "GameTime.h"
#include <DirectXMath.h>
#include <iostream>

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
	m_startInterpolate = DirectX::XMVECTOR{ 0,0,0 }; 
	m_targetInterpolate = DirectX::XMVECTOR{ 0,0,0 };
	m_currentFinalColor = DirectX::XMVECTOR{ 0,0,0 };
	m_sunRotationStart = DirectX::XMVECTOR{ 0,0,0 };
	m_sunRotationTarget = DirectX::XMVECTOR{ 0,0,0 };

	m_startInterpolate = DirectX::XMVECTOR{ 182.0f / m_divider,126.0f / m_divider,91.0f / m_divider };
}

GameTime::~GameTime()
{
}

void GameTime::updateCurrentTime(float refreshRate, Light& light)
{
	m_currentClockValue = 1.0f; //(1.0f / refreshRate);
	m_seconds += m_currentClockValue; 
	
	if (m_currentClockValue >= 1)
	{
		m_totalSeconds++;
		m_minutes = m_totalSeconds / 60;
	}
	

	light.setDir(DirectX::XMFLOAT4A((float)cos((m_totalSeconds / (60.0f * 24.0f)) * (3.14159265358979323846f * 2) + 3.14159265358979323846f),
									(float)sin((m_totalSeconds / (60.0f * 24.0f)) * (3.14159265358979323846f * 2) + 3.14159265358979323846f), 0.0f, 0.0f));
	//std::cout << "X: " << (float)cos((m_totalSeconds / (60.0f * 24.0f)) * (3.14159265358979323846f * 2) + 3.14159265358979323846f) << " Y: " << (float)sin((m_totalSeconds / (60.0f * 24.0f)) * (3.14159265358979323846f * 2) + 3.14159265358979323846f) << std::endl;

	if (m_minutes >= 24)
		m_totalSeconds = 0;

	light.setColor(DirectX::XMFLOAT4A((float)abs(cos((m_totalSeconds / (60.0f * 24.0f)) * (3.14159265358979323846f * 2) + 3.14159265358979323846f)),
		1.0f, 1.0f, 1.0f));

	return;

	switch (m_currentTime)
	{
	case MORNINGTONOON:
		if (m_minutes <= 6) 
		{
			std::cout << "Morning to Noon" << std::endl;
			m_startInterpolate = DirectX::XMVECTOR{ 182.0f / m_divider,126.0f / m_divider,91.0f / m_divider };
			m_targetInterpolate = DirectX::XMVECTOR{ 192.0f / m_divider,191.0f / m_divider, 173.0f / m_divider };
			m_colorScaleFactor = m_seconds / (6.0f * 60.0f); 
			m_currentFinalColor = DirectX::XMVectorLerp(m_startInterpolate, m_targetInterpolate, m_colorScaleFactor); 
			DirectX::XMStoreFloat4A(&m_fFinalColor, m_currentFinalColor); 

			m_sunRotationTarget = DirectX::XMVECTOR{ 0, -1, 0 };
			m_rotationFactor = m_colorScaleFactor; 
			m_finalRotation = DirectX::XMVectorLerp(m_sunRotationStart, m_sunRotationTarget, m_rotationFactor); 
			DirectX::XMVector3Normalize(m_finalRotation);
			XMStoreFloat4A(&m_fFinalRotation, m_finalRotation); 
			//light.setDir(m_fFinalRotation);
		}
		else
		{
			m_seconds = 0; 
			m_currentTime = NOONTOEVENING; 
			m_startInterpolate = m_targetInterpolate;
			m_sunRotationStart = m_finalRotation; 
		}
		break; 
	
	case NOONTOEVENING:
		if (m_minutes <= 12) 
		{
			std::cout << "Noon to Evening" << std::endl;

			m_targetInterpolate = DirectX::XMVECTOR{ 182.0f / m_divider,126.0f / m_divider, 91.0f / m_divider };
			m_colorScaleFactor = m_seconds / (6.0f * 60.0f);
			m_currentFinalColor = DirectX::XMVectorLerp(m_startInterpolate, m_targetInterpolate, m_colorScaleFactor);
			DirectX::XMStoreFloat4A(&m_fFinalColor, m_currentFinalColor);

			m_sunRotationTarget = DirectX::XMVECTOR{ -0.5f, -0.3f, 0 };
			m_rotationFactor = m_colorScaleFactor;
			m_finalRotation = DirectX::XMVectorLerp(m_sunRotationStart, m_sunRotationTarget, m_rotationFactor);
			DirectX::XMVector3Normalize(m_finalRotation); 
			XMStoreFloat4A(&m_fFinalRotation, m_finalRotation);
			//light.setDir(m_fFinalRotation);
		}
		else
		{
			m_seconds = 0;
			m_currentTime = EVENINGTONIGHT;
			m_startInterpolate = m_targetInterpolate;
			m_sunRotationStart = m_finalRotation;
		}
		break; 

	case EVENINGTONIGHT:
		if (m_minutes <= 18) 
		{

			std::cout << "Evening to Night" << std::endl;
			m_targetInterpolate = DirectX::XMVECTOR{ 20.0f / m_divider,20.0f / m_divider, 70.0f / m_divider };
			m_colorScaleFactor = m_seconds / (6.0f * 60.0f);
			m_currentFinalColor = DirectX::XMVectorLerp(m_startInterpolate, m_targetInterpolate, m_colorScaleFactor);
			DirectX::XMStoreFloat4A(&m_fFinalColor, m_currentFinalColor);

			m_sunRotationTarget = DirectX::XMVECTOR{ -1.0f, 0.0f, 0.0f };
			m_rotationFactor = m_colorScaleFactor;
			m_finalRotation = DirectX::XMVectorLerp(m_sunRotationStart, m_sunRotationTarget, m_rotationFactor);
			DirectX::XMVector3Normalize(m_finalRotation);
			XMStoreFloat4A(&m_fFinalRotation, m_finalRotation);
			//light.setDir(m_fFinalRotation);
		}
		else
		{
			m_seconds = 0;
			m_currentTime = NIGHTTOMORNING;
			m_startInterpolate = m_targetInterpolate;
		}
		break;

	case NIGHTTOMORNING:


		if (m_minutes <= 24)
		{

			std::cout << "Night to Morning" << std::endl;
			m_targetInterpolate = DirectX::XMVECTOR{ 182.0f / m_divider,126.0f / m_divider, 91.0f / m_divider };

			m_colorScaleFactor = m_seconds / (6.0f * 60.0f) + .01f;
			m_currentFinalColor = DirectX::XMVectorLerp(m_startInterpolate, m_targetInterpolate, m_colorScaleFactor);
			DirectX::XMStoreFloat4A(&m_fFinalColor, m_currentFinalColor);




			m_finalRotation = DirectX::XMVectorLerp(m_sunRotationStart, m_sunRotationTarget, m_rotationFactor);
			XMStoreFloat4A(&m_fFinalRotation, m_finalRotation);
			//light.setDir(m_fFinalRotation);
		}
		else
		{
			m_seconds = 0;
			m_currentTime = MORNINGTONOON;
			m_totalSeconds = 0;
			m_startInterpolate = m_currentFinalColor;
		}
		break;
	}
}

DirectX::XMFLOAT4A GameTime::getFinalSunColor() const
{
	return m_fFinalColor; 
}
