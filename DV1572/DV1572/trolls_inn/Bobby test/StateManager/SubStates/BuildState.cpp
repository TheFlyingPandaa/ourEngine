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
	if (Input::isMouseLeftPressed() && (m_currentBuildType != CurrentBuildType::NONE))
	{
		if (m_buildStage == BuildStage::None)
		{
			m_buildStage = BuildStage::Start;
			this->grid->PickTiles();
		}
		else if (m_buildStage == BuildStage::Selection)
		{
			//This runs when we are in the selection Face.
			//aka when you are dragin and droping.
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

				//For each state it will have a special condition
				//Just follow what is done below for new states
				//Don't forget to declare the new state in the update
				if (m_currentBuildType == CurrentBuildType::Room)
				{
					int area = (abs(end.x -start.x) + 1) * (abs(end.y - start.y) + 1);
					std::string textString = "Area: " + std::to_string(area);
					m_priceOfRoom.setTextString(textString);
					DirectX::XMFLOAT2 mp = Input::getMousePositionLH();
					m_priceOfRoom.setPosition(mp.x, mp.y + 20);
					m_roomPlaceable = this->grid->CheckAndMarkTiles(start, end);
				}
				else if(m_currentBuildType == CurrentBuildType::Door)
				{
					this->grid->CheckIfDoorCanBeBuilt(start, end);
				}
				else if (m_currentBuildType == CurrentBuildType::Furniture)
				{
					//THIS WILL BE CHANGED WHEN WE HAVE A OBJECTLOADER
					m_canBuildFurniture = this->grid->CheckAndMarkTilesObject(start, 1, 0);
				}

			}
		}
	}
	else if (m_buildStage == BuildStage::Selection && !Input::isMouseLeftPressed())
	{
		m_buildStage = BuildStage::End;
	}
	else if (m_buildStage == BuildStage::End && m_currentBuildType == CurrentBuildType::Door) {
		//All the code that was here is now in this neet function wop wop
		_doorBuildInput();

	}
	else if (m_buildStage == BuildStage::End && m_currentBuildType == CurrentBuildType::Room)
	{
		//All the code that was here is now in this neet function wop wop
		_roomBuildInput();
	}
	else if (m_buildStage == BuildStage::End && m_currentBuildType == CurrentBuildType::Furniture)
	{
		_objectBuildInput();
	}
	else {
		m_buildStage = BuildStage::None;
	}

}

void BuildState::_doorBuildInput()
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

	XMINT2 size = end;
	size.x -= start.x - 1;
	size.y -= start.y - 1;


	m_buildStage = BuildStage::None;
	m_startTile = nullptr;
	m_selectedTile = nullptr;
	m_roomPlaceable = false;

	//If debugging is needed you got the size
	this->grid->AddDoor(start, end, size);
		
}

void BuildState::_roomBuildInput()
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
	m_roomPlaceable = false;
	this->grid->AddRoom(start, end, m_selectedRoomType);
}

void BuildState::_objectBuildInput()
{
	XMFLOAT3 s = m_startTile->getPosition();
	XMINT2 start;
	start.x = static_cast<int>(s.x + 0.5f);
	start.y = static_cast<int>(s.z + 0.5f);
	Table fut = Table(DirectX::XMFLOAT3(start.x,0,start.y), &table);
	this->grid->ResetTileColor(start, start);
	if (m_canBuildFurniture)
	{
	
		this->grid->AddRoomObject(fut);
	}
	m_buildStage = BuildStage::None;
	m_startTile = nullptr;
	m_selectedTile = nullptr;
	m_roomPlaceable = false;
	
}

bool BuildState::_handleHUDPicking()
{
	bool pickedHUD = false;
	int index = p_HUD.PickHud(Input::getMousePositionLH());

	if (index != -2) pickedHUD = true;

	if (index >= 0)
	{
		float cH = 5.0f;
		float cHL = 2.0f;
		float cC = 50.0f;
		m_madeFullReset = false;
		if (!m_clickedLastFrame && Input::isMouseLeftPressed())
		{
			// Clicked a button
			_resetHudButtonPressedExcept(index);
			m_hudButtonsPressed[index] = !m_hudButtonsPressed[index];
			m_clickedLastFrame = true;

			switch (index)
			{
			case 0:
				// Rooms Button
				std::cout << "Build Rooms Button Pressed\n";
				p_HUD.SetColorOnButton(index, cC, cHL, cHL);
				break;
			case 1:
				// Furniture Button
				std::cout << "Build Furniutre Button Pressed\n";
				p_HUD.SetColorOnButton(index, cHL, cC, cHL);
				break;
			case 2:
				// Door Button
				std::cout << "Build Door Button Pressed\n";
				p_HUD.SetColorOnButton(index, cHL, cHL, cC);
				break;
			}
			if (m_hudButtonsPressed[0])
			{
				m_currentBuildType = CurrentBuildType::Room;
			}
			else if (m_hudButtonsPressed[1])
			{
				m_currentBuildType = CurrentBuildType::Furniture;
			}
			else if (m_hudButtonsPressed[2])
			{
				m_currentBuildType = CurrentBuildType::Door;
			}
			else
			{
				m_currentBuildType = CurrentBuildType::NONE;
			}

		}
		else
		{
			switch (index)
			{
			case 0:
				// Rooms Button
				if (!m_hudButtonsPressed[index])
					p_HUD.SetColorOnButton(index, cH, cHL, cHL);
				break;
			case 1:
				// Furniture Button
				if (!m_hudButtonsPressed[index])
					p_HUD.SetColorOnButton(index, cHL, cH, cHL);
				break;
			case 2:
				// Door Button
				if (!m_hudButtonsPressed[index])
					p_HUD.SetColorOnButton(index, cHL, cHL, cH);
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
		for (size_t i = 0; i < m_hudButtonsPressed.size(); i++)
		{
			if (!m_hudButtonsPressed[i])
				p_HUD.ResetColorsOnPickableWithIndex(i);
		}
		m_madeFullReset = true;
	}

	return pickedHUD;
}


BuildState::BuildState(Camera * cam,
	std::stack<Shape *>* pickingEvent,
	Grid * grid) : SubState(cam, pickingEvent)
{
	this->grid = grid;
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
	m_currentBuildType = CurrentBuildType::NONE;

	//TEMP
	table.LoadModel("trolls_inn/Resources/Stol.obj");
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
	//m_doorBuild = m_hudButtonsPressed[2];
	
	//m_currentBuildType = m_hudButtonsPressed[2];
	
	//_buildInput();

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
	_handleBuildRoom(pickedObject);
}

void BuildState::HandleInput()
{
	if(!_handleHUDPicking())
		_buildInput();
}
