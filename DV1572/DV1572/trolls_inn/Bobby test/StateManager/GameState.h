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
#include "../../AI_Base/Inn.h"
#include "../../Events/EventHandler.h"
#include "../../Furniture/Table.h"
#include "../../AI_Base/HeightMapGenerator.h"
#include "HUD/ClickMenu.h"
#include "../../../ourEngine/interface/OurMusic.h"

class GameState : public State {
private:
	enum GameStage
	{
		Play,
		BuildRoom,
		CrewWindow,
		StatsWindow,
		EventWindow
	};

private:
	Grid* m_grid;
	RoomCtrl* m_roomctrl;
	Furniture* m_receptionFur;

	HeightMap m_background;

	DirectX::SoundEffectInstance* m_music;
	DirectX::SoundEffectInstance* m_hudClick;
	DirectX::SoundEffectInstance* m_hudSnap;

	// AI
	MasterAI* m_mai;
	std::chrono::high_resolution_clock::time_point m_pauseTimePoint;

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
	
	bool m_justMoved = false;

	Inn * inn;

	int m_in = 0;

	//EventCollection testtt;
	EventHandler * m_eventHandle;
	
	// Small Quality of Life additions
	Text m_levelText;
	RectangleShape m_levelBox;
	

	void			_init() override;
	void			_setHud();
	void			_handlePicking();
	void			_handlePickingAi(Shape * obj);
	bool			_handleHUDPicking();
	void			_handleInput();

	//std::vector<std::shared_ptr<Node>> getPathAndEatAss(XMINT2 startPosition, XMINT2 targetPosition);

public:
	GameState(std::stack<Shape *>* pickingEvent, std::stack<int>* keyEvent, Camera* cam = nullptr);
	~GameState();
	// Inherited via State
	virtual void Update(double deltaTime) override;
	virtual void Draw() override;
	virtual void DrawHUD() override;
};
