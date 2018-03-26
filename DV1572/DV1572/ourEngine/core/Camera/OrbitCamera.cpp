#include "OrbitCamera.h"

#include <algorithm>

OrbitCamera::OrbitCamera()
{
}

OrbitCamera::OrbitCamera(glm::vec3 cameraTarget)
{
	CameraTargetPos = cameraTarget;
	CameraTargetRot = glm::vec3(0, 0, 0);
	Yaw = ORBIT_CAMERA_YAW_DEFAULT;
	Pitch = ORBIT_CAMERA_PITCH_DEFUALT;
	MouseSensitivity = ORBIT_CAMERA_MOUSE_SENSITIVTY;
	DistanceToTarget = 10.0f;
	AngleAroundTarget = ORBIT_CAMERA_ANGLE_DEFUALT;

	m_lookAt = DirectX::XMFLOAT3(0.0f,0.0f, -1.0f);
	m_pos = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
}

OrbitCamera::~OrbitCamera()
{
}


void OrbitCamera::update()
{
	OurMath::Vec2 mouse = m_mousePos;
	OurMath::Vec2 delta = mouse - m_lastMouse;
	m_lastMouse = mouse;

	using namespace DirectX;

	if (GetAsyncKeyState(VK_MBUTTON))
	{
		DistanceToTarget += delta.y * 0.5f;
	
	}
	if (GetAsyncKeyState(VK_LBUTTON)) 
	{
		XMVECTOR rotPoint = XMLoadFloat3(&m_lookAt);
		rotPoint = XMVector3Normalize(rotPoint) * DistanceToTarget;
		XMVECTOR camPos = XMLoadFloat3(&m_pos);

		XMVECTOR RotToPos = camPos - rotPoint;
		XMMATRIX rotX = XMMatrixRotationX(delta.y * 0.01f);
		XMMATRIX rotYAndX = rotX * XMMatrixRotationY(delta.x * 0.01f);
		RotToPos = XMVector3TransformNormal(RotToPos, rotYAndX);

		XMVECTOR newCamPos = rotPoint + RotToPos;
		XMVECTOR newLookAt = XMVector3Normalize(-RotToPos);
		XMStoreFloat3(&m_pos, newCamPos);
		XMStoreFloat3(&m_lookAt, newLookAt);

	}

	

	
	setViewMatrix();
}

const glm::mat4 OrbitCamera::GetViewMatrix() const
{
	glm::mat4 viewMatrix = glm::mat4(1.0f);
	viewMatrix = glm::rotate(viewMatrix, glm::radians(Pitch), glm::vec3(1.0f, 0.0f, 0.0f));
	viewMatrix = glm::rotate(viewMatrix, glm::radians(Yaw), glm::vec3(0.0f, 1.0f, 0.0f));
	viewMatrix = glm::translate(viewMatrix, -Position);
	return viewMatrix;
}

void OrbitCamera::ProcessMouseMovement(float offsetX, float offsetY)
{

	offsetX *= MouseSensitivity;
	offsetY *= MouseSensitivity;

	//Yaw += offsetX;
	if (GetAsyncKeyState(VK_LBUTTON))
	{
		AngleAroundTarget -= offsetX;
		AngleAroundTarget = glm::mod(AngleAroundTarget - offsetX, 360.0f);
	}

	if (GetAsyncKeyState(VK_RBUTTON))
	{
		Pitch -= offsetY;
		
		
		Pitch = glm::clamp(Pitch, -89.0f, 89.0f);
	}

	//Yaw = glm::mod(Yaw + offsetX, 360.0f);
}

void OrbitCamera::ProcessMouseScroll(double offsetY)
{
	if (GetAsyncKeyState(VK_MBUTTON))
	{
		if (DistanceToTarget >= 10.0f && DistanceToTarget <= 45.0f)
			DistanceToTarget -= static_cast<float>(offsetY);
		if (DistanceToTarget <= 10.0f)
			DistanceToTarget = 10.0f;
		if (DistanceToTarget >= 45.0f)
			DistanceToTarget = 45.0f;
	}
	
}

float OrbitCamera::InvertPitch()
{
	Pitch = -Pitch;
	return Pitch;
}

void OrbitCamera::updateCameraPostion(float horizontalDistance, float verticalDistance)
{
	float theta = CameraTargetRot.y + AngleAroundTarget;
	float offsetX = horizontalDistance * sin(glm::radians(theta));
	float offsetZ = horizontalDistance * cos(glm::radians(theta));
	Position.x = CameraTargetPos.x - offsetX;
	Position.y = CameraTargetPos.y + verticalDistance + 5;
	Position.z = CameraTargetPos.z - offsetZ;
}
