#include "OrbitCamera.h"
#include <iostream>
#include "../../interface/Input.h"
void OrbitCamera::setMaxTilt(float t)
{
	m_tiltCapUp = t;
}
void OrbitCamera::setMinTilt(float t)
{
	m_tiltCapDown = t;
}
OrbitCamera::OrbitCamera(DirectX::XMFLOAT2 windowDim, DirectX::XMFLOAT3 pos) : Camera()
{
	m_distanceFromTarget = 10.0f;
	m_windowDim = windowDim;
	m_lastMouse = { 0,0 };
	rotation.x = rotation.y = 0;
	m_sensitivity = 0.01f;
	m_tiltCapDown = 0.0f;
	m_tiltCapUp = 0.5f;
	m_zoomSensitivity = 0.5f;
	m_pos = pos;

}

OrbitCamera::~OrbitCamera()
{
}

void printVec3(const char* name , XMVECTOR vec)
{
	XMFLOAT3 debugVec;
	XMStoreFloat3(&debugVec, vec);
	std::cout << name << " (" <<debugVec.x << "," << debugVec.y << "," << debugVec.z <<")"<<std::endl;
}
void OrbitCamera::update()
{
	XMVECTOR xmMouse = XMLoadFloat2(&Input::getMousePosition());
	XMVECTOR xmLastMouse = XMLoadFloat2(&m_lastMouse);
	XMVECTOR xmDelta = xmMouse - xmLastMouse;
	XMStoreFloat2(&m_lastMouse, xmMouse);
	XMFLOAT2 delta;

	XMStoreFloat2(&delta, xmDelta);
	float xDeltaMouse = delta.x;
	float yDeltaMouse = delta.y;
	
	XMVECTOR xmCamPos = XMLoadFloat3(&m_pos);
	XMVECTOR xmLookAt = XMVector3Normalize(XMLoadFloat3(&m_lookAt));

	// --- ZOOM ----
	float modifier = m_zoomSensitivity * Input::getMouseScroll();

	float potDist = m_distanceFromTarget - modifier;

	if (potDist > 2.0f && potDist < 50.0f)
	{
		m_distanceFromTarget -= modifier;

		xmCamPos = xmCamPos + (xmLookAt *  modifier);
	}
	
	
	XMStoreFloat3(&m_pos, xmCamPos);
	// --- ZOOM ----
	if (Input::isMouseRightPressed())
	{
		XMVECTOR rotVector = (xmCamPos + (XMVector3Normalize(xmLookAt) * m_distanceFromTarget));
		XMVECTOR startLA = XMVectorSet(0, 0, -1, 0);

		float potentialTilt = rotation.x - yDeltaMouse * m_sensitivity;

		if (abs(potentialTilt) < XM_PI * m_tiltCapUp && potentialTilt < 0.0f)
		{
			rotation.x -= yDeltaMouse * m_sensitivity;
		}
		rotation.y += xDeltaMouse * m_sensitivity;

		XMMATRIX rot = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, 0.0f);

		XMVECTOR newLookAt = XMVector3Normalize(XMVector3Transform(startLA, rot));
		XMVECTOR newPos = rotVector + (-newLookAt * m_distanceFromTarget);
		
		xmCamPos = newPos;
		xmLookAt = newLookAt;

		XMStoreFloat3(&m_lookAt, xmLookAt);
		XMStoreFloat3(&m_pos, xmCamPos);
	}
	else if(Input::isMouseLeftPressed())
	{
		// If Mouse is 20% of the top
		if (m_lastMouse.y < m_windowDim.y * 0.2)
		{
			float acceleration = 1.0f - (m_lastMouse.y) / (m_windowDim.y * 0.2f);
			XMVECTOR front = xmLookAt;
			XMVECTOR up = m_up;
			XMVECTOR right = -XMVector3Normalize(XMVector3Cross(up, front));
			XMVECTOR newFront = XMVector3Normalize(XMVector3Cross(up, right));

			xmCamPos += newFront * acceleration;

			XMStoreFloat3(&m_pos, xmCamPos);

		} // end if --- if mouse to 80% of the bottom
		else if (m_lastMouse.y > m_windowDim.y * 0.8)
		{

			float distanceGap = m_windowDim.y - (m_windowDim.y * 0.8f);
			float acceleration = (m_lastMouse.y - m_windowDim.y * 0.8f) / distanceGap;

			XMVECTOR front = xmLookAt;
			XMVECTOR up = m_up;
			XMVECTOR right = XMVector3Normalize(XMVector3Cross(up, front));
			XMVECTOR newFront = XMVector3Normalize(XMVector3Cross(up, right));

			xmCamPos += newFront * acceleration;


			XMStoreFloat3(&m_pos, xmCamPos);

		}

		// If the mouse is 20% to the left of the screen
		if (m_lastMouse.x < m_windowDim.x * 0.2f)
		{
			float acceleration = 1.0f - (m_lastMouse.x) / (m_windowDim.x * 0.2f);
			XMVECTOR front = xmLookAt;
			XMVECTOR up = m_up;
			XMVECTOR right = -XMVector3Normalize(XMVector3Cross(up, front));

			xmCamPos += right * acceleration;

			XMStoreFloat3(&m_pos, xmCamPos);

		}//If the mouse is 80% to right of the screen
		else if (m_lastMouse.x > m_windowDim.x * 0.8f)
		{
			float distanceGap = m_windowDim.x - (m_windowDim.x * 0.8f);
			float accceleration = (m_lastMouse.x - m_windowDim.x * 0.8f) / distanceGap;
			XMVECTOR front = xmLookAt;
			XMVECTOR up = m_up;
			XMVECTOR right = XMVector3Normalize(XMVector3Cross(up, front));

			xmCamPos += right * accceleration;

			XMStoreFloat3(&m_pos, xmCamPos);

		}
	}

	setViewMatrix();
}

