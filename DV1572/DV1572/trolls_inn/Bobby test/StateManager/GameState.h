#pragma once
#include "../Room/Grid.h"
#include "State.h"
#include "MainMenu.h"


class GameState : public State {
private:
	Mesh m;
	Mesh rect;
	Grid* grid;
	Grid* grid2;

	int posX;
	int posY;

	int previousKey;

	virtual void _init() override;
public:
	GameState(std::stack<Shape *>* pickingEvent, std::stack<int>* keyEvent, Camera* cam = nullptr);
	~GameState();

	// Inherited via State
	virtual void Update(double deltaTime) override;
	virtual void Draw() override;
};