#pragma once
#include "../Grid.h"
#include "State.h"
#include "MainMenu.h"


class GameState : public State {
private:
	
	Object3D obj;
	Mesh m;

	Grid* grid;

	virtual void _init() override;
public:
	GameState(std::stack<Shape *>* pickingEvent, std::stack<int>* keyEvent, Camera* cam = nullptr);
	~GameState();

	// Inherited via State
	virtual void Update(double deltaTime) override;
	virtual void Draw() override;
};