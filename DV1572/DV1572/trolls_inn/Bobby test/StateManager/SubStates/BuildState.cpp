#include "BuildState.h"

void BuildState::_resetHudButtonPressedExcept(int index)
{
	for (size_t i = 0; i < m_hudButtonsPressed.size(); i++)
	{
		if (i != index)
		{
			m_hudButtonsPressed[i] = false;
			p_HUD.ResetColorsOnPickableWithIndex(i);
		}
	}
}

void BuildState::_handleBuildRoom(Shape * pickedShape)
{
	if (dynamic_cast<RectangleShape*>(pickedShape) == nullptr)
	{
		switch (m_buildStage)
		{
		case BuildStage::Start:
			if (pickedShape)
			{
				m_startTile = pickedShape;
				m_buildStage = BuildStage::Selection;
			}
			else
				m_buildStage = BuildStage::None;
			break;
		case BuildStage::Selection:
			if (pickedShape)
				m_selectedTile = pickedShape;
			break;
		}
	}
}

void BuildState::_buildInput()
{
	if (Input::isMouseLeftPressed() && (m_readyToPick || m_doorBuild))
	{
		if (m_buildStage == BuildStage::None)
		{
			m_buildStage = BuildStage::Start;
			this->grid->PickTiles();
		}
		else if (m_buildStage == BuildStage::Selection)
		{
			this->grid->PickTiles(m_selectedTile);

			if (m_startTile && m_selectedTile)
			{
				XMFLOAT3 s = m_startTile->getPosition();
				XMFLOAT3 e = m_selectedTile->getPosition();

				XMINT2 start;
				start.x = static_cast<int>(s.x + 0.5f);
				start.y = static_cast<int>(s.z + 0.5f);
				XMINT2 end;
				end.x = static_cast<int>(e.x + 0.5f);
				end.y = static_cast<int>(e.z + 0.5f);

				/*if (start.x > end.x)
					std::swap(start.x, end.x);
				if (start.y > end.y)
					std::swap(start.y, end.y);*/

				//for text
				if (!m_doorBuild)
				{
					/*int area = (abs(end.x -start.x) + 1) * (abs(end.y - start.y) + 1);
					std::string textString = "Area: " + std::to_string(area);
					m_priceOfRoom.setTextString(textString);
					DirectX::XMFLOAT2 mp = Input::getMousePositionLH();
					m_priceOfRoom.setPosition(mp.x, mp.y + 20);*/
					m_roomPlaceable = this->grid->CheckAndMarkTiles(start, end);
				}
				else
				{
				//	this->grid->CheckIfDoorCanBeBuilt(start, end);
				}

			}
		}
	}
	else if (m_buildStage == BuildStage::Selection && !Input::isMouseLeftPressed())
	{
		m_buildStage = BuildStage::End;
	}
	else if (m_buildStage == BuildStage::End && m_doorBuild) {
		XMFLOAT3 s = m_startTile->getPosition();
		XMFLOAT3 e = m_selectedTile->getPosition();

		XMINT2 start;
		start.x = static_cast<int>(s.x + 0.5f);
		start.y = static_cast<int>(s.z + 0.5f);
		XMINT2 end;
		end.x = static_cast<int>(e.x + 0.5f);
		end.y = static_cast<int>(e.z + 0.5f);

		if (start.x > end.x)
			std::swap(start.x, end.x);
		if (start.y > end.y)
			std::swap(start.y, end.y);
		this->grid->ResetTileColor(start, end);

		XMINT2 size = end;
		size.x -= start.x - 1;
		size.y -= start.y - 1;


		m_buildStage = BuildStage::None;
		m_startTile = nullptr;
		m_selectedTile = nullptr;
		m_roomPlaceable = false;

		//If debugging is needed you got the size
		//this->grid->AddDoor(start, end, size);


	}
	else if (m_buildStage == BuildStage::End)
	{
		XMFLOAT3 s = m_startTile->getPosition();
		XMFLOAT3 e = m_selectedTile->getPosition();

		XMINT2 start;
		start.x = static_cast<int>(s.x + 0.5f);
		start.y = static_cast<int>(s.z + 0.5f);
		XMINT2 end;
		end.x = static_cast<int>(e.x + 0.5f);
		end.y = static_cast<int>(e.z + 0.5f);

		if (start.x > end.x)
			std::swap(start.x, end.x);
		if (start.y > end.y)
			std::swap(start.y, end.y);
		this->grid->ResetTileColor(start, end);

		

		//This makes it a size.
		//this will change the end point
		end.x -= start.x - 1;
		end.y -= start.y - 1;

		m_buildStage = BuildStage::None;
		m_startTile = nullptr;
		m_selectedTile = nullptr;
		 if(m_roomPlaceable)
			 m_roomCtrl->AddRoom(start, end, m_selectedRoomType, grid->extractTiles(start,end));
		 m_roomPlaceable = false;
	}
	else {
		m_buildStage = BuildStage::None;
	}

	if (!Input::isMouseLeftPressed() && m_hudButtonsPressed[0])
		m_readyToPick = true;
	else if (!m_hudButtonsPressed[0])
		m_readyToPick = false;

}

void BuildState::_doorBuildInput()
{
	this->grid->PickTiles(m_startTile);
	if (m_startTile)
	{
		XMFLOAT3 s = m_startTile->getPosition();
		XMFLOAT3 e = s;

		XMINT2 start;
		start.x = static_cast<int>(s.x + 0.5f);
		start.y = static_cast<int>(s.z + 0.5f);
		XMINT2 end;
		end.x = static_cast<int>(e.x + 0.5f);
		end.y = static_cast<int>(e.z + 0.5f);

		if (start.x > end.x)
			std::swap(start.x, end.x);
		if (start.y > end.y)
			std::swap(start.y, end.y);
		
		//if (Input::isMouseLeftPressed() && this->grid->CheckIfDoorCanBeBuilt(start, end))
		//{
		//	//this->grid->AddDoor(start, end, end);
		//}
	}
		




}

void BuildState::_handlePickingOfHud(RectangleShape * r)
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
						p_HUD.ResetColorsOnPickableWithIndex(i);
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
			default:
				r->setColor(3, 3, 3);
				break;
			}
			break;
		}
		case HudPickingStage::Click:
		{
			int index = r->getIndex();
			switch (index)
			{
			case 0:
				std::cout << "Build Rooms Button Pressed\n";
				r->setColor(cC, cHL, cHL);
				_resetHudButtonPressedExcept(index);
				m_hudButtonsPressed[index] = !m_hudButtonsPressed[index];
				break;
			case 1:
				std::cout << "Build Furniutre Button Pressed\n";
				r->setColor(cHL, cC, cHL);
				_resetHudButtonPressedExcept(index);
				break;
			case 2:
				std::cout << "Build Door Button Pressed\n";
				r->setColor(cHL, cHL, cC);
				_resetHudButtonPressedExcept(index);
				m_hudButtonsPressed[index] = !m_hudButtonsPressed[index];
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

BuildState::BuildState(Camera * cam,
	std::stack<Shape *>* pickingEvent,
	Grid * grid, RoomCtrl* roomCtrl) : SubState(cam, pickingEvent)
{
	this->grid = grid;
	m_roomCtrl = roomCtrl;
	this->_init();
	m_buildStage = BuildStage::None;
	m_readyToPick = false;
	m_priceOfRoom.setColor(1, 1, 1, 1);
	m_priceOfRoom.setRelative(Text::RelativeTo::BL);
	m_priceOfRoom.setPosition(0, 0);
	m_priceOfRoom.setScale(0.3f);
	m_priceOfRoom.setRotation(0.0f);
	m_priceOfRoom.setTextString("");
	m_priceOfRoom.setAllignment(TXT::Center);
}

BuildState::~BuildState()
{
}

void BuildState::_init()
{
	p_HUD.LoadHud("trolls_inn/Resources/HUD/BuildHud/BuildHud.txt");
	for (int i = 0; i < p_HUD.getNrOfPickableButtons(); i++)
		m_hudButtonsPressed.push_back(false);
}

void BuildState::Update(double deltaTime)
{
	m_doorBuild = m_hudButtonsPressed[2];
	
	_buildInput();

}

void BuildState::Draw()
{
	if (!m_hudButtonsPressed[2] && m_buildStage == BuildStage::Selection)
		m_priceOfRoom.Draw();
}

void BuildState::DrawHUD()
{
	p_HUD.Draw();
}

void BuildState::HandlePicking(Shape * pickedObject)
{
	if (m_hudPickStage != HudPickingStage::Miss)
		_handlePickingOfHud(dynamic_cast<RectangleShape*>(pickedObject));

	_handleBuildRoom(pickedObject);
}

void BuildState::HandleInput()
{
	if (p_HUD.isMouseInsidePotentialAreaRect(Input::getMousePositionLH()))
	{
		p_HUD.CheckIfPicked();
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
					p_HUD.ResetColorsOnPickableWithIndex(i);
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
					p_HUD.ResetColorsOnPickableWithIndex(i);
			}
		}
		_buildInput();
	}


}
