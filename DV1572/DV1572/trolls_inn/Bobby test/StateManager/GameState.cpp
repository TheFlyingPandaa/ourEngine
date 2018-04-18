#include "GameState.h"
#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <thread>
#include <future>
#include "../../../ourEngine/interface/light/PointLight.h"
#include "../../../ourEngine/core/Dx.h"
#include "../StateManager/SubStates/BuildState.h"

GameState::GameState(std::stack<Shape*>* pickingEvent, std::stack<int>* keyEvent, Camera * cam) : State(pickingEvent, keyEvent)
{
	m_stage = GameStage::Play;
	m_hasClicked = false;
	m_colorButton = false;
	m_hudPickStage = HudPickingStage::Miss;
	// Building END
	m_lastPickedIndex = -1;
	m_Rpressed = false;
	_setHud();

	int nrOfButtons = m_stateHUD.getNrOfPickableButtons();
	{
	for (int i = 0; i < nrOfButtons; i++)
		m_hudButtonsPressed.push_back(false);
	}
	this->m_cam = cam;
	this->_init();

	//grid->getRoomCtrl().CreateDoors();
	previousKey = -1;	

}

GameState::~GameState()
{
	while (!m_subStates.empty())
	{
		delete m_subStates.top();
		m_subStates.pop();
	}
}

// round float to n decimals precision
float round_n(float num, int dec)
{
	float m = (num < 0.0f) ? -1.0f : 1.0f;   // check if input is negative
	float pwr = pow(10.0f, dec);
	return float((float)floor((double)num * m * pwr + 0.5) / pwr) * m;
}
void GameState::Update(double deltaTime)
{
	_handlePicking();	// It's important this is before handleInput();
	_handleInput();		// It's important this is after handlePicking();
	
	static bool lol = false;
	bool lol2 = Input::isKeyPressed('L');
	if (lol2 && !lol)
	{
		this->m_mai.spawn();	
	}
	lol = lol2;


	this->m_cam->update();
	


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
		return;
		
	}
	m_mai.Update(this->m_cam);
	gameTime.updateCurrentTime(static_cast<float>(deltaTime));
	//auto currentTime = std::chrono::high_resolution_clock::now();
	if (Input::isKeyPressed('N')) {
		//m_newState = new MainMenu(p_pickingEvent, p_keyEvents, m_cam);
	}
	auto currentTime = std::chrono::high_resolution_clock::now();
	
	
	m_colorButton = false;
	auto time = std::chrono::high_resolution_clock::now();
	auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(time - currentTime).count();
}

void GameState::Draw()
{
	gameTime.m_cpyLightToGPU();
	m_mai.Draw();
	if (!m_subStates.empty())
		m_subStates.top()->Draw();
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
	for (size_t i = 0; i < m_hudButtonsPressed.size(); i++)
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
	
}

void GameState::_setHud()
{
	m_stateHUD.LoadHud("trolls_inn/Resources/HUD/MainHud/MainHud.txt");
}

void GameState::_handlePicking()
{
	while (!p_pickingEvent->empty())
	{
		Shape * obj = this->p_pickingEvent->top();
		this->p_pickingEvent->pop();

		if (m_hudPickStage != HudPickingStage::Miss)
			_handleHUDPicking(dynamic_cast<RectangleShape*>(obj));
		else if (!m_subStates.empty())
		{
			SubState* ss = m_subStates.top();

			ss->HandlePicking(obj);
		}



		using namespace std::chrono_literals;

		// Create a promise and get its future.
		if (m_i == 0)
		{
			m_i++;
			future = std::async(std::launch::async, &GameState::_handlePickingAi, this, obj);
		}


		// Use wait_for() with zero milliseconds to check thread status.
		auto status = future.wait_for(0ms);

		// Print status. And start a new thread if the other thread was finnished
		if (status == std::future_status::ready) {
			//std::cout << "Thread finished" << std::endl;
			future.get();
			future = std::async(std::launch::async, &GameState::_handlePickingAi, this, obj);
			
		}
		else {
			std::cout << "Thread still running" << std::endl;
		}

		//_handlePickingAi(obj);
		
	}
}

void GameState::_handlePickingAi(Shape * obj)
{

	if (m_stage == GameStage::Play)
	{
		
	}
}


void GameState::_handleHUDPicking(RectangleShape* r)
{
	if (r)
	{
		float cH = 5.0f;
		float cHL = 2.0f;
		float cC = 50.0f;

			m_colorButton = true;
		switch (m_hudPickStage)
		{
		case HudPickingStage::Hover:
		{
			int index = r->getIndex();
			if (index != m_lastPickedIndex)
			{
				m_lastPickedIndex = index;
				for (size_t i = 0; i < m_hudButtonsPressed.size(); i++)
				{
					if (!m_hudButtonsPressed[i])
						m_stateHUD.ResetColorsOnPickableWithIndex(i);
				}
			}
			switch (index)
			{
			case 0:
				if (!m_hudButtonsPressed[index])
					r->setColor(cH, cHL, cHL);
				break;
			case 1:
				if (!m_hudButtonsPressed[index])
					r->setColor(cHL, cH, cHL);
				break;
			case 2:
				if (!m_hudButtonsPressed[index])
					r->setColor(cHL, cHL, cH);
				break;
			case 3:
				if (!m_hudButtonsPressed[index])
					r->setColor(cH, cHL / 4, cH);
				break;
			default:
				r->setColor(3, 3, 3);
				break;
			}
			break;
		}
		case HudPickingStage::Click:
		{
			int index = r->getIndex();
			while (!m_subStates.empty())
			{
				SubState * s = m_subStates.top();
				m_subStates.pop();
				delete s;
			}
			switch (index)
			{
			case 0:
				std::cout << "Crew Button Pressed\n";
				r->setColor(cC, cHL, cHL);
				_resetHudButtonPressedExcept(index);
				break;
			case 1:
				std::cout << "BUID Button Pressed\n";
				r->setColor(cHL, cC, cHL);
				_resetHudButtonPressedExcept(index);
				m_hudButtonsPressed[index] = !m_hudButtonsPressed[index];
				if (m_hudButtonsPressed[index])
					m_subStates.push(new BuildState(m_cam, p_pickingEvent, m_mai.GetGrid()));
					
				break;
			case 2:
				std::cout << "Event Button Pressed\n";
				r->setColor(cHL, cHL, cC);
				_resetHudButtonPressedExcept(index);
				break;
			case 3:
				std::cout << "Stats Button Pressed\n";
				r->setColor(cC, cHL/4, cC);
				_resetHudButtonPressedExcept(index);
				break;
			default:
				std::cout << "Something Pressed\n";
				_resetHudButtonPressedExcept(index);
				break;
			}
			
			break;
		}
		}
	}
	else
	{
		m_hudPickStage = HudPickingStage::Miss;
	}
}

void GameState::_handleInput()
{
	if (m_stateHUD.isMouseInsidePotentialAreaRect(Input::getMousePositionLH()))
	{
		m_stateHUD.CheckIfPicked();
		m_hudPickStage = HudPickingStage::Hover;
		if (Input::isMouseLeftPressed() && !m_hasClicked)
		{
			m_hudPickStage = HudPickingStage::Click;
			m_hasClicked = true;
		}
		else if (!Input::isMouseLeftPressed() && m_hasClicked)
		{
			m_hasClicked = false;
		}
		if (!m_colorButton)
			for (size_t i = 0; i < m_hudButtonsPressed.size(); i++)
			{
				if (!m_hudButtonsPressed[i])
					m_stateHUD.ResetColorsOnPickableWithIndex(i);
			}
	}
	else
	{
		if (m_hudPickStage != HudPickingStage::Miss)
		{
			m_hudPickStage = HudPickingStage::Miss;
			for (size_t i = 0; i < m_hudButtonsPressed.size(); i++)
			{
				if (!m_hudButtonsPressed[i])
					m_stateHUD.ResetColorsOnPickableWithIndex(i);
			}
		}
		
		if (!m_subStates.empty())
		{
			SubState* ss = m_subStates.top();

			ss->HandleInput();
		}

		
		if (m_stage == GameStage::Play && m_subStates.empty())
		{
			if (Input::isMouseLeftPressed())
			{
				m_mai.GetGrid()->PickTiles();
				m_move = true;
			}
			else
				m_move = false;
		}
	}

	if (Input::isKeyPressed('P'))
		m_stage = GameStage::Play;

	

	if (Input::isKeyPressed('R') && !m_Rpressed)
		_setHud();
	else if (!Input::isKeyPressed('R'))
		m_Rpressed = false;
}

