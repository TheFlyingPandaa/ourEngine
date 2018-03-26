#pragma once
#include "State.h"

class MainMenu : public State {
private:


public:
	MainMenu();
	~MainMenu();

	// Inherited via State
	virtual void update(double deltaTime) override;
	virtual void draw() override;

};
