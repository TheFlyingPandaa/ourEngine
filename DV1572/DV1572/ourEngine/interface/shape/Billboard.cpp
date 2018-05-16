#include "OurEngine.h"
#include "Billboard.h"

Billboard::Billboard()
{
	setVertexShader(DX::g_billboardVertexShader);
#if DEFERRED_RENDERING
	setPixelShader(DX::g_billboardPixelShader);
#elif FORWARD_RENDERING
	setPixelShader(DX::g_forwardPixelShader);
#endif
	m_spriteIndex = 0; 
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
