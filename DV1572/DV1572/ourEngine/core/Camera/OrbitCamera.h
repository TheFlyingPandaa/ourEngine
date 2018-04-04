#pragma once
#include "Camera.h"
#include <Windows.h>
using namespace DirectX;

class OrbitCamera : public Camera
{
private:
	float m_distanceFromTarget;
	DirectX::XMFLOAT2 m_windowDim;

	DirectX::XMFLOAT2 rotation;
	float m_sensitivity;
	float m_tiltCapDown;
	float m_tiltCapUp;


public:

	void setMaxTilt(float t);
	void setMinTilt(float t);

	OrbitCamera(DirectX::XMFLOAT2 windowDim);
	~OrbitCamera();

	void update();

private:
};
