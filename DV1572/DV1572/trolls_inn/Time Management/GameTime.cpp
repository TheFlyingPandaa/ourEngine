#include "GameTime.h"
#include <DirectXMath.h>
#include <iostream>
#include <string>

GameTime::GameTime()
{
	m_currentClockValue = 0.0f;
	m_currentTime = MORNINGTONOON; 
	m_minutes = 0;
	m_seconds = 0.0f; 
	m_totalSeconds = 0; 
	m_divider = 255.0f; 
	m_colorScaleFactor = 0.00001f; 
	m_startInterpolate = DirectX::XMVECTOR{ 0,0,0 }; 
	m_targetInterpolate = DirectX::XMVECTOR{ 0,0,0 };
	m_currentFinalColor = DirectX::XMVECTOR{ 0,0,0 };
}

GameTime::~GameTime()
{
}

void GameTime::updateCurrentTime(float refreshRate)
{
	m_currentClockValue = (1.0f / refreshRate);
	m_seconds += m_currentClockValue; 
	
	if (m_currentClockValue >= 1)
	{
		m_totalSeconds++;
		m_minutes = m_totalSeconds / 60;
	}

	switch (m_currentTime)
	{
	case MORNINGTONOON:
		if (m_minutes >= 0 && m_minutes <= 6) 
		{
			m_startInterpolate = DirectX::XMVECTOR{ 182.0f / m_divider,126.0f / m_divider,91.0f / m_divider };
			m_targetInterpolate = DirectX::XMVECTOR{ 192.0f / m_divider,191.0f / m_divider, 173.0f / m_divider };
			m_colorScaleFactor = m_seconds / (6.0f * 60.0f); 
			m_currentFinalColor = DirectX::XMVectorLerp(m_startInterpolate, m_targetInterpolate, m_colorScaleFactor); 
			DirectX::XMStoreFloat4A(&m_fFinalColor, m_currentFinalColor); 
		}
		else
		{
			m_seconds = 0; 
			m_currentTime = NOONTOEVENING; 
			m_startInterpolate = m_targetInterpolate; 
		}
		break; 
	
	case NOONTOEVENING:
		if (m_minutes > 6 && m_minutes <= 12) 
		{
			m_targetInterpolate = DirectX::XMVECTOR{ 182.0f / m_divider,126.0f / m_divider, 91.0f / m_divider };
			m_colorScaleFactor = m_seconds / (6.0f * 60.0f); 
			m_currentFinalColor = DirectX::XMVectorLerp(m_startInterpolate, m_targetInterpolate, m_colorScaleFactor);
			DirectX::XMStoreFloat4A(&m_fFinalColor, m_currentFinalColor);
		}
		else
		{
			m_seconds = 0;
			m_currentTime = EVENINGTONIGHT;
			m_startInterpolate = m_targetInterpolate;
		}
		break; 

	case EVENINGTONIGHT:
		if (m_minutes > 12 && m_minutes <= 18) 
		{
			m_targetInterpolate = DirectX::XMVECTOR{ 20.0f / m_divider,20.0f / m_divider, 20.0f / m_divider };
			m_colorScaleFactor = m_seconds / (6.0f * 60.0f); 
			m_currentFinalColor = DirectX::XMVectorLerp(m_startInterpolate, m_targetInterpolate, m_colorScaleFactor);
			DirectX::XMStoreFloat4A(&m_fFinalColor, m_currentFinalColor);
		}
		else
		{
			m_seconds = 0;
			m_currentTime = NIGHTTOMORNING;
			m_startInterpolate = m_targetInterpolate;
		}
		break;

	case NIGHTTOMORNING:

		if (m_minutes > 18 && m_minutes <= 24)
		{
			m_targetInterpolate = DirectX::XMVECTOR{ 182.0f / m_divider,126.0f / m_divider, 91.0f / m_divider };
			m_colorScaleFactor = m_seconds / (6.0f * 60.0f); 
			m_currentFinalColor = DirectX::XMVectorLerp(m_startInterpolate, m_targetInterpolate, m_colorScaleFactor);
			DirectX::XMStoreFloat4A(&m_fFinalColor, m_currentFinalColor);
		}
		else
		{
			m_seconds = 0;
			m_currentTime = MORNINGTONOON;
			m_totalSeconds = 0;
		}
		break;
	}
}

DirectX::XMFLOAT4A GameTime::getFinalSunColor() const
{
	return m_fFinalColor; 
}
