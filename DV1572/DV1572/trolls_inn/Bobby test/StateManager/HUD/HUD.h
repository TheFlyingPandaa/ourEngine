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
	std::vector<Text*>				m_texts;

private:
	void _cleanUp();
	void _setupAPotentialAreaCircle(int x, int y, int r, int relative);
	void _setupAPotentialAreaRect(int x, int y, int sx, int sy, int relative);
	bool _isMouseInsidePotentialAreaCircle(DirectX::XMFLOAT2 mousePos) const;
	bool _isMouseInsidePotentialAreaRect(DirectX::XMFLOAT2 mousePos) const;
	bool _checkAgainstCircle(DirectX::XMFLOAT2 mousePos, const RectangleShape & rect) const;
	bool _checkAgainstRectangle(DirectX::XMFLOAT2 mousePos, const RectangleShape & rect) const;
	bool _checkAgainstElipse(DirectX::XMFLOAT2 mousePos, const RectangleShape & rect) const;
public:
	HUD();
	~HUD();
	bool LoadHud(const std::string &path);
	RectangleShape*	Pick(DirectX::XMFLOAT2 at);
	void	ResetColorsOnPickableWithIndex(int index);
	int		getNrOfPickableButtons() const;
	int		PickHud(DirectX::XMFLOAT2 mousePos) const;
	void	SetColorOnButton(int index, float r, float g, float b);
	void	SetHoverColorOnButton(int index);
	void	SetPressColorOnButton(int index);
	void	ResetColorsExcept(int index);
	//void CheckIfPicked();
	void Draw();

	//THIS CAN BE CHANGED
	void addText(Text * text);
};