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
void printVec3(XMVECTOR vec)
{
	XMFLOAT3 debugVec;
	XMStoreFloat3(&debugVec, vec);
	std::cout << debugVec.x << "," << debugVec.y << "," << debugVec.z << std::endl;
}
void OrbitCamera::update()
{
	XMVECTOR xmMouse = XMLoadFloat2(&m_mousePos);
	XMVECTOR xmLastMouse = XMLoadFloat2(&m_lastMouse);
	XMVECTOR xmDelta = xmMouse - xmLastMouse;
	XMStoreFloat2(&m_lastMouse, xmMouse);
	XMFLOAT2 delta;

	XMStoreFloat2(&delta, xmDelta);
	m_yaw = delta.x;
	m_pitch = delta.y;

	XMVECTOR xmCamPos = XMLoadFloat3(&m_pos);
	XMVECTOR xmLookAt = XMVector3Normalize(XMLoadFloat3(&m_lookAt));

	static XMFLOAT3 target = { -5, 0, 2 };

	XMVECTOR xmTarget = XMLoadFloat3(&target);

	if (GetAsyncKeyState(int('Z')))
		m_distanceFromTarget += 0.1f;
	if(GetAsyncKeyState(int('X')))
		m_distanceFromTarget -= 0.1f;
	
		if (GetAsyncKeyState(int('W')))
		{
			XMVECTOR front = xmLookAt;
			XMVECTOR up = m_up;
			XMVECTOR right = -XMVector3Normalize(XMVector3Cross(up, front));
			XMVECTOR newFront = XMVector3Normalize(XMVector3Cross(up, right));
			xmCamPos += newFront * 0.1f;
			XMStoreFloat3(&m_pos, xmCamPos);
			

		}
		else if (GetAsyncKeyState(int('S')))
		{
			XMVECTOR front = xmLookAt;
			XMVECTOR up = m_up;
			XMVECTOR right = XMVector3Normalize(XMVector3Cross(up, front));
			XMVECTOR newFront = XMVector3Normalize(XMVector3Cross(up, right));
			xmCamPos += newFront * 0.1f;


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
	
			XMStoreFloat3(&m_pos, xmCamPos);
		
		}
		
		if (GetAsyncKeyState(VK_SPACE))
		{
			
			XMVECTOR rotVector = (XMVector3Normalize(xmCamPos + xmLookAt) * m_distanceFromTarget);

			XMMATRIX trans = XMMatrixTranslationFromVector(rotVector);
			XMMATRIX rotX = XMMatrixRotationX(delta.y * 0.1f);
			XMMATRIX rotY = XMMatrixRotationY(delta.x * 0.1f);
			XMVECTOR newPos = XMVector3Transform(rotVector, rotY * rotX * trans);

			xmCamPos = newPos;
			xmLookAt = -newPos;


			XMStoreFloat3(&m_lookAt, xmLookAt);
		}
		
		
	
	XMStoreFloat3(&m_pos, xmCamPos);


	setViewMatrix();
}

