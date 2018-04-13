#pragma once
#include "State.h"
#include "Menu/TopMenu.h"
class MainMenu : public State {
private:

	std::stack<State *> m_subMenus;
	
	void _handleInput();
	void _handleHUDPicking(RectangleShape* r);
	void _handlePicking();
	void _init() override;

	enum HudPickingStage
	{
		Miss,
		Hover,
		Click

	};
	HudPickingStage m_hudPickStage;

	bool m_hasClicked;
	bool m_colorButton;
	int m_lastPickedIndex;

public:
	MainMenu(std::stack<Shape *>* pickingEvent, std::stack<int>* keyEvent, Camera* cam = nullptr);
	~MainMenu();

	// Inherited via State
	void Update(double deltaTime) override;
	void Draw() override;


};
