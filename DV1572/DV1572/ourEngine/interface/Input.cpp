#include "Input.h"

bool Input::m_keys[256];

bool Input::m_mouseKeys[3];
DirectX::XMFLOAT2 Input::m_mousePos;
float Input::m_scrollDelta;

int Input::lastPressed;
DirectX::XMINT2 Input::m_windowSize;


Input::Input()
{
}

Input::~Input()
{

}

bool Input::isKeyPressed(int keyCode)
{
	return m_keys[keyCode];
}

bool Input::isMouseLeftPressed()
{
	return m_mouseKeys[0];
}

bool Input::isMouseRightPressed()
{
	return m_mouseKeys[2];
}

bool Input::isMouseMiddlePressed()
{
	return m_mouseKeys[1];
}

DirectX::XMFLOAT2 Input::getMousePosition()
{
	return m_mousePos;
}

DirectX::XMFLOAT2 Input::getMousePositionLH()
{
	DirectX::XMFLOAT2 pos;

	pos.x = m_mousePos.x;
	pos.y = static_cast<float>(m_windowSize.y) - m_mousePos.y;
	return pos;
}

float Input::getMouseScroll()
{
	float returnValue = m_scrollDelta;
	m_scrollDelta = 0.0f;
	return returnValue;
}

DirectX::XMINT2 Input::getWindowSize()
{
	return m_windowSize;
}

int Input::GetKeyIndex()
{
	return lastPressed;
}
