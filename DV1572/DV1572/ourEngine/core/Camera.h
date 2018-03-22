#pragma once
#include <DirectXMath.h>

class Camera
{
private:
	DirectX::XMFLOAT3 m_pos, m_lookAt;
	const DirectX::XMVECTOR m_up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
public:
	Camera();
	Camera(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 lookAt);
	
	void setPosition(float x, float y, float z);
	void setPosition(DirectX::XMFLOAT3 pos);
	void Move(float x, float y, float z);
	void Move(DirectX::XMFLOAT3 direction);

	void LookAt(float x, float y, float z);
	void LookAt(DirectX::XMFLOAT3 target);

	const DirectX::XMFLOAT3& getPosition() const;
	const DirectX::XMFLOAT3& getLookAt() const;

	DirectX::XMMATRIX getViewMatrix() const;
};
