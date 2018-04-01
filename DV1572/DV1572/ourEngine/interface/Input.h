#ifndef INPUT_H
#define INPUT_H
#include <DirectXMath.h>
#include "Window.h"
class Input
{
	friend class Window;
private:
	static bool m_keys[256];

	static bool m_mouseKeys[3];
	static DirectX::XMFLOAT2 m_mousePos;
	static float m_scrollDelta;

	Input();
	~Input();

	static void _reset();

public:
	static Input& Instance()
	{
		static Input input;
		return input;
	}
	static bool isKeyPressed(int keyCode);

	static bool isMouseLeftPressed();
	static bool isMouseRightPressed();
	static bool isMouseMiddlePressed();
	static DirectX::XMFLOAT2 getMousePosition();
	static float getMouseScroll();
};
#endif // !INPUT_H

