#pragma once
#include "State.h"

class MainMenu : public State {
private:

	void _init() override;

public:
	MainMenu(std::stack<Shape *>* pickingEvent, std::stack<int>* keyEvent, Camera* cam = nullptr);
	~MainMenu();

	// Inherited via State
	void Update(double deltaTime) override;
	void Draw() override;
};
