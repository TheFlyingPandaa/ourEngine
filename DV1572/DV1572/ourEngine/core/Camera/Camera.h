#pragma once
#include <DirectXMath.h>
#include "../../interface/OurMath.h"

class Camera
{
private:

	const DirectX::XMVECTOR m_up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	DirectX::XMMATRIX m_viewMatrix;

protected:
	DirectX::XMFLOAT3 m_pos, m_lookAt;
	OurMath::Vec2 m_mousePos;
	OurMath::Vec3 m_rotation;
	OurMath::Vec3 m_position;
	OurMath::Vec3 m_focalPoint;
	OurMath::Vec2 m_lastMouse;
public:
	Camera();
	virtual ~Camera();
	Camera(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 lookAt);
	
	void setPosition(float x, float y, float z);
	void setPosition(DirectX::XMFLOAT3 pos);
	void Move(float x, float y, float z);
	void Move(DirectX::XMFLOAT3 direction);

	virtual void update() = 0; 

	void LookAt(float x, float y, float z);
	void LookAt(DirectX::XMFLOAT3 target);

	const DirectX::XMFLOAT3& getPosition() const;
	const DirectX::XMFLOAT3& getLookAt() const;

	void setViewMatrix();
	void setViewMatrix(const OurMath::Mat4& view);

	DirectX::XMMATRIX getViewMatrix() const;

	void setMousePos(OurMath::Vec2 mousePos);
	

private:
};
