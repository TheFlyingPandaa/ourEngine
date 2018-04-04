#ifndef GAMETIME_H 
#define GAMETIME_H
#include <DirectXMath.h>
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

	DirectX::XMVECTOR m_startInterpolate; 
	DirectX::XMVECTOR m_targetInterpolate; 
	DirectX::XMVECTOR m_currentFinalColor; 

	DirectX::XMFLOAT4A m_fFinalColor; 

public:
	GameTime(); 
	~GameTime(); 

	void updateCurrentTime(float refreshRate); 

	DirectX::XMFLOAT4A getFinalSunColor() const; 
};

#endif 
