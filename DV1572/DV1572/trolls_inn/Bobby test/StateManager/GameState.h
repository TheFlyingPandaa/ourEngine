#pragma once
#include "../Room/Grid.h"
#include "State.h"
#include "MainMenu.h"
#include "../Character.h"
#include <vector>
#include <algorithm>
#include "HUD/HUD.h"
#include <thread>
#include <future>


class GameState : public State {
private:
	enum GameStage
	{
		Play,
		BuildRoom
	};


private:
	Mesh m;
	Mesh kitchenTile;
	Mesh rect;
	Grid* grid;


	DirectX::XMINT2 m_mainDoorPos;

	Mesh door;

	Object3D test;

	Object3D d;

	int posX;
	int posY;

	GameTime gameTime; 
	GameStage m_stage;


	int previousKey;
	bool m_Rpressed;

	// HUD
	enum HudPickingStage
	{
		Miss,
		Hover,
		Click

	};
	HudPickingStage m_hudPickStage;
	bool m_colorButton;
	bool m_hasClicked;
	int m_lastPickedIndex;

	//TEST
	Mesh box;
	Character c;
	
	bool m_move = false;
	bool m_justMoved = false;

	int m_i = 0;
	std::future<void> future;

	Mesh table;
	


	void			_init() override;
	void			_setHud();
	void			_handlePicking();
	void			_handlePickingAi(Shape * obj);
	void			_handleHUDPicking(RectangleShape* r);
	void			_handleInput();

public:
	GameState(std::stack<Shape *>* pickingEvent, std::stack<int>* keyEvent, Camera* cam = nullptr);
	~GameState();
	// Inherited via State
	virtual void Update(double deltaTime) override;
	virtual void Draw() override;
	virtual void DrawHUD() override;
};
