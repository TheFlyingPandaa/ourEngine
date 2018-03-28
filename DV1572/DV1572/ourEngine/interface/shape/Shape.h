#pragma once
#include <DirectXMath.h>
#include "Mesh.h"

class Shape
{
private:
	DirectX::XMFLOAT3		m_pos, m_scl, m_rot;
	Mesh*					m_mesh;
	ID3D11VertexShader*		m_vs;
	ID3D11HullShader*		m_hs;
	ID3D11DomainShader*		m_ds;
	ID3D11GeometryShader*	m_gs;
	ID3D11PixelShader*		m_ps;

	DirectX::XMMATRIX		m_worldMatrix; 

private: 
	void _buildMatrix();
	DirectX::XMFLOAT3 _convertToRad(DirectX::XMFLOAT3 deg);

protected:
	void setVertexShader(ID3D11VertexShader* s);
	void setHullShader(ID3D11HullShader* s);
	void setDomainShader(ID3D11DomainShader* s);
	void setGeometryShader(ID3D11GeometryShader* s);
	void setPixelShader(ID3D11PixelShader* s);

public:
	Shape();
	void setMesh(Mesh* m);
	Mesh* getMesh() const;
	ID3D11Buffer* getVertices() const;
	void setPos(float x, float y, float z);
	void setPos(DirectX::XMFLOAT3 pos);
	void Move(float x, float y, float z);
	void Move(DirectX::XMFLOAT3 move);
	void setRotation(float x, float y, float z);
	void setRotation(DirectX::XMFLOAT3 rotation);
	void Rotate(float x, float y, float z);
	void Rotate(DirectX::XMFLOAT3 rotation);
	void setScale(float scl);
	void setScale(float x, float y, float z);
	void setScale(DirectX::XMFLOAT3 scl);
	void Scale(float scl);
	void Scale(float x, float y, float z);
	void Scale(DirectX::XMFLOAT3 scl);
	
	std::string toString() const;

	void ApplyShaders();
	void CheckPick();
	const DirectX::XMMATRIX& getWorld() const; 

	void Draw();
	void TEMPTRANS();
};