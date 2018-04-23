#include "BuildState.h"

void BuildState::_resetHudButtonPressedExcept(int index, std::vector<bool> &vec, HUD &selectedHud)
{
	for (size_t i = 0; i < vec.size(); i++)
	{
		if (i != index)
		{
			vec[i] = false;
			selectedHud.ResetColorsOnPickableWithIndex(i);
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
	if (Input::isMouseLeftPressed() && (m_currentBuildType != CurrentBuildType::NONE && (m_selectedRoomType != RoomType::UNDEFINED || m_selectedDoor != -1)))
	{
		//this->grid->PickTiles();
		if (m_buildStage == BuildStage::None)
		{
			m_buildStage = BuildStage::Start;
			

			if (m_currentBuildType == CurrentBuildType::Room)
				grid->PickTiles();
			if (m_currentBuildType == CurrentBuildType::Door)
				m_roomCtrl->PickWalls();
			if (m_currentBuildType == CurrentBuildType::Furniture)
				m_roomCtrl->PickRoomTiles();

			
		}
		else if (m_buildStage == BuildStage::Selection)
		{
			//This runs when we are in the selection Face.
			//aka when you are dragin and droping.
			grid->PickTiles(m_selectedTile);
			m_roomCtrl->PickRoomTiles();

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
				if (m_currentBuildType == CurrentBuildType::Room && m_selectedRoomType != RoomType::UNDEFINED)
				{
					/*int area = (abs(end.x -start.x) + 1) * (abs(end.y - start.y) + 1);
					std::string textString = "Area: " + std::to_string(area);
					m_priceOfRoom.setTextString(textString);
					DirectX::XMFLOAT2 mp = Input::getMousePositionLH();
					m_priceOfRoom.setPosition(mp.x, mp.y + 20);*/
					m_roomPlaceable = this->grid->CheckAndMarkTiles(start, end);
				}
				else if(m_currentBuildType == CurrentBuildType::Door && m_selectedDoor != -1)
				{
					// This is the door building
					/*std::cout << "Start: (" << start.x << "," << start.y << ") ";
					std::cout << "End: (" << end.x << "," << end.y << ")\n";*/
					//this->grid->CheckIfDoorCanBeBuilt(start, end);
				}
				else if (m_currentBuildType == CurrentBuildType::Furniture)
				{
					//THIS WILL BE CHANGED WHEN WE HAVE A OBJECTLOADER
					m_canBuildFurniture = this->m_roomCtrl->CheckAndMarkTilesObject(start, 2, 0);
				}

			}
		}
	}
	else if (m_buildStage == BuildStage::Selection && !Input::isMouseLeftPressed())
	{
		m_buildStage = BuildStage::End;
	}
	else if (m_buildStage == BuildStage::End && m_currentBuildType == CurrentBuildType::Door && m_selectedDoor != -1) {
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

	m_buildStage = BuildStage::None;
	m_startTile = nullptr;
	m_selectedTile = nullptr;
	m_roomPlaceable = false;
	m_roomCtrl->CreateDoor(s);

		
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
	if(m_roomPlaceable)
		m_roomCtrl->AddRoom(start, end, RoomType::reception, grid->extractTiles(start, end));
	m_roomPlaceable = false;
	
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
		m_roomCtrl->AddRoomObject(fut);
	
	}
	m_buildStage = BuildStage::None;
	m_startTile = nullptr;
	m_selectedTile = nullptr;
	m_roomPlaceable = false;
	
}

bool BuildState::_handleHUDPicking()
{
	bool hudPicked = false;
	hudPicked = _mainHudPick();

	if (!hudPicked && m_currentBuildType == CurrentBuildType::Room)
		hudPicked = _roomBuildHudPick();
	else if (!hudPicked && m_currentBuildType == CurrentBuildType::Door)
		hudPicked = _doorBuildHudPick();


	return hudPicked;
}

bool BuildState::_mainHudPick()
{
	bool pickedHUD = false;
	int index = p_HUD.PickHud(Input::getMousePositionLH());

	if (index != -2) pickedHUD = true;

	if (index >= 0)
	{
		float cH = 5.0f;
		float cHL = 2.0f;
		float cC = 50.0f;
		m_madeFullResetMain = false;
		if (!m_clickedLastFrame && Input::isMouseLeftPressed())
		{
			_resetHudButtonPressedExcept(index, m_hudButtonsPressed, p_HUD);
			// Clicked a button
			m_hudButtonsPressed[index] = !m_hudButtonsPressed[index];
			m_clickedLastFrame = true;

			switch (index)
			{
			case 0:
				// Rooms Button
				std::cout << "Build Rooms Button Pressed\n";
				p_HUD.SetColorOnButton(index, cC, cHL, cHL);
				if (!m_hudButtonsPressed[index])
				{
					m_selectedRoomType = RoomType::UNDEFINED;
					m_currentBuildType = CurrentBuildType::NONE;
					for (size_t i = 0; i < m_roomHUDButtonsPressed.size(); i++)
						m_roomHUDButtonsPressed[i] = false;
				}
				else
				{
					m_selectedDoor = -1;
					m_currentBuildType = CurrentBuildType::Room;
				}
				break;
			case 1:
				// Furniture Button
				std::cout << "Build Furniutre Button Pressed\n";
				p_HUD.SetColorOnButton(index, cHL, cC, cHL);
				m_currentBuildType = CurrentBuildType::Furniture;
				break;
			case 2:
				// Door Button
				std::cout << "Build Door Button Pressed\n";
				p_HUD.SetColorOnButton(index, cHL, cHL, cC);
				if (!m_hudButtonsPressed[index])
				{
					m_selectedDoor = -1;
					m_currentBuildType = CurrentBuildType::NONE;
				}
				else
				{
					m_selectedRoomType = RoomType::UNDEFINED;
					m_currentBuildType = CurrentBuildType::Door;
					for (size_t i = 0; i < m_roomHUDButtonsPressed.size(); i++)
						m_roomHUDButtonsPressed[i] = false;
				}
				break;
			}
		}
		else
		{
			int picked = -1;
			for (size_t i = 0; i < m_hudButtonsPressed.size() && picked == -1; i++)
			{
				if (m_hudButtonsPressed[i])
					picked = i;
			}
			
			p_HUD.ResetColorsExcept(picked);

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
	else if (!m_madeFullResetMain)
	{
		// Miss all buttons
		for (size_t i = 0; i < m_hudButtonsPressed.size(); i++)
		{
			if (!m_hudButtonsPressed[i])
				p_HUD.ResetColorsOnPickableWithIndex(i);
		}
		m_madeFullResetMain = true;
	}

	return pickedHUD;
}

bool BuildState::_roomBuildHudPick()
{
	bool pickedHUD = false;
	int index = m_roomHUD.PickHud(Input::getMousePositionLH());

	if (index != -2) pickedHUD = true;

	

	if (index >= 0)
	{
		float cH = 5.0f;
		float cHL = 2.0f;
		float cC = 50.0f;
		m_madeFullResetRoomHud = false;
		if (!m_clickedLastFrame && Input::isMouseLeftPressed())
		{
			// Clicked a button
			_resetHudButtonPressedExcept(index, m_roomHUDButtonsPressed, m_roomHUD);
			m_roomHUDButtonsPressed[index] = !m_roomHUDButtonsPressed[index];
			if (!m_roomHUDButtonsPressed[index])
				m_selectedRoomType = RoomType::UNDEFINED;
			m_clickedLastFrame = true;

			switch (index)
			{
			case 0:
				// Kitchen Button
				std::cout << "Kitchen Button Pressed\n";
				m_roomHUD.SetColorOnButton(index, cC, cHL, cHL);
				if (m_roomHUDButtonsPressed[index])
					m_selectedRoomType = RoomType::kitchen;
				break;
			case 1:
				// hallway Button
				std::cout << "Hallway Button Pressed\n";
				m_roomHUD.SetColorOnButton(index, cHL, cC, cHL);
				if (m_roomHUDButtonsPressed[index])
					m_selectedRoomType = RoomType::hallway;
				break;
			case 2:
				// Bedroom Button
				std::cout << "Bedroom Button Pressed\n";
				m_roomHUD.SetColorOnButton(index, cHL, cHL, cC);
				if (m_roomHUDButtonsPressed[index])
					m_selectedRoomType = RoomType::bedroom;
				break;
			}
		}
		else
		{
			int picked = -1;
			for (size_t i = 0; i < m_roomHUDButtonsPressed.size() && picked == -1; i++)
			{
				if (m_roomHUDButtonsPressed[i])
					picked = i;
			}

			m_roomHUD.ResetColorsExcept(picked);

			switch (index)
			{
			case 0:
				// kitchen Button
				if (!m_roomHUDButtonsPressed[index])
					m_roomHUD.SetColorOnButton(index, cH, cHL, cHL);
				break;
			case 1:
				// hallway Button
				if (!m_roomHUDButtonsPressed[index])
					m_roomHUD.SetColorOnButton(index, cHL, cH, cHL);
				break;
			case 2:
				// Bedroom Button
				if (!m_roomHUDButtonsPressed[index])
					m_roomHUD.SetColorOnButton(index, cHL, cHL, cH);
				break;
			}

		}
		if (m_clickedLastFrame && !Input::isMouseLeftPressed())
		{
			m_clickedLastFrame = false;
		}

	}
	else if (!m_madeFullResetRoomHud)
	{
		// Miss all buttons
		for (size_t i = 0; i < m_roomHUDButtonsPressed.size(); i++)
		{
			if (!m_roomHUDButtonsPressed[i])
				m_roomHUD.ResetColorsOnPickableWithIndex(i);
		}
		m_madeFullResetRoomHud = true;
	}

	return pickedHUD;
}

bool BuildState::_doorBuildHudPick()
{
	bool pickedHUD = false;
	int index = m_doorHUD.PickHud(Input::getMousePositionLH());

	if (index != -2) pickedHUD = true;

	if (index >= 0)
	{
		float cH = 5.0f;
		float cHL = 2.0f;
		float cC = 50.0f;
		m_madeFullResetRoomHud = false;
		if (!m_clickedLastFrame && Input::isMouseLeftPressed())
		{
			// Clicked a button
			m_clickedLastFrame = true;
			if (m_selectedDoor != index)
			{
				m_selectedDoor = index;
				m_doorHUD.SetColorOnButton(m_selectedDoor, 0.0, 178.0/255.0, 255.0/255.0);
			}
			else
			{
				m_selectedDoor = -1;
				m_doorHUD.ResetColorsExcept(m_selectedDoor);
			}

			
		}
		else
		{
			m_doorHUD.ResetColorsExcept(m_selectedDoor);
			if (m_selectedDoor != index)
				m_doorHUD.SetColorOnButton(index, 232.0 / 255.0, 81.0 / 255.0, 0.0);
		}
		if (m_clickedLastFrame && !Input::isMouseLeftPressed())
		{
			m_clickedLastFrame = false;
		}

	}
	else
		m_doorHUD.ResetColorsExcept(m_selectedDoor);

	return pickedHUD;
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
	m_currentBuildType = CurrentBuildType::NONE;

	//TEMP
	table.LoadModel("trolls_inn/Resources/Stol.obj");
}

BuildState::~BuildState()
{
}

void BuildState::_init()
{
	m_selectedRoomType = RoomType::UNDEFINED;
	m_madeFullResetMain = true;
	m_madeFullResetRoomHud = true;
	m_selectedDoor = -1;
	
	p_HUD.LoadHud("trolls_inn/Resources/HUD/BuildHud/BuildHud.txt");
	for (int i = 0; i < p_HUD.getNrOfPickableButtons(); i++)
		m_hudButtonsPressed.push_back(false);

	m_roomHUD.LoadHud("trolls_inn/Resources/HUD/BuildHud/RoomBuild/RoomBuildHud.txt");
	for (int i = 0; i < m_roomHUD.getNrOfPickableButtons(); i++)
		m_roomHUDButtonsPressed.push_back(false);

	m_doorHUD.LoadHud("trolls_inn/Resources/HUD/BuildHud/DoorBuild/DoorBuildHud.txt");
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

	switch (m_currentBuildType)
	{
	case CurrentBuildType::Room:
		m_roomHUD.Draw();
		break;
	case CurrentBuildType::Door:
		m_doorHUD.Draw();
		break;
	}

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
