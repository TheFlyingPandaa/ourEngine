#pragma once
#include "Camera.h"
#include <Windows.h>
class FPSCamera : public Camera
{
private:
	float m_mouseSensitivty;
	float m_speed;
	float m_pitch;
	float m_yaw;
	
public:
	FPSCamera();
	virtual void update();
	OurMath::Quaternion GetOrientation();

	OurMath::Vec3 GetForwardDirection(const OurMath::Quaternion& orientation);

	OurMath::Vec3 GetRightDirection(const OurMath::Quaternion& orientation);
};