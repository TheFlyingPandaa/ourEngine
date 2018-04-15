#pragma once
#include "../State.h"
class SettingsMenu : public State
{
private:

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
	SettingsMenu(std::stack<Shape*>* pickingEvent, std::stack<int>* keyEvent, Camera * cam);
	~SettingsMenu();


	void Update(double deltaTime) override;
	void Draw() override;
	
};
