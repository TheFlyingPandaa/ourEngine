#include "GameState.h"
#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <thread>
#include <future>
#include "../../../ourEngine/interface/light/PointLight.h"
#include "../../../ourEngine/core/Dx.h"
#include "../StateManager/SubStates/BuildState.h"
#include "../StateManager/SubStates/CrewState.h"
#include "../StateManager/SubStates/StatsState.h"
#include "../StateManager/SubStates/EventsState.h"
#include "../../Mesh Manager/MeshManager.h"
#include "../../Mesh Manager/MeshLoaderPlus.h"
#include "../../Furniture/Reception.h"
#include "../../Furniture/Table.h"

GameState::GameState(std::stack<Shape*>* pickingEvent, std::stack<int>* keyEvent, Camera * cam) : State(pickingEvent, keyEvent)
{
	m_stage = GameStage::Play;
	m_clickedLastFrame = false;
	m_madeFullReset = true;
	m_lastClickedIndex = -1;
	// Building END
	m_Rpressed = false;

	int startSize = 32;
	int firstRoomSizeX = 6;
	int firstRoomSizeY = 5;

	int secondRoomSizeX = 4;
	int secondRoomSizeY = 2;

	this->_init();
	m_grid = new Grid(0, 0, startSize, startSize);
	m_roomctrl = new RoomCtrl();
	inn = new Inn();

	XMINT2 targetPosition = { inn->getReceptionPos().x, inn->getReceptionPos().y };
	XMINT2 startPosition = { 0, 0 };
	auto path1 = std::vector<std::shared_ptr<Node>>();// getPathAndEatAss(startPosition, targetPosition);
	XMINT2 targetPosition2 = { 32, 0 };
	XMINT2 startPosition2 = { inn->getReceptionPos().x , inn->getReceptionPos().y };
	auto path2 = std::vector<std::shared_ptr<Node>>();// getPathAndEatAss(startPosition, targetPosition);

	m_eventHandle = new EventHandler(inn, m_roomctrl,path1,path2);
	_setHud();

	int nrOfButtons = m_stateHUD.getNrOfPickableButtons();
	{
	for (int i = 0; i < nrOfButtons; i++)
		m_hudButtonsPressed.push_back(false);
	}

	this->m_cam = cam;

	m_mai = new MasterAI(m_roomctrl, m_grid, inn);
	previousKey = -1;


	dynamic_cast<OrbitCamera*>(m_cam)->Init();
	m_receptionFur = new Reception(XMFLOAT3(0, 0, 0), MESH::RECEPTION_LOW);

	m_receptionFur->setPosition(XMFLOAT3(17.5, 0, 7.5));
	m_receptionFur->setRotation(270);
	m_roomctrl->AddRoomObject(m_receptionFur);
	delete m_receptionFur;

	MLP::GetInstance().LoadMeshRectangle("lvlBox");
	MLP::GetInstance().GetMesh("lvlBox")->setDiffuseTexture("trolls_inn/Resources/HUD/MainHud/ButtonBackground.png");

	m_levelBox.setMesh(MLP::GetInstance().GetMesh("lvlBox"));
	m_levelBox.setScreenPos(Input::getWindowSize().x - 200, 10, 0.1f);
	m_levelBox.setWidth(200);
	m_levelBox.setHeight(180);

	m_levelText.setAllignment(TXT::Center);
	m_levelText.setRelative(Text::BL);
	m_levelText.setColor(0, 0, 0);
	m_levelText.setScale(0.5f);
	m_levelText.setPosition(Input::getWindowSize().x - 100, 120);
	
}

GameState::~GameState()
{
	delete m_roomctrl;
	delete m_grid;
	delete m_mai;
	while (!m_subStates.empty())
	{
		delete m_subStates.top();
		m_subStates.pop();
	}
	delete inn;
	//delete m_receptionFur;
	delete m_eventHandle;
	delete m_music;
	delete m_hudSnap;
	delete m_hudClick;
}


void GameState::Update(double deltaTime)
{
	m_levelText.setTextString(inn->getStatus());
	using namespace std::chrono_literals;
	if (inn->getExitState())
	{
		m_exitState = true;
	}
	if (inn->getAngryCustomers() >= inn->getAngryCustomersCap())
	{
		m_exitState = true;
	}

	m_eventHandle->Update(deltaTime);


	if (m_subStates.empty())
	{
		m_stage = GameStage::Play;
	}

	_handlePicking();	// It's important this is before handleInput();
	_handleInput();		// It's important this is after handlePicking();
	
	static bool lol = false;
	bool lol2 = Input::isKeyPressed('L');
	if (lol2 && !lol)
	{
		this->m_mai->spawn();
	}
	lol = lol2;

	if (Input::isKeyPressed('0'))
		m_mai->spawn();



	this->m_cam->update();
	m_roomctrl->Update(m_cam);
	if (inn->GetRecievedReview())
	{
		m_stateHUD.SlideMeterBarWithIndex(0, inn->GetInnAttributes().GetStat(), 0);
		inn->SetRecievedReviewToFalse();
	}

	if (!m_subStates.empty())
	{
		m_subStates.top()->Update(deltaTime);
		if (m_subStates.top()->exitState()) {
			delete m_subStates.top();
			m_subStates.pop();
		}
		else
		{
			SubState * ref = m_subStates.top()->newState();
			if (ref)
				m_subStates.push(ref);
		}
	}
	inn->Update(deltaTime, gameTime.getTimePeriod());
	
	if (Input::isKeyPressed('Y'))
		inn->Deposit(500);
	
	if (Input::isKeyPressed('U'))
		inn->Withdraw(500);

	m_mai->Update(m_cam);
	gameTime.updateCurrentTime(static_cast<float>(deltaTime));
}

void GameState::Draw()
{
	gameTime.m_cpyLightToGPU();
	
	m_roomctrl->Draw();
	if (m_grid)
	{
		this->m_grid->Draw();
	}

	m_mai->Draw();
	if (!m_subStates.empty())
		m_subStates.top()->Draw();

	m_background.Draw();

	if (inn)
		inn->Draw();

	m_levelBox.DrawAsHud();
	m_levelText.Draw();
}

void GameState::DrawHUD()
{
	m_stateHUD.Draw();
	if (!m_subStates.empty())
	{
		m_subStates.top()->DrawHUD();
		return;
	}


}

void GameState::_resetHudButtonPressedExcept(int index)
{
	for (int i = 0; i < m_hudButtonsPressed.size(); i++)
	{
		if (i != index)
		{
			m_hudButtonsPressed[i] = false;
			m_stateHUD.ResetColorsOnPickableWithIndex(i);
		}
	}
}

void GameState::_init()
{
	m_music = OurMusic::LoadSound("trolls_inn/Resources/sounds/ambient_background.wav");
	m_hudClick = OurMusic::LoadSound("trolls_inn/Resources/sounds/click_menu.wav");
	m_hudSnap = OurMusic::LoadSound("trolls_inn/Resources/sounds/snap_menu.wav");
	m_hudSnap->SetVolume(0.5f);
	if (m_music)
		m_music->Play(true);
}

void GameState::_setHud()
{
	m_stateHUD.LoadHud("trolls_inn/Resources/HUD/MainHud/MainHud.txt");
	m_stateHUD.addText(inn->GetText());
}

void GameState::_handlePicking()
{
	bool hudWasPicked = _handleHUDPicking();
	if (m_stage == GameStage::Play && Input::isMouseLeftPressed())
	{
		m_grid->PickTiles();
		m_roomctrl->PickRoomTiles();
		m_mai->PickCustomers();
		
	}

	if (hudWasPicked)
	{
		while (!p_pickingEvent->empty()) this->p_pickingEvent->pop();
	}

	if (p_pickingEvent->empty())
	{
		if (!m_subStates.empty())
		{
			SubState* ss = m_subStates.top();
			ss->HandlePicking(nullptr);
		}
	}
	while (!p_pickingEvent->empty())
	{
		Shape * obj = this->p_pickingEvent->top();
		
		this->p_pickingEvent->pop();


		if (!m_subStates.empty())
		{
			SubState* ss = m_subStates.top();
			ss->HandlePicking(obj);
		}

	
		if (m_stage == GameStage::Play && Input::isMouseLeftPressed())
		{
			_handlePickingAi(obj);
		}

	}
}
// round float to n decimals precision
float round_n(float num, int dec)
{
	float m = (num < 0.0f) ? -1.0f : 1.0f;   // check if input is negative
	float pwr = pow(10.0f, dec);
	return float((float)floor((double)num * m * pwr + 0.5) / pwr) * m;
}
void GameState:: _handlePickingAi(Shape * obj)
{
	// If the the y position is 0, then this is a ground tile
	// else we probably hit an customer
	if (obj->getPosition().y == 0)
	{
		//std::cout << "Tile" << std::endl;
		Staff* troll = m_mai->getTroll();

		float xLol, yLol;
		troll->getStepsLeft(xLol, yLol);
		//xLol = yLol = 0.0f;
		troll->clearWalkingQueue();

		//Shape * obj = this->p_pickingEvent->top();
		XMFLOAT2 trollPos = troll->getPosition(); // (x,y) == (x,z,0)
		trollPos.x += xLol;
		trollPos.y += yLol;

		int xTile = (int)trollPos.x;// (int)(round_n(trollPos.x, 1) - 0.5f);
		int yTile = (int)trollPos.y;//(int)(round_n(trollPos.y, 1) - 0.5f);


		XMINT2 targetPosition = { (int)obj->getPosition().x , (int)obj->getPosition().z };

		XMINT2 startPosition = { xTile, yTile };
		
		auto path = m_mai->getSolverPtr()->GetPathAndSmokeGrass(startPosition, targetPosition);


		XMFLOAT3 oldPos = { float(xTile),0.0f, float(yTile) };

		if (path.size() != 0)
		{
			if(!troll->getCancelFlag())
				troll->setCancelFlag(true);
			m_justMoved = false;

			troll->Move(troll->getDirectionFromPoint(oldPos, path[0]->tile->getQuad().getPosition()));

			for (int i = 0; i < path.size() - 1; i++)
			{
				troll->Move(troll->getDirectionFromPoint(path[i]->tile->getQuad().getPosition(), path[i + 1]->tile->getQuad().getPosition()));
			}
		}
	}
	else
	{
		m_mai->PickedCustomerShape(obj);
	}
	
	
}


bool GameState::_handleHUDPicking()
{
	bool pickedHUD = false;
	int index = m_stateHUD.PickHud(Input::getMousePositionLH());
	static int lastHover = -1;

	if (index != -2) pickedHUD = true;
	if (index < 0)
		lastHover = -1;
	
	if (index >= 0)
	{
		float cH = 5.0f;
		float cHL = 2.0f;
		float cC = 50.0f;
		m_madeFullReset = false;
		if (!m_clickedLastFrame && Input::isMouseLeftPressed())
		{
			if (m_hudClick->GetState() == DirectX::SoundState::PLAYING)
				m_hudClick->Stop();
			m_hudClick->Play();
			m_lastClickedIndex = index;
			// Clicked a button
			while (!m_subStates.empty())
			{
				SubState * ss = m_subStates.top();
				m_subStates.pop();

				// Update paused time points
				std::chrono::duration<double, std::ratio<1, 1>> d;
				std::chrono::high_resolution_clock::time_point now = m_mai->SaveTimePoint();
				d = std::chrono::duration_cast<std::chrono::duration<double>>(now - m_pauseTimePoint);
				m_mai->UpdateCustomerSpawnTimePoint(d);
				m_mai->UpdateCustomerNeedsTimePoint(d);
				delete ss;
			}
			_resetHudButtonPressedExcept(index);
			m_hudButtonsPressed[index] = !m_hudButtonsPressed[index];
			m_clickedLastFrame = true;

			switch (index)
			{
			case 0:
				// Build Button
				m_stateHUD.SetColorOnButton(index, cHL, cC, cHL);
				m_pauseTimePoint = m_mai->SaveTimePoint();
				if (m_hudButtonsPressed[index]){
					m_subStates.push(new BuildState(m_cam, p_pickingEvent, m_grid, m_roomctrl, inn, m_mai->getTroll()));
					m_stage = GameStage::BuildRoom;
				}
				break;
			case 1:
				// Crew Button
				m_stateHUD.SetColorOnButton(index, cC, cHL, cHL);
				if (m_hudButtonsPressed[index])
				{
					m_subStates.push(new CrewState(m_cam, p_pickingEvent));
					m_stage = GameStage::CrewWindow;
				}
				
				break;
			case 2:
				// Event Button
				m_stateHUD.SetColorOnButton(index, cHL, cHL, cC);
				if (m_hudButtonsPressed[index])
				{
					m_subStates.push(new EventsState(m_cam, p_pickingEvent));
					m_stage = GameStage::EventWindow;
				}
				break;
			case 3:
				// Stats Button
				m_stateHUD.SetColorOnButton(index, cC, cHL / 4, cC);
				if (m_hudButtonsPressed[index])
				{
					m_subStates.push(new StatsState(m_cam, p_pickingEvent));
					m_stage = GameStage::StatsWindow;
				}
				break;
			}

		}
		else
		{
			if (index != lastHover)
			{
				lastHover = index;
				if (m_hudSnap->GetState() == DirectX::SoundState::PLAYING)
					m_hudSnap->Stop();
				m_hudSnap->Play();
			}

			switch (index)
			{
			case 0:
				// Crew Button
				if (!m_hudButtonsPressed[index])
					m_stateHUD.SetColorOnButton(index, cH, cHL, cHL);
				break;
			case 1:
				// Build Button
				if (!m_hudButtonsPressed[index])
					m_stateHUD.SetColorOnButton(index, cHL, cH, cHL);
				break;
			case 2:
				// Event Button
				if (!m_hudButtonsPressed[index])
					m_stateHUD.SetColorOnButton(index, cHL, cHL, cH);
				break;
			case 3:
				// Stats Button
				if (!m_hudButtonsPressed[index])
					m_stateHUD.SetColorOnButton(index, cH, cHL / 4, cH);
				break;
			}

		}
		if (m_clickedLastFrame && !Input::isMouseLeftPressed())
		{
			m_clickedLastFrame = false;
		}

	}
	else if (!m_madeFullReset)
	{
		// Miss all buttons
		for (int i = 0; i < m_hudButtonsPressed.size(); i++)
		{
			if (!m_hudButtonsPressed[i])
				m_stateHUD.ResetColorsOnPickableWithIndex(i);
		}
		m_madeFullReset = true;
	}

	return pickedHUD;
}

void GameState::_handleInput()
{	
	if (!m_subStates.empty())
	{
		SubState* ss = m_subStates.top();

		ss->HandleInput();
	}
}
