#pragma once
#include "../Room/Grid.h"
#include "State.h"
#include "MainMenu.h"
#include <vector>
#include <algorithm>
#include "HUD/HUD.h"
#include <thread>
#include <future>
#include "../../AI_Base/MasterAI.h"

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
	
	Grid* m_grid;
	RoomCtrl* m_roomctrl;
	
	MasterAI* m_mai;

	GameTime gameTime; 
	GameStage m_stage;


	int previousKey;
	bool m_Rpressed;

	// HUD
	bool m_clickedLastFrame;
	bool m_madeFullReset;
	int m_lastClickedIndex;
	std::vector<bool> m_hudButtonsPressed;
	void _resetHudButtonPressedExcept(int index);
	
	bool m_move = false;
	bool m_justMoved = false;

	int m_i = 0;
	std::future<void> future;

	Mesh table;
	


	void			_init() override;
	void			_setHud();
	void			_handlePicking();
	void			_handlePickingAi(Shape * obj);
	bool			_handleHUDPicking();
	void			_handleInput();

public:
	GameState(std::stack<Shape *>* pickingEvent, std::stack<int>* keyEvent, Camera* cam = nullptr);
	~GameState();
	// Inherited via State
	virtual void Update(double deltaTime) override;
	virtual void Draw() override;
	virtual void DrawHUD() override;
};
