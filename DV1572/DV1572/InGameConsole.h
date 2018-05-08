#pragma once
#include "trolls_inn\Bobby test\StateManager\HUD\HUD.h"
#include <direct.h>
#include <string.h>
#include <sstream>
#include <iostream>
const unsigned int textHight = 20;
const int32_t xPos = 8;
const unsigned int maxWidth = 32;
const unsigned int maxSize = 5;

const double startFade = 2.0f;
const double fadeTime = 1.0f;

class InGameConsole
{
	struct text_t {
		DirectX::XMINT2 position;
		int rows = 1;
		int32_t size = 1;
		double timer = 0;
		Text text;
	};
public:
	InGameConsole();
	~InGameConsole();
	static std::vector <text_t*> text;

	static void update(double deltaTime);
	static void draw();

	static void pushString(const std::string & string);
};

