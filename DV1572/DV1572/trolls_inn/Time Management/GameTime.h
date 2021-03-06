#ifndef GAMETIME_H 
#define GAMETIME_H
#include <DirectXMath.h>
#include "../../ourEngine/interface/light/Light.h"
#include <cmath>
#include <vector>
#include <d3d11.h>
enum TIMEOFDAY
{
	MORNINGTONOON,
	NOONTOEVENING,
	EVENINGTONIGHT,
	NIGHTTOMID,
	NIGHTTOMORNING
};
class GameTime
{
private:

	TIMEOFDAY m_currentTime;
	float m_currentClockValue;
	float m_seconds; 
	short m_totalSeconds; 
	short m_minutes; 

	float m_divider;
	float m_colorScaleFactor;
	float m_rotationFactor; 
	
	Light m_sun; 

	DirectX::XMVECTOR m_sunStartInterpolate; 
	DirectX::XMVECTOR m_sunTargetInterpolate; 
	
	DirectX::XMVECTOR m_sunCurrentFinalColor;

	DirectX::XMVECTOR m_sunRotationStart;
	DirectX::XMVECTOR m_sunRotationTarget;

	DirectX::XMVECTOR m_sunFinalRotation;

	DirectX::XMFLOAT4A m_fFinalColor; 
	DirectX::XMFLOAT4A m_fFinalRotation; 

	DirectX::XMVECTOR m_vSunDir; 
	DirectX::XMVECTOR m_vUp;

	DirectX::XMVECTOR m_sunPos;
	float m_sunAngle;
	float m_currentAngle;
	float m_sunAngleTarget;

	ID3D11Buffer* m_pSunBuffer; 

	DIRECTIONAL_LIGHT_BUFFER m_sunBuffer; 


	void m_createSunBuffer(); 


public:
	GameTime(); 
	~GameTime(); 

	void updateCurrentTime(float refreshRate); 
	TIMEOFDAY getTimePeriod(); 

	void m_cpyLightToGPU();
	Light& getSun(); 
};

#endif 
