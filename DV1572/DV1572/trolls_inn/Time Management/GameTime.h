#ifndef GAMETIME_H 
#define GAMETIME_H
#include <DirectXMath.h>
#include "../../ourEngine/interface/light/Light.h"
#include <cmath>
#include <vector>
#include "../../ourEngine/core/Dx.h"
#include <d3d11.h>
class GameTime
{
private:
	enum TIMEOFDAY
	{
		MORNINGTONOON,
		NOONTOEVENING,
		EVENINGTONIGHT,
		NIGHTTOMID,
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
	
	DirectX::XMVECTOR m_moonStartInterpolate; 
	DirectX::XMVECTOR m_moonTargetInterpolate;
	
	DirectX::XMVECTOR m_sunStartInterpolate; 
	DirectX::XMVECTOR m_sunTargetInterpolate; 
	
	DirectX::XMVECTOR m_sunCurrentFinalColor;
	DirectX::XMVECTOR m_moonCurrentFinalColor;

	DirectX::XMVECTOR m_sunRotationStart;
	DirectX::XMVECTOR m_sunRotationTarget;
	DirectX::XMVECTOR m_moonRotationStart;
	DirectX::XMVECTOR m_moonRotationTarget;

	DirectX::XMVECTOR m_sunFinalRotation;
	DirectX::XMVECTOR m_moonFinalRotation;

	DirectX::XMFLOAT4A m_fFinalColor; 
	DirectX::XMFLOAT4A m_fFinalRotation; 

	DirectX::XMVECTOR m_vSunDir; 
	DirectX::XMVECTOR m_vUp;

	DirectX::XMVECTOR m_sunPos;
	float m_sunAngle;
	float m_currentAngle;
	float m_sunAngleTarget;

	ID3D11Buffer* m_pSunBuffer; 
	ID3D11Buffer* m_pMoonBuffer;

	DIRECTIONAL_LIGHT_BUFFER m_sunBuffer; 
	DIRECTIONAL_LIGHT_BUFFER m_moonBuffer; 

	Light m_sun; 
	Light m_moon;

	std::vector<Light*> m_sunAndMoon;


	void m_createSunAndMoonBuffers(); 

	void m_cpyLightToGPU();

public:
	GameTime(); 
	~GameTime(); 

	void updateCurrentTime(float refreshRate); 
	TIMEOFDAY getTimePeriod(); 

	Light& getSun(); 
	Light& getMoon(); 
	
	std::vector<Light*>& getSunAndMoonVector(); 
};

#endif 
