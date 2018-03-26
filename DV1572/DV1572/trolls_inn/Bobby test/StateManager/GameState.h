#pragma once
#include "State.h"
#include "MainMenu.h"

class GameState : public State {

public:
	GameState();
	~GameState();

	// Inherited via State
	virtual void update(double deltaTime) override;
	virtual void draw() override;
};