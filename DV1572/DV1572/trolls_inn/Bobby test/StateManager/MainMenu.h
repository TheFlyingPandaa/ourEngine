#pragma once
#include "State.h"

class MainMenu : public State {
private:


public:
	MainMenu();
	~MainMenu();

	// Inherited via State
	virtual void Update(double deltaTime) override;
	virtual void Draw() override;

};
