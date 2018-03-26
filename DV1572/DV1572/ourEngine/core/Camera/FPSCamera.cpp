#include "FPSCamera.h"

FPSCamera::FPSCamera()
{
	m_yaw = 0.0f;
	m_pitch = 0.0f;
	m_mouseSensitivty = 0.002f;
	m_lastMouse = OurMath::Vec2(0);
}

void FPSCamera::update()
{
	if (m_lastMouse != m_mousePos)
	{
		OurMath::Vec2 mouse = m_mousePos;
		float deltaX = 0.0f;
		float deltaY = 0.0f;
		if (GetAsyncKeyState(VK_SPACE))
		{
			deltaX = m_lastMouse.x - mouse.x;
			deltaY = m_lastMouse.y - mouse.y;
		}

		m_lastMouse = m_mousePos;

		m_yaw += deltaX * m_mouseSensitivty;
		m_pitch += deltaY * m_mouseSensitivty;
	}
	


	OurMath::Quaternion orientation = GetOrientation();
	OurMath::Vec3 m_Rotation = orientation.ToEulerAngles() * (180.0f / OurMath::PI);

	OurMath::Vec3 forward = GetForwardDirection(orientation);
	OurMath::Vec3 right = GetRightDirection(orientation);
	OurMath::Vec3 up = OurMath::Vec3::YAxis();
	OurMath::Vec3 pos = OurMath::Vec3(getPosition());

	if (GetAsyncKeyState(int('W')))
	{

		pos -= forward * 0.05f;
	}
	else if (GetAsyncKeyState(int('S')))
	{
		pos += forward * 0.05f;
	}

	if (GetAsyncKeyState(int('A')))
	{
		pos -= right * 0.05f;
	}
	else if (GetAsyncKeyState(int('D')))
	{
		pos += right * 0.05f;
	}
	setPosition((pos).toXMFLOAT3());

	OurMath::Mat4 rotation = OurMath::Mat4::Rotate(orientation.Conjugate());
	OurMath::Mat4 translation = OurMath::Mat4::Translate(pos.Negative());
	OurMath::Mat4 viewMatrix = rotation.Multiply(translation);


	setViewMatrix(viewMatrix);
}

OurMath::Quaternion FPSCamera::GetOrientation()
{
	return OurMath::Quaternion::RotationY(-m_yaw) * OurMath::Quaternion::RotationX(-m_pitch);
}

OurMath::Vec3 FPSCamera::GetForwardDirection(const OurMath::Quaternion & orientation)
{
	return OurMath::Quaternion::Rotate(orientation, OurMath::Vec3::ZAxis().Negative());
}

OurMath::Vec3 FPSCamera::GetRightDirection(const OurMath::Quaternion & orientation)
{
	return OurMath::Quaternion::Rotate(orientation, OurMath::Vec3::XAxis());
}
