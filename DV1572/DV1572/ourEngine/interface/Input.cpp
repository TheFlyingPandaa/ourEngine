#include "Input.h"

bool Input::m_keys[256];

bool Input::m_mouseKeys[3];
DirectX::XMFLOAT2 Input::m_mousePos;
float Input::m_scrollDelta;

Input::Input()
{
}

Input::~Input()
{

}

void Input::_reset()
{
	m_scrollDelta = 0.0f;
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

float Input::getMouseScroll()
{
	return m_scrollDelta;
}
