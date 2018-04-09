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

	bool m_firstPick;
	bool m_lastPick;
	Shape * m_firstPickedTile;
	Shape * m_middlePickedTile;
	Shape * m_lastPickedTile;


	virtual void	_init() override;
	void			_checkCreationOfRoom();


public:
	GameState(std::stack<Shape *>* pickingEvent, std::stack<int>* keyEvent, Camera* cam = nullptr);
	~GameState();

	// Inherited via State
	virtual void Update(double deltaTime) override;
	virtual void Draw() override;
};