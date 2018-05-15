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
	static DirectX::XMINT2 m_windowSize;

	static int lastPressed;

	Input();
	~Input();

public:
	enum Key
	{
		Del = 46,
		LeftArrow = 37,
		UpArrow = 38,
		RightArrow = 39,
		DownArrow = 40,
		SPACEBAR = 0x20
		// add more if needed
	};

	// Singleton class, only one instance
	static Input& Instance()
	{
		static Input input;
		return input;
	}

	/*
		The argument is a character in integer. 

		Example:
		if(isKeyPressed('A')) 

		Returns
			true, if the specified key is pressed
			false, if the specified key is not pressed
	*/
	static bool isKeyPressed(int keyCode);

	/*
		Returns
			true, if the left mouse button is pressed
			false, if the left mouse button is not pressed
	*/
	static bool isMouseLeftPressed(bool repeat = true);

	/*
		Returns
			true, if the right mouse button is pressed
			false, if the right mouse button is not pressed
	*/
	static bool isMouseRightPressed();

	/*
		Middle mouse button is the scroll button
		Returns
			true, if the middle mouse button is pressed
			false, if the middle mouse button is not pressed
	*/
	static bool isMouseMiddlePressed();
	/*
		Gets the mouse position in screen space. This position is 
		relative to the client window. And Origo is in the top left.
		Returns:
			XMFLOAT2.x, mouse x-coordinate
			XMFLOAT2.y, mouse y-coordinate
	*/
	static DirectX::XMFLOAT2 getMousePosition();
	/*
		Gets the mouse position in screen space. This position is
		relative to the client window. And Origo is in the bottom left.
		Returns:
			XMFLOAT2.x, mouse x-coordinate
			XMFLOAT2.y, mouse y-coordinate
	*/
	static DirectX::XMFLOAT2 getMousePositionLH();

	/*
		!Exprimental!
		Get the direction of the mouse scroll. Calling
		this function consumes the resource, meaning that subsequentual
		calls will result in zero scroll. 
	*/
	static float getMouseScroll();
	/*
		Gets the size of the window as a XMINT2 where x is the windowSizeX and y is the windowSizeY;
	
	*/
	static DirectX::XMINT2 getWindowSize();
	
	static int GetKeyIndex();
};
#endif // !INPUT_H

