#include "OrbitCamera.h"
#include <iostream>
OrbitCamera::OrbitCamera(DirectX::XMFLOAT2 windowDim) : Camera()
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

	XMVECTOR xmCamPos = XMLoadFloat3(&m_pos);
	XMVECTOR xmLookAt = XMVector3Normalize(XMLoadFloat3(&m_lookAt));

	if (GetAsyncKeyState(int('Z')))
		m_distanceFromTarget += 0.1f;
	if (GetAsyncKeyState(VK_SPACE))
	{
		if (GetAsyncKeyState(int('W')))
		{
			XMVECTOR front = xmLookAt;


			xmCamPos += front * 0.1f;

			XMStoreFloat3(&m_pos, xmCamPos);
			

		}
		else if (GetAsyncKeyState(int('S')))
		{
			XMVECTOR front = -xmLookAt;

			xmCamPos += front * 0.1f;


			XMStoreFloat3(&m_pos, xmCamPos);
			
		}


		if (GetAsyncKeyState(int('A')))
		{
			XMVECTOR front = xmLookAt;
			XMVECTOR up = m_up;
			XMVECTOR right = -XMVector3Normalize(XMVector3Cross(up, front));


			xmCamPos += right * 0.1f;
	


			XMStoreFloat3(&m_pos, xmCamPos);

		}
		else if (GetAsyncKeyState(int('D')))
		{
			XMVECTOR front = xmLookAt;
			XMVECTOR up = m_up;
			XMVECTOR right = XMVector3Normalize(XMVector3Cross(up, front));

			xmCamPos += right * 0.1f;
			xmLookAt += right * 0.1f;


			XMStoreFloat3(&m_pos, xmCamPos);
		
		}
		
	}
	else
	{
		XMVECTOR rotPoint = (xmLookAt * m_distanceFromTarget);


		XMVECTOR RotToPos = rotPoint - xmCamPos;
		XMMATRIX rotX = XMMatrixRotationX(delta.y * 0.01f);
		XMMATRIX rotYAndX = rotX * XMMatrixRotationY(delta.x * 0.01f);
		RotToPos = XMVector3TransformNormal(-RotToPos, rotYAndX);

		XMVECTOR newCamPos = RotToPos - xmCamPos;
		XMVECTOR newLookAt = XMVector3Normalize(-RotToPos);

		XMStoreFloat3(&m_pos, newCamPos);
		XMStoreFloat3(&m_lookAt, newLookAt - newCamPos);
	}
	


	setViewMatrix();
}
