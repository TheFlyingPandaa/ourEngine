#include "BuildState.h"
#include "../../../Mesh Manager/MeshManager.h"

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
		switch (m_currentBuildType)
		{
		case BuildState::RoomBuild:
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
			case BuildStage::None:
				if (pickedShape)
				{
					DirectX::XMINT2 pos(pickedShape->getPosition().x, pickedShape->getPosition().z);
					Room* temp = m_roomCtrl->getRoomAtPos(pos);
					if (temp != m_selectedRoom)
					{
						if (m_selectedRoom)
							m_selectedRoom->Select();
						m_selectedRoom = temp;
						m_selectedRoom->Select();
						
					}
					else
					{
						m_selectedRoom->Select();
						m_selectedRoom = nullptr;
					}
					
				}
				break;
			}
			break;
		case BuildState::Door:
			m_startTile = pickedShape;
			break;
		case BuildState::Furniture:
			m_startTile = pickedShape;
			break;
		default:
			m_startTile = nullptr;
			m_selectedTile = nullptr;
			break;
		}
	}
}

void BuildState::_buildInput()
{
	switch (m_currentBuildType)
	{
	case CurrentBuildType::RoomBuild:
		_inputBuildRoom();
		break;
	case CurrentBuildType::Door:
		_inputDoor();
		break;
	case CurrentBuildType::Furniture:
		_inputFurniture();
		break;
	default:
		m_startTile = nullptr;
		m_selectedTile = nullptr;
		m_selectedThing = -1;
		break;
	}

}

void BuildState::_objectBuildInput()
{
		
}

bool BuildState::_handleHUDPicking()
{
	bool hudPicked = false;
	hudPicked = _mainHudPick();
	if (!hudPicked)
	{
		if (m_currentBuildType == CurrentBuildType::RoomBuild)
			hudPicked = _selectionBuildHudPick(m_roomHUD);
		else if (m_currentBuildType == CurrentBuildType::Door)
			hudPicked = _selectionBuildHudPick(m_doorHUD);
		else if (m_currentBuildType == CurrentBuildType::Furniture)
			hudPicked = _selectionBuildHudPick(m_furnitureHUD);
	}


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
			if (m_roomCtrl->getIsBuildingDoor())
				m_roomCtrl->setIsBuildingDoor(false);
			m_selectedThing = -1;
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
					m_currentBuildType = CurrentBuildType::NONE;
					m_selectedThing = -1;
				}
				else
				{
					m_selectedThing = -1;
					m_currentBuildType = CurrentBuildType::RoomBuild;
				}
				break;
			case 1:
				// Furniture Button
				std::cout << "Build Furniutre Button Pressed\n";
				p_HUD.SetColorOnButton(index, cHL, cC, cHL);
				if (!m_hudButtonsPressed[index])
				{
					m_selectedThing = -1;
					m_currentBuildType = CurrentBuildType::NONE;
				}
				else
				{
					m_selectedThing = -1;
					m_currentBuildType = CurrentBuildType::Furniture;
				}
				break;
			case 2:
				// Door Button
				std::cout << "Build Door Button Pressed\n";
				p_HUD.SetColorOnButton(index, cHL, cHL, cC);
				if (!m_hudButtonsPressed[index])
				{
					m_selectedThing = -1;
					m_currentBuildType = CurrentBuildType::NONE;
				}
				else
				{
					m_selectedThing = -1;
					m_currentBuildType = CurrentBuildType::Door;
					m_roomCtrl->setIsBuildingDoor(true);
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


bool BuildState::_selectionBuildHudPick(HUD & h)
{
	bool pickedHUD = false;
	int index = h.PickHud(Input::getMousePositionLH());
	
	if (index != -2) pickedHUD = true;

	if (index >= 0)
	{
		float cH = 5.0f;
		float cHL = 2.0f;
		float cC = 50.0f;
		if (!m_clickedLastFrame && Input::isMouseLeftPressed())
		{
			// Clicked a button
			m_clickedLastFrame = true;
			if (m_selectedThing != index)
			{
				m_selectedThing = index;
				h.SetPressColorOnButton(m_selectedThing);
			}
			else
			{
				m_selectedThing = -1;
				h.ResetColorsExcept(m_selectedThing);
			}
		}
		else
		{
			h.ResetColorsExcept(m_selectedThing);
			if (m_selectedThing != index)
				h.SetHoverColorOnButton(index);
		}
		if (m_clickedLastFrame && !Input::isMouseLeftPressed())
		{
			m_clickedLastFrame = false;
		}
	}
	else
		h.ResetColorsExcept(m_selectedThing);

	return pickedHUD;
}

void BuildState::_inputBuildRoom()
{
	if (Input::isMouseLeftPressed() && m_selectedThing != -1)
	{
		//this->grid->PickTiles();
		if (m_buildStage == BuildStage::None)
		{
			m_buildStage = BuildStage::Start;
			grid->PickTiles();
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

				int area = (abs(end.x - start.x) + 1) * (abs(end.y - start.y) + 1);
				std::string textString = "Area: " + std::to_string(area);
				m_priceOfRoom.setTextString(textString);
				DirectX::XMFLOAT2 mp = Input::getMousePositionLH();
				m_priceOfRoom.setPosition(mp.x, mp.y + 30);
				m_roomPlaceable = this->grid->CheckAndMarkTiles(start, end);
			}
		}
	}
	else if (m_buildStage == BuildStage::Selection && !Input::isMouseLeftPressed())
	{
		m_buildStage = BuildStage::End;
	}
	else if (m_buildStage == BuildStage::End)
	{
		_buildRoom();
	}
	else if (!m_clickedLastFrame && m_selectedThing == -1 && Input::isMouseLeftPressed())
	{
		m_roomCtrl->PickRoomTiles();
		m_clickedLastFrame = true;
	}
	else if (m_clickedLastFrame && !Input::isMouseLeftPressed())
	{
		m_clickedLastFrame = false;
	}
}

void BuildState::_buildRoom()
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
	if (m_roomPlaceable && m_selectedThing != -1)
	{
		m_roomCtrl->AddRoom(start, end, static_cast<RoomType>(m_selectedThing), grid->extractTiles(start, end));
		m_startTile = nullptr;
		m_selectedTile = nullptr;
	}
	m_roomPlaceable = false;
}

void BuildState::_inputDoor()
{
	if (m_selectedThing != -1)
	{
		m_roomCtrl->PickWalls();
		if (m_startTile && Input::isMouseLeftPressed())
		{
			XMFLOAT3 s = m_startTile->getPosition();
			m_buildStage = BuildStage::None;
			m_startTile = nullptr;
			m_selectedTile = nullptr;
			m_roomPlaceable = false;
			m_roomCtrl->CreateDoor(s);
			drawSelectedThing = true;
		}
		else if (m_startTile)
		{
			DirectX::XMFLOAT3 p(m_startTile->getPosition());
			door.setPos(p.x, p.y, p.z);
			//std::cout << m_startTile->getRotation().y << std::endl;
			door.setRotation(0.0f, DirectX::XMConvertToDegrees(m_startTile->getRotation().y), 0.0f);
			door.setScale(1.01);
			//TEMP
			drawSelectedThing = true;
			twoStepThingy = true;
		}
		else
		{
			if (twoStepThingy)
				twoStepThingy = false;
			else drawSelectedThing = false;
			
		}
	}
	else
		drawSelectedThing = false;

}

void BuildState::_inputFurniture()
{
	if (m_selectedThing != -1)
	{
		m_roomCtrl->PickRoomTiles();
		if (m_startTile && Input::isMouseLeftPressed())
		{
			XMINT3 s= table->getPosition();
			XMINT2 start;
			start.x = s.x;
			start.y = s.z;
			if (m_canBuildFurniture)
			{
				m_roomCtrl->AddRoomObject(*table);
			}

			m_buildStage = BuildStage::None;
			m_startTile = nullptr;
			m_selectedTile = nullptr;
			m_roomPlaceable = false;
			drawSelectedThing = true;
			m_canBuildFurniture = false;
		}
		else if (m_startTile)
		{
			DirectX::XMFLOAT3 p(m_startTile->getPosition());
			table->setPosition(p.x + 0.5f, p.y - 0.2f, p.z + 0.5f);
			DirectX::XMINT2 start;
			start.x = table->getPosition().x;
			start.y = table->getPosition().z;
			//this->grid->ResetTileColor(start, start);
			m_canBuildFurniture = m_roomCtrl->CheckAndMarkTilesObject(start, table->getGridSize(), table->getRotation());
			
			//std::cout << m_startTile->getRotation().y << std::endl;
			//TEMP
			drawSelectedThing = true;
			twoStepThingy = true;
		}
		else
		{
			if (twoStepThingy)
				twoStepThingy = false;
			else drawSelectedThing = false;

		}
	}
	else
		drawSelectedThing = false;


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

	drawSelectedThing = false;

	//TEMP
	door.setMesh(MeshHandler::getDoor());
	table = new Table(DirectX::XMFLOAT3(0, 0, 0), MeshHandler::getTable());
}

BuildState::~BuildState()
{
	if (m_roomCtrl->getIsBuildingDoor())
		m_roomCtrl->setIsBuildingDoor(false);
	delete table;
}

void BuildState::_init()
{
	m_madeFullResetMain = true;
	m_selectedThing = -1;
	m_selectedRoom = nullptr;
	p_HUD.LoadHud("trolls_inn/Resources/HUD/BuildHud/BuildHud.txt");
	for (int i = 0; i < p_HUD.getNrOfPickableButtons(); i++)
		m_hudButtonsPressed.push_back(false);

	m_roomHUD.LoadHud("trolls_inn/Resources/HUD/BuildHud/RoomBuild/RoomBuildHud.txt");

	m_doorHUD.LoadHud("trolls_inn/Resources/HUD/BuildHud/DoorBuild/DoorBuildHud.txt");

	m_furnitureHUD.LoadHud("trolls_inn/Resources/HUD/BuildHud/FurnitureBuild/FurnitureBuildHud.txt");
}

void BuildState::Update(double deltaTime)
{
	if (m_selectedRoom && Input::isKeyPressed(Input::Del))
	{
		DirectX::XMFLOAT3 p = m_selectedRoom->getPosition();
		DirectX::XMINT2 pos(p.x + 0.5f, p.z + 0.5f);
		std::vector<Tile*> backtiles;
		DirectX::XMINT2 delPos;
		DirectX::XMINT2 delSize;
		
		if (m_roomCtrl->RemoveRoom(pos, backtiles, delPos, delSize));
		{
			m_selectedRoom->Select();
			grid->insertTiles(delPos, delSize, backtiles);
			m_selectedRoom = nullptr;
		}
	}

}

void BuildState::Draw()
{
	
	switch (m_currentBuildType)
	{
	case BuildState::RoomBuild:
		if (m_buildStage == BuildStage::Selection)
			m_priceOfRoom.Draw();
		break;
	case BuildState::Door:
		if (drawSelectedThing)
			door.Draw();
		break;
	case BuildState::Furniture:
		if (drawSelectedThing)
			table->Draw();
		break;
	default:
		break;
	}

}

void BuildState::DrawHUD()
{
	p_HUD.Draw();

	switch (m_currentBuildType)
	{
	case CurrentBuildType::RoomBuild:
		m_roomHUD.Draw();
		break;
	case CurrentBuildType::Door:
		m_doorHUD.Draw();
		break;
	case CurrentBuildType::Furniture:
		m_furnitureHUD.Draw();
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
