#pragma once
#include "Camera.h"
#include <Windows.h>
class MayaCamera : public Camera
{
private:
	bool m_panning, m_rotating;
	OurMath::Vec2 m_initialMousePosition;
	OurMath::Vec3 m_initialFocalPoint, m_initialRotation;

	float m_distance;
	float m_panSpeed, m_rotationSpeed, m_zoomSpeed;

	float m_pitch;
	float m_yaw;
public:
	MayaCamera();
	virtual void update();
private:
	void MouseRotate(const OurMath::Vec2& delta);
	void MousePan(const OurMath::Vec2& delta);
	void MouseZoom(float delta);

	OurMath::Vec3 CalculatePosition();

	OurMath::Quaternion GetOrientation();
	OurMath::Vec3 GetRightDirection();
	OurMath::Vec3 GetUpDirection();
	OurMath::Vec3 GetForwardDirection();
};


