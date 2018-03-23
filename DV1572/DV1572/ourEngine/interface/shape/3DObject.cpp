#include "3DObject.h"
#include "../../core/Dx.h"

Object3D::Object3D()
{
	setVertexShader(DX::g_3DVertexShader);
	setPixelShader(DX::g_3DPixelShader);
}
