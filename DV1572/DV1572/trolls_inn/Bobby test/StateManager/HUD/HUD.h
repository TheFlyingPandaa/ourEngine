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
	void _setupAPotentialAreaCircle(int x, int y, int r, int relative);
public:
	HUD();
	~HUD();
	bool LoadHud(const std::string &path);
	RectangleShape*	Pick(DirectX::XMFLOAT2 at);
	bool			isMouseInsidePotentialArea(DirectX::XMFLOAT2 mousePos);
	void	ResetColorsOnPickable();

	void CheckIfPicked();
	void Draw();
};