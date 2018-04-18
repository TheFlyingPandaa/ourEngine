#include "Billboard.h"
#include "../../core/Dx.h"

Billboard::Billboard()
{
	setVertexShader(DX::g_billboardVertexShader);
	setPixelShader(DX::g_billboardPixelShader);
}

std::string Billboard::toString() const
{
	return Shape::toString() + "\n";
}
