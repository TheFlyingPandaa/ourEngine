#pragma once
#include "../../../../ourEngine/interface/Interface.h"
#include <fstream>
#include <sstream>

class HUD
{
private:
	struct PotentialAreaCircle
	{
		int x, y, r;
	};


	std::vector<Mesh*>				m_mesh;
	std::vector<RectangleShape*>	m_quadsNonClickAble;
	std::vector<RectangleShape*>	m_quadsClickAble;
	std::vector<PotentialAreaCircle>	m_potentialAreas;
	std::vector<Text>				m_texts;

private:
	void _cleanUp();

public:
	HUD();
	~HUD();
	bool LoadHud(const std::string &path);
	RectangleShape*	Pick(DirectX::XMFLOAT2 at);
	bool			isMouseInsidePotentialArea(DirectX::XMFLOAT2 mousePos);
	void	ResetColorsOnPickable();

	//(Path to texture, pos X, pos Y, scale X, scale Y, PickIndex)
	void	CreateRectangle(std::string path, float pX, float pY, float sX, float sY, float index);

	void CheckIfPicked();
	void Draw();
};