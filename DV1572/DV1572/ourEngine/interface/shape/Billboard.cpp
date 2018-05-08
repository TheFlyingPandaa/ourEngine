#include "Billboard.h"
#include "../../core/Dx.h"

Billboard::Billboard()
{
	setVertexShader(DX::g_billboardVertexShader);
	setPixelShader(DX::g_billboardPixelShader);
}

int Billboard::getSpriteIndex() const
{
	return m_spriteIndex;
}

void Billboard::setSpriteIndex(int index)
{
	m_spriteIndex = index;
}

std::string Billboard::toString() const
{
	return Shape::toString() + "\n";
}
