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
	bool m_isPlaceable;
	Shape * m_firstPickedTile;
	Shape * m_middlePickedTile;
	Shape * m_lastPickedTile;

	DirectX::XMINT2 m_prevStart;
	DirectX::XMINT2 m_prevEnd;


	virtual void	_init() override;
	void			_checkCreationOfRoom();


public:
	GameState(std::stack<Shape *>* pickingEvent, std::stack<int>* keyEvent, Camera* cam = nullptr);
	~GameState();

	// Inherited via State
	virtual void Update(double deltaTime) override;
	virtual void Draw() override;
};