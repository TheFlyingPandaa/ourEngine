#pragma once
#include "../Room/Grid.h"
#include "State.h"
#include "MainMenu.h"
#include "../Character.h"
#include <vector>
#include <algorithm>
#include "HUD/HUD.h"



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
	Grid* grid2;

	Mesh door;

	Object3D d;

	int posX;
	int posY;

	GameTime gameTime; 
	GameStage m_stage;


	int previousKey;
	bool m_Rpressed;






	// Build Mode
	enum BuildStage
	{
		None,
		Start,
		Selection,
		End
	};
	
	RoomType m_selectedRoomType;
	Shape * m_startTile;
	Shape * m_selectedTile;
	BuildStage m_buildStage;
	bool m_roomPlaceable;
	// Build Mode END


	DirectX::XMINT2 m_prevStart;
	DirectX::XMINT2 m_prevEnd;

	//TEST
	Mesh box;
	Character c;
	
	bool m_move = false;


	virtual void	_init() override;
	void			_handleBuildRoom(Shape * s);
	void			_setHud();
	void			_handlePicking();
	void			_handleInput();

public:
	GameState(std::stack<Shape *>* pickingEvent, std::stack<int>* keyEvent, Camera* cam = nullptr);
	~GameState();

	// Inherited via State
	virtual void Update(double deltaTime) override;
	virtual void Draw() override;
};