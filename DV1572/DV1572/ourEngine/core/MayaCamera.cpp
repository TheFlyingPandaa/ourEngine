#include "MayaCamera.h"

MayaCamera::MayaCamera()
{
	m_panSpeed = 0.015f;
	m_rotationSpeed = 0.002f;
	m_zoomSpeed = 0.2f;

	m_position = OurMath::Vec3(0.0f, 0.0f, 0.0f);
	m_rotation = OurMath::Vec3(90.0f, 0.0f, 0.0f);

	m_focalPoint = OurMath::Vec3(0.0f, 0.0f, 0.0f);
	m_distance = OurMath::Length(m_position - m_focalPoint);

	m_yaw = 3.0f * OurMath::PI / 4.0f;
	m_pitch = OurMath::PI / 4.0f;
	
}

void MayaCamera::update()
{
	
	OurMath::Vec2 mouse = m_mousePos;
	OurMath::Vec2 delta = mouse - m_lastMouse;
	m_lastMouse = mouse;

	if (GetAsyncKeyState(VK_LBUTTON))
	{
		MousePan(delta);
	}
	if (GetAsyncKeyState(VK_RBUTTON))
	{
		MouseRotate(delta);
	}
	if (GetAsyncKeyState(VK_MBUTTON))
	{
		MouseZoom(delta.y);
	}

	
	m_position = CalculatePosition();
	OurMath::Quaternion orientation = GetOrientation();
	m_rotation = orientation.ToEulerAngles().toDegrees();

	OurMath::Mat4 viewMatrix = OurMath::Mat4::Translate(OurMath::Vec3(0, 0, 1)) * OurMath::Mat4::Rotate(orientation.Conjugate()) * OurMath::Mat4::Translate(m_position.Negative());

	setViewMatrix(viewMatrix);
}

void MayaCamera::MouseRotate(const OurMath::Vec2 & delta)
{
	float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
	m_yaw += yawSign * delta.x * m_rotationSpeed;
	m_pitch += delta.y * m_rotationSpeed;
}

void MayaCamera::MousePan(const OurMath::Vec2 & delta)
{
	m_focalPoint += GetRightDirection().Negative() * delta.x * m_panSpeed;
	m_focalPoint += GetUpDirection() * delta.y * m_panSpeed;
}

void MayaCamera::MouseZoom(float delta)
{
	m_distance -= delta * m_zoomSpeed;
	if (m_distance < 1.0f)
	{
		m_focalPoint += GetForwardDirection();
		m_distance = 1.0f;
	}
}

OurMath::Vec3 MayaCamera::CalculatePosition()
{

	return m_focalPoint - GetForwardDirection() * m_distance;
}

OurMath::Vec3 MayaCamera::GetRightDirection()
{

	return OurMath::Quaternion::Rotate(GetOrientation(), OurMath::Vec3::XAxis());
}

OurMath::Quaternion MayaCamera::GetOrientation()
{
	return OurMath::Quaternion::RotationY(-m_yaw) * OurMath::Quaternion::RotationX(-m_pitch);
}

OurMath::Vec3 MayaCamera::GetUpDirection()
{
	return OurMath::Quaternion::Rotate(GetOrientation(), OurMath::Vec3::YAxis());
}

OurMath::Vec3 MayaCamera::GetForwardDirection()
{
	return OurMath::Quaternion::Rotate(GetOrientation(), OurMath::Vec3::ZAxis().Negative());
}
