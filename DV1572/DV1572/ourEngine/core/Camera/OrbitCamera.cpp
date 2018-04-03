#include "OrbitCamera.h"
#include <iostream>
#include "../../interface/Input.h"
OrbitCamera::OrbitCamera(DirectX::XMFLOAT2 windowDim) : Camera()
{
	m_distanceFromTarget = 10.0f;
	m_yaw = 0.0f;
	m_pitch = 0.0f;
	m_windowDim = windowDim;
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
	float modifier = 0.5f * Input::getMouseScroll();

	m_distanceFromTarget -= modifier;

	xmCamPos = xmCamPos + (xmLookAt *  modifier);
	
	XMStoreFloat3(&m_pos, xmCamPos);
	// --- ZOOM ----
	if (Input::isMouseRightPressed())
	{
		

		XMVECTOR rotVector = xmCamPos + (XMVector3Normalize(xmLookAt) * m_distanceFromTarget);
		

		XMMATRIX trans = XMMatrixTranslationFromVector(rotVector);
		XMMATRIX transBack = XMMatrixTranslationFromVector(-rotVector);

		XMMATRIX rotX = XMMatrixRotationX(yDeltaMouse * 0.01f);
		XMMATRIX rotY = XMMatrixRotationY(xDeltaMouse * 0.01f);
		XMVECTOR newPos = XMVector3Transform(xmCamPos, transBack * rotY * rotX * trans);
		XMVECTOR newLookAt = XMVector3Transform(xmLookAt, rotY * rotX);

		XMFLOAT3 xAxis = { 0,0,-1 };
		XMVECTOR cosine = XMVector3Dot(newLookAt, XMLoadFloat3(&xAxis)) / (XMVector3Length(newLookAt) * XMVector3Length(XMLoadFloat3(&xAxis)));
		XMFLOAT3 result;
		XMStoreFloat3(&result, cosine);
		float angle = (acos(result.x) *57.2957795f);
		
		
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
			float acceleration = 1 - (m_lastMouse.y) / (m_windowDim.y * 0.2);
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
			float acceleration = (m_lastMouse.y - m_windowDim.y * 0.8) / distanceGap;

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
			float acceleration = 1 - (m_lastMouse.x) / (m_windowDim.x * 0.2);
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

