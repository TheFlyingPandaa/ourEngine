#include "OrbitCamera.h"
#include <iostream>
OrbitCamera::OrbitCamera(DirectX::XMFLOAT2 windowDim)
{
	m_distanceFromTarget = 10.0f;
	m_yaw = 0.0f;
	m_pitch = 0.0f;
	m_windowDim = m_windowDim;
	m_lastMouse = { 0,0 };
}

OrbitCamera::~OrbitCamera()
{
}

void OrbitCamera::update(DirectX::XMFLOAT2 mousePos)
{
	m_mousePos = mousePos;
	update();
}

void OrbitCamera::update()
{
	XMVECTOR xmMouse = XMLoadFloat2(&m_mousePos);
	XMVECTOR xmLastMouse = XMLoadFloat2(&m_lastMouse);
	XMVECTOR xmDelta = xmMouse - xmLastMouse;
	XMStoreFloat2(&m_lastMouse, xmMouse);
	XMFLOAT2 delta;
	XMStoreFloat2(&delta, xmDelta);
	
	std::cout << delta.x << " " << delta.y << std::endl;

	XMVECTOR rotPoint = XMVector3Normalize(XMLoadFloat3(&m_lookAt)) * m_distanceFromTarget;
	XMVECTOR camPos = XMLoadFloat3(&m_pos);

	XMVECTOR RotToPos = rotPoint - camPos;
	XMMATRIX rotX = XMMatrixRotationX(delta.y * 0.01f);
	XMMATRIX rotYAndX = rotX * XMMatrixRotationY(delta.x * 0.01f);
	RotToPos = XMVector3TransformNormal(-RotToPos, rotYAndX);

	XMVECTOR newCamPos = RotToPos - camPos;
	XMVECTOR newLookAt = XMVector3Normalize(-RotToPos);

	XMStoreFloat3(&m_pos, newCamPos);
	XMStoreFloat3(&m_lookAt, newLookAt);

	setViewMatrix();
}
