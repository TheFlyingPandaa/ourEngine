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
	struct PotentialAreaRect
	{
		int x, y, sx, sy;
	};

	std::vector<Mesh*>				m_mesh;
	std::vector<RectangleShape*>	m_quadsNonClickAble;
	std::vector<RectangleShape*>	m_quadsClickAble;
	std::vector<PotentialAreaCircle>	m_potentialAreasCircle;
	std::vector<PotentialAreaRect>	m_potentialAreasRect;
	std::vector<Text>				m_texts;

private:
	void _cleanUp();
	void _setupAPotentialAreaCircle(int x, int y, int r, int relative);
	void _setupAPotentialAreaRect(int x, int y, int sx, int sy, int relative);
public:
	HUD();
	~HUD();
	bool LoadHud(const std::string &path);
	RectangleShape*	Pick(DirectX::XMFLOAT2 at);
	bool	isMouseInsidePotentialAreaCircle(DirectX::XMFLOAT2 mousePos);
	bool	isMouseInsidePotentialAreaRect(DirectX::XMFLOAT2 mousePos);
	void	ResetColorsOnPickableWithIndex(int index);
	int		getNrOfPickableButtons() const;

	void CheckIfPicked();
	void Draw();
};