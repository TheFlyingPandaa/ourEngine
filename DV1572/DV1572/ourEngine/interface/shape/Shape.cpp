#include "Shape.h"
#include "../../core/Dx.h"
void Shape::setVertexShader(ID3D11VertexShader * s)
{
	m_vs = s;
}
void Shape::setHullShader(ID3D11HullShader * s)
{
	m_hs = s;
}
void Shape::setDomainShader(ID3D11DomainShader * s)
{
	m_ds = s;
}
void Shape::setGeometryShader(ID3D11GeometryShader * s)
{
	m_gs = s;
}
void Shape::setPixelShader(ID3D11PixelShader * s)
{
	m_ps = s;
}
Shape::Shape()
{
	m_mesh	= nullptr;
	m_vs	= nullptr;
	m_hs	= nullptr;
	m_ds	= nullptr;
	m_gs	= nullptr;
	m_ps	= nullptr;
}

void Shape::setMesh(Mesh * m)
{
	m_mesh = m;
}

Mesh * Shape::getMesh() const
{
	return m_mesh;
}

ID3D11Buffer * Shape::getVertices() const
{
	return m_mesh->getVertices();
}

void Shape::Draw()
{
	DX::g_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DX::g_deviceContext->IASetInputLayout(DX::g_inputLayout);

	DX::g_deviceContext->VSSetShader(DX::g_vertexShader, nullptr, 0);
	DX::g_deviceContext->HSSetShader(nullptr, nullptr, 0);
	DX::g_deviceContext->DSSetShader(nullptr, nullptr, 0);
	DX::g_deviceContext->GSSetShader(nullptr, nullptr, 0);
	DX::g_deviceContext->PSSetShader(DX::g_pixelShader, nullptr, 0);

	UINT32 vertexSize = sizeof(VERTEX);
	UINT offset = 0;

	ID3D11Buffer* v = m_mesh->getVertices();
	DX::g_deviceContext->IASetVertexBuffers(0, 1, &v, &vertexSize, &offset);
	DX::g_deviceContext->Draw(m_mesh->getNumberOfVertices(), 0);
}
