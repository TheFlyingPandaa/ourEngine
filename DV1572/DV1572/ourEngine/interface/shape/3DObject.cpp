#include "OurEngine.h"
#include "3DObject.h"

Object3D::Object3D()
{
	setVertexShader(DX::g_3DVertexShader);
#if DEFERRED_RENDERING
	setPixelShader(DX::g_3DPixelShader);
#elif FORWARD_RENDERING
	setPixelShader(DX::g_forwardPixelShader);
#endif
	setHullShader(DX::g_standardHullShader);
	setDomainShader(DX::g_standardDomainShader);
}

std::string Object3D::toString() const
{
	return Shape::toString() + "\n";
}
