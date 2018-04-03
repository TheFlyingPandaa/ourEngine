#include "Camera.h"

using namespace DirectX;
Camera::Camera()
{
	m_pos = XMFLOAT3{ -38,0,40 };
	m_lookAt = XMFLOAT3{ 0,0,-1 };
}

Camera::~Camera()
{
}

Camera::Camera(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 lookAt)
{
	m_pos = pos;
	XMVECTOR newLookAt = XMVector3Normalize(XMLoadFloat3(&lookAt));
	XMStoreFloat3(&m_lookAt, newLookAt);
}

void Camera::setPosition(float x, float y, float z)
{
	setPosition(XMFLOAT3(x, y, z));
}

void Camera::setPosition(DirectX::XMFLOAT3 pos)
{
	m_pos = pos;
}

void Camera::Move(float x, float y, float z)
{
	Move(XMFLOAT3(x, y, z));
}

void Camera::Move(DirectX::XMFLOAT3 direction)
{
	XMVECTOR newPos = XMLoadFloat3(&m_pos) + XMLoadFloat3(&direction);
	XMStoreFloat3(&m_pos, newPos);
}

void Camera::LookAt(float x, float y, float z)
{
	LookAt(XMFLOAT3(x, y, z));
}

void Camera::LookAt(DirectX::XMFLOAT3 target)
{
	XMVECTOR newLookAt = XMVector3Normalize(XMLoadFloat3(&target));
	XMStoreFloat3(&m_lookAt, newLookAt);
}

const DirectX::XMFLOAT3& Camera::getPosition() const
{
	return m_pos;
}

const DirectX::XMFLOAT3& Camera::getLookAt() const
{
	return m_lookAt;
}

void Camera::setViewMatrix()
{
	m_viewMatrix = XMMatrixLookAtLH(
		XMLoadFloat3(&m_pos),
		XMLoadFloat3(&m_pos) + XMLoadFloat3(&m_lookAt),
		m_up);
}

DirectX::XMMATRIX Camera::getViewMatrix() const
{

	return m_viewMatrix;
	
}

void Camera::setMousePos(XMFLOAT2 mousePos)
{
	m_mousePos = mousePos;
}
