#ifndef GAMETIME_H 
#define GAMETIME_H
#include <DirectXMath.h>
#include "../../ourEngine/interface/light/Light.h"
#include <cmath>
class GameTime
{
private:
	enum TIMEOFDAY
	{
		MORNINGTONOON,
		NOONTOEVENING,
		EVENINGTONIGHT,
		NIGHTTOMORNING
	};

	
	TIMEOFDAY m_currentTime;
	float m_currentClockValue;
	float m_seconds; 
	short m_totalSeconds; 
	short m_minutes; 

	float m_divider;
	float m_colorScaleFactor;
	float m_rotationFactor; 
 
	DirectX::XMVECTOR m_startInterpolate; 
	DirectX::XMVECTOR m_targetInterpolate; 
	DirectX::XMVECTOR m_currentFinalColor;
	DirectX::XMVECTOR m_sunRotationStart;
	DirectX::XMVECTOR m_sunRotationTarget;
	DirectX::XMVECTOR m_finalRotation; 

	DirectX::XMFLOAT4A m_fFinalColor; 
	DirectX::XMFLOAT4A m_fFinalRotation; 

public:
	GameTime(); 
	~GameTime(); 

	void updateCurrentTime(float refreshRate, Light& light); 

	DirectX::XMFLOAT4A getFinalSunColor() const; 
};

#endif 
