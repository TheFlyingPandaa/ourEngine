#pragma once
#include "State.h"

class MainMenu : public State {
private:
	virtual void _init() override;

public:
	MainMenu(std::stack<Shape *>* pickingEvent, std::stack<int>* keyEvent, Camera* cam = nullptr);
	~MainMenu();

	// Inherited via State
	virtual void Update(double deltaTime) override;
	virtual void Draw() override;

	// Inherited via State
	virtual void DrawHUD() override;
};
