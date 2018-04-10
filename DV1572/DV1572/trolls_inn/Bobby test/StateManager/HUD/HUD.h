#pragma once
#include "../../../../ourEngine/interface/Interface.h"
#include <fstream>
#include <sstream>

class HUD
{
private:
	std::vector<Mesh*>				m_mesh;
	std::vector<RectangleShape*>	m_quadsNonClickAble;
	std::vector<RectangleShape*>	m_quadsClickAble;
	float							m_windowX;
	float							m_windowY;


private:
	void _cleanUp();

public:
	HUD();
	~HUD();
	void setWindowSize(float x, float y);
	bool LoadHud(const std::string &path);
	void CheckIfPicked();
	void Draw();
};