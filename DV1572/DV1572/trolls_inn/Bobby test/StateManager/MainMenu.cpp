#include "MainMenu.h"
#include <iostream>

MainMenu::MainMenu(std::stack<Shape*>* pickingEvent, std::stack<int>* keyEvent, Camera * cam) : State(pickingEvent, keyEvent)
{
	this->m_cam = cam;
	this->_init();

	
}

MainMenu::~MainMenu()
{

}

void MainMenu::Update(double deltaTime)
{  
	
	_handlePicking();
	_handleInput();

}

void MainMenu::_handlePicking()
{
	while (!p_pickingEvent->empty())
	{
		Shape * obj = this->p_pickingEvent->top();
		this->p_pickingEvent->pop();

		if (m_hudPickStage != HudPickingStage::Miss)
			_handleHUDPicking(dynamic_cast<RectangleShape*>(obj));

	}
}

void MainMenu::_handleHUDPicking(RectangleShape* r)
{
	if (r)
	{
		switch (m_hudPickStage)
		{
		case HudPickingStage::Hover:
			m_colorButton = true;
			if (r->getIndex() != m_lastPickedIndex)
			{
				m_lastPickedIndex = r->getIndex();
				m_stateHUD.ResetColorsOnPickable();
			}
			switch (r->getIndex())
			{
			case 0:
				r->setColor(0.2f, 0.2f, 2.0f);
				m_exitState = true;
				break;
			case 1:
				r->setColor(2.0f, 0.2f, 0.2f);
				break;
			case 2:
				r->setColor(1.5f, 0.2f, 1.5f);
				break;
			case 3:
				r->setColor(0.2f, 1.5f, 1.5f);
				break;
			}
			break;
		case HudPickingStage::Click:
			m_colorButton = true;
			//do something
			std::cout << "Something Picked";
			break;
		}
	}
	else
	{
		m_hudPickStage = HudPickingStage::Miss;
	}
}

void MainMenu::_handleInput()
{
	if (m_stateHUD.isMouseInsidePotentialArea(Input::getMousePositionLH()))
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
			m_stateHUD.ResetColorsOnPickable();
	}
	else
	{
		if (m_hudPickStage != HudPickingStage::Miss)
		{
			m_hudPickStage = HudPickingStage::Miss;
			m_stateHUD.ResetColorsOnPickable();
		}

	}

}

void MainMenu::Draw()
{
	m_stateHUD.Draw();
}

void MainMenu::_init()
{
	m_stateHUD.LoadHud("trolls_inn/Resources/Menus/TopMenu.txt");

}
