#include "3DObject.h"
#include "../../core/Dx.h"

Object3D::Object3D()
{
	setVertexShader(DX::g_3DVertexShader);
	setPixelShader(DX::g_3DPixelShader);
	setHullShader(DX::g_standardHullShader);
	setDomainShader(DX::g_standardDomainShader);
	m_HENRIKTABORT = false;
}

std::string Object3D::toString() const
{
	return Shape::toString() + "\n";
}
