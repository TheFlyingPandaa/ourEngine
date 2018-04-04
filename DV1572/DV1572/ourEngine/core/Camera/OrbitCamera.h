#pragma once
#include "Camera.h"
#include <Windows.h>
using namespace DirectX;

class OrbitCamera : public Camera
{
private:
	float m_distanceFromTarget;
	float m_yaw;
	float m_pitch;
	XMFLOAT3 m_target;
	DirectX::XMFLOAT2 m_windowDim;
public:

	OrbitCamera(DirectX::XMFLOAT2 windowDim);
	~OrbitCamera();

	void update();

private:
};
