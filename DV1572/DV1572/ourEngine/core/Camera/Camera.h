#pragma once
#include <DirectXMath.h>
//#include "../../interface/Input.h"

class Camera
{
private:
	DirectX::XMMATRIX m_viewMatrix;

protected:
	DirectX::XMVECTOR m_up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT3 m_pos, m_lookAt;
	DirectX::XMFLOAT2 m_lastMouse;
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

	DirectX::XMMATRIX getViewMatrix() const;

	void setMousePos(DirectX::XMFLOAT2 mousePos);

};
