#pragma once
#include "trolls_inn\Bobby test\StateManager\HUD\HUD.h"
#include <direct.h>
#include <string.h>
#include <sstream>
#include <iostream>
const unsigned int textHight = 16;
const DirectX::XMINT2 startPosition(8,360);
const unsigned int maxWidth = 32;
const unsigned int maxSize = 32;
class InGameConsole
{
	struct text_t {
		DirectX::XMINT2 position;
		int rows = 0;
		Text text;
	};
public:
	InGameConsole();
	~InGameConsole();
	static std::vector <text_t*> text;

	static void update();
	static void draw();

	static void pushString(const std::string & string);
};

