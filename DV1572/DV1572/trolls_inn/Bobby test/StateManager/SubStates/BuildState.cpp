#include "BuildState.h"
#include "../../../Mesh Manager/MeshManager.h"
#include "../../../Furniture/Stove.h"
#include "../../../Furniture/Bar.h"
#include "../../../Furniture/Chair.h"
#include "../../../Furniture/Reception.h"

#define DEBUG = 1;

void BuildState::_resetHudButtonPressedExcept(int index, std::vector<bool> &vec, HUD &selectedHud)
{
	for (int i = 0; i < vec.size(); i++)
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
				else if(m_selectedRoom)
				{
					m_selectedRoom->Select();
					m_selectedRoom = nullptr;
				}
				else
					m_buildStage = BuildStage::None;
				break;
			case BuildStage::Selection:
				if (pickedShape) {
					m_selectedTile = pickedShape;
				}
				else if (m_selectedRoom)
				{
					m_selectedRoom->Select();
					m_selectedRoom = nullptr;
				}
				break;
			case BuildStage::None:
				if (pickedShape)
				{
					DirectX::XMINT2 pos(static_cast<int32_t>(pickedShape->getPosition().x), static_cast<int32_t>(pickedShape->getPosition().z));
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
			if (m_selectedRoom)
			{
				m_selectedRoom->Select();
				m_selectedRoom = nullptr;
			}
			m_startTile = pickedShape;
			break;
		case BuildState::Furniture:
			if (m_selectedRoom)
			{
				m_selectedRoom->Select();
				m_selectedRoom = nullptr;
			}

			if (m_selectedThing == -1)
			{
				if (Input::isMouseLeftPressed() && !m_clickedLast)
				{
					if (!m_furnitureRemove || (m_furnitureRemove && m_cm->ButtonClicked() == -2))
					{
						if (pickedShape)
						{
							m_furnitureDeleteMode = true;
							m_clickedLast = true;

							DirectX::XMINT2 pos(static_cast<int32_t>(pickedShape->getPosition().x), static_cast<int32_t>(pickedShape->getPosition().z));
							class::Furniture* newPick = m_roomCtrl->getFurnitureAtPos(pos);
							if (m_furnitureRemove)
							{
								if (m_cm->ButtonClicked() == -2)
								{
									if (m_furnitureRemove == newPick)
									{
										float dirty = 1.0f / float(m_furnitureRemove->getDirtyStat());
										m_furnitureRemove->getObject3D().setColor(dirty, 0, 0);
										m_furnitureRemove = nullptr;
									}
									else
									{
										m_furnitureRemove->getObject3D().setColor(1, 1, 1);
										m_furnitureRemove = newPick;
										m_furnitureRemove->getObject3D().setColor(0.2f, 2.0f, 0.2f);
										m_cm->ClearSubText();
										//m_cm->setInfo(m_furnitureRemove->WhatType());
										std::string s = "\n\n\n\n\n" + m_furnitureRemove->getInfo(m_furnitureRemove->getType()) + 
											"\nLevel" + std::to_string(m_furnitureRemove->getLevel())
											+ "\nClean: " + std::to_string(m_furnitureRemove->getDirtyStat());
										m_cm->PushText(s);
										m_cm->setPos(Input::getMousePositionLH());
									}
								}
							}
							else
							{
								m_furnitureRemove = m_roomCtrl->getFurnitureAtPos(pos);
								if (m_furnitureRemove)
								{
									m_furnitureRemove->getObject3D().setColor(0.2f, 2.0f, 0.2f);
									m_cm->ClearSubText();
									//m_cm->setInfo(m_furnitureRemove->WhatType());
									std::string s = "\n\n\n\n\n" + m_furnitureRemove->getInfo(m_furnitureRemove->getType()) +
										"\nLevel: " + std::to_string(m_furnitureRemove->getLevel())
										+ "\nClean: " + std::to_string(m_furnitureRemove->getDirtyStat());
									m_cm->PushText(s);

									m_cm->setPos(Input::getMousePositionLH());
								}
							}

						}
						else if (m_furnitureRemove)
						{
							if (m_cm->ButtonClicked() == -2)
							{
								m_furnitureRemove->getObject3D().setColor(1, 1, 1);
								m_furnitureRemove = nullptr;
							}
						}
					}

				}
				else if (m_clickedLast && !Input::isMouseLeftPressed())
				{
					m_clickedLast = false;
				}
			}
			else 
			{
				m_furnitureDeleteMode = false;
				m_startTile = pickedShape;
				/*if (m_furnitureRemove)
				{
					m_furnitureRemove->getObject3D().setColor(1, 1, 1);
					m_furnitureRemove = nullptr;
				}*/
				 // Chair (Looks like table)
				
				if (m_selectedThing == 0)
				{
					// This is the first selected image, the chair
					if (lastSelected != m_selectedThing)
					{
						lastSelected = m_selectedThing;
						delete table;
						table = new Table(XMFLOAT3(0, 0, 0), MESH::TABLE_LOW);
					}
					
				}
				// Table
				else if (m_selectedThing == 1)
				{

					if (lastSelected != m_selectedThing)
					{
						lastSelected = m_selectedThing;
						delete table;
						table = new Table(XMFLOAT3(0, 0, 0), MESH::TABLE_HIGH,2);
					}
				}
				// Bed High
				else if (m_selectedThing == 2)
				{

					if (lastSelected != m_selectedThing)
					{
						lastSelected = m_selectedThing;
						delete table;
						table = new Bed(XMFLOAT3(0, 0, 0), MESH::BED_HIGH);
					}
				}
				// Bed Low
				else if (m_selectedThing == 3)
				{

					if (lastSelected != m_selectedThing)
					{
						lastSelected = m_selectedThing;
						delete table;
						table = new Bed(XMFLOAT3(0, 0, 0), MESH::BED_LOW);
					}
				}
				// Bar high
				else if (m_selectedThing == 4)
				{

					if (lastSelected != m_selectedThing)
					{
						lastSelected = m_selectedThing;
						delete table;
						table = new Bar(XMFLOAT3(0, 0, 0), MESH::BAR_HIGH);
					}
				}
				// Bar low
				else if (m_selectedThing == 5)
				{

					if (lastSelected != m_selectedThing)
					{
						lastSelected = m_selectedThing;
						delete table;
						table = new Bar(XMFLOAT3(0, 0, 0), MESH::BAR_LOW);
					}
				}

				// Chair High
				else if (m_selectedThing == 6)
				{

					if (lastSelected != m_selectedThing)
					{
						lastSelected = m_selectedThing;
						delete table;
						table = new Chair(XMFLOAT3(0, 0, 0), MESH::CHAIR_HIGH);
					}
				}
				// Chair low
				else if (m_selectedThing == 7)
				{

					if (lastSelected != m_selectedThing)
					{
						lastSelected = m_selectedThing;
						delete table;
						table = new Chair(XMFLOAT3(0, 0, 0), MESH::CHAIR_LOW);
					}
				}

				// Stove
				else if (m_selectedThing == 8)
				{

					if (lastSelected != m_selectedThing)
					{
						lastSelected = m_selectedThing;
						delete table;
						table = new Stove(XMFLOAT3(0, 0, 0), MESH::STOVE);
					}
				}

				// Reception HIGH
				else if (m_selectedThing == 9)
				{

					if (lastSelected != m_selectedThing)
					{
						lastSelected = m_selectedThing;
						delete table;
						table = new Reception(XMFLOAT3(0, 0, 0), MESH::RECEPTION_HIGH);
					}
				}
				// Reception LOW
				else if (m_selectedThing == 10)
				{

					if (lastSelected != m_selectedThing)
					{
						lastSelected = m_selectedThing;
						delete table;
						table = new Reception(XMFLOAT3(0, 0, 0), MESH::RECEPTION_LOW);
					}
				}
				
				
			}
			break;
		default:
			m_startTile = nullptr;
			m_selectedTile = nullptr;
			if (m_selectedRoomm)
			{
				m_selectedRoomm->Select();
			}
			m_selectedRoomm = nullptr;
			if (m_selectedRoom)
			{
				m_selectedRoom->Select();
			}
			m_selectedRoom = nullptr;
			
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
	m_drawFloatingText = false;
	hudPicked = _mainHudPick();
	int hover = -1;
	if (!hudPicked)
	{
		if (m_currentBuildType == CurrentBuildType::RoomBuild)
		{
			hudPicked = _selectionBuildHudPick(m_roomHUD, hover);
		}
		else if (m_currentBuildType == CurrentBuildType::Door)
		{
			hudPicked = _selectionBuildHudPick(m_doorHUD, hover);
		}
		else if (m_currentBuildType == CurrentBuildType::Furniture)
		{
			hudPicked = _selectionBuildHudPick(m_furnitureHUD, hover);
		}
	}

	if (hover >= 0)
	{
		switch (m_currentBuildType)
		{
		case Furniture:
			m_floatingText.setString(Furniture::getInfo(hover));
			m_floatingText.setPosition(Input::getMousePositionLH().x, Input::getMousePositionLH().y);
			m_drawFloatingText = true;
		break;
		default:
			break;
		}
		
	}
	return hudPicked;
}

bool BuildState::_mainHudPick()
{
	bool pickedHUD = false;
	int index = p_HUD.PickHud(Input::getMousePositionLH());
	static int lastHover = -1;
	if (index != -2) pickedHUD = true;

	if (index < 0)
		lastHover = -1;

	if (index >= 0)
	{
		m_drawFloatingText = true;
		m_floatingText.setPosition(Input::getMousePositionLH().x, Input::getMousePositionLH().y);
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
			if (m_hudClick->GetState() == DirectX::SoundState::PLAYING)
				m_hudClick->Stop();
			m_hudClick->Play();

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
			for (int i = 0; i < m_hudButtonsPressed.size() && picked == -1; i++)
			{
				if (m_hudButtonsPressed[i])
					picked = i;
			}
			
			if (lastHover != index)
			{
				lastHover = index;
				if (m_hudSnap->GetState() == DirectX::SoundState::PLAYING)
					m_hudSnap->Stop();
				m_hudSnap->Play();
			}

			p_HUD.ResetColorsExcept(picked);

			switch (index)
			{
			case 0:
				// Rooms Button
				if (!m_hudButtonsPressed[index])
					p_HUD.SetColorOnButton(index, cH, cHL, cHL);
				m_floatingText.setString("Build Rooms");
				break;
			case 1:
				// Furniture Button
				if (!m_hudButtonsPressed[index])
					p_HUD.SetColorOnButton(index, cHL, cH, cHL);
				m_floatingText.setString("Build Furniture");
				break;
			case 2:
				// Door Button
				if (!m_hudButtonsPressed[index])
					p_HUD.SetColorOnButton(index, cHL, cHL, cH);
				m_floatingText.setString("Build Doors");
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
		for (int i = 0; i < m_hudButtonsPressed.size(); i++)
		{
			if (!m_hudButtonsPressed[i])
				p_HUD.ResetColorsOnPickableWithIndex(i);
		}
		m_madeFullResetMain = true;
	}

	return pickedHUD;
}


bool BuildState::_selectionBuildHudPick(HUD & h, int & getIndex)
{
	bool pickedHUD = false;
	int index = h.PickHud(Input::getMousePositionLH());
	static int lastHover = -1;

	if (index != -2) pickedHUD = true;
	if (index < 0)
		lastHover = -1;


	if (index >= 0)
	{
		getIndex = index;
		m_floatingText.setPosition(Input::getMousePositionLH().x, Input::getMousePositionLH().y);
		
		float cH = 5.0f;
		float cHL = 2.0f;
		float cC = 50.0f;
		if (!m_clickedLastFrame && Input::isMouseLeftPressed())
		{
			if (m_hudClick->GetState() == DirectX::SoundState::PLAYING)
				m_hudClick->Stop();
			m_hudClick->Play();
			// Clicked a button
			m_clickedLastFrame = true;
			if (m_selectedThing != index)
			{
				m_selectedThing = index;
				h.SetPressColorOnButton(m_selectedThing);
				if (m_selectedRoom)
				{
					m_selectedRoom->Select();
					m_selectedRoom = nullptr;
				}
			}
			else
			{
				m_selectedThing = -1;
				h.ResetColorsExcept(m_selectedThing);
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
				std::string textString = "Area: " + std::to_string(area) + " Price: " + std::to_string(area * 20); //20 is the roomPrice
				m_priceOfRoom.setTextString(textString);
				DirectX::XMFLOAT2 mp = Input::getMousePositionLH();
				m_priceOfRoom.setPosition(mp.x, mp.y + 30);
				if (area * 20 <= m_inn->getMoney())
				{
					m_roomPlaceable = this->grid->CheckAndMarkTiles(start, end);
				}
				else {
					m_roomPlaceable = this->grid->MarkAllTilesRed(start, end);
				}
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
	int area = (abs(end.x - start.x) + 1) * (abs(end.y - start.y) + 1);
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
		
		m_inn->Withdraw(area * 20);
		m_inn->IncreaseXP(area * 5);
		//m_inn->Deposit(area);
		//m_inn->UpdateMoney();
		
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
		if (m_startTile && Input::isMouseLeftPressed(false))
		{
			XMFLOAT3 s = m_startTile->getPosition();
			m_buildStage = BuildStage::None;
			m_startTile = nullptr;
			m_selectedTile = nullptr;
			m_roomPlaceable = false;
			m_roomCtrl->CreateDoor(s, door.getRotation().y);
			drawSelectedThing = true;
		}
		else if (m_startTile)
		{
			static bool _srot = false;
			if (Input::isKeyPressed(Input::Comma) || Input::isKeyPressed(Input::Period))
			{
				if (!rotationKeyPressed)
					_srot = !_srot;
				rotationKeyPressed = true;
			}
			else
				rotationKeyPressed = false;

			DirectX::XMFLOAT3 p(m_startTile->getPosition());
			door.setPos(p.x, p.y, p.z);
			door.setRotation(0.0f, DirectX::XMConvertToDegrees(m_startTile->getRotation().y) + (_srot * 180.0f), 0.0f);
			door.setScale(1.01f);
			//TEMP
			drawSelectedThing = true;
			twoStepThingy = true;
		}
		else
		{
			if (twoStepThingy)
				twoStepThingy = false;
			else 
				drawSelectedThing = false;
			
		}
	}
	else
		drawSelectedThing = false;

}

void BuildState::_inputFurniture()
{
	if (!m_furnitureRemove || m_cm->ButtonClicked() == -2)
	{
		if (m_selectedThing == -1 && Input::isMouseLeftPressed())
		{
			m_roomCtrl->PickAllFurnitures();
		}
		if (m_selectedThing != -1)
		{
			m_roomCtrl->PickRoomTiles();
			if (Input::isKeyPressed(Input::Comma))
			{
				if (!rotationKeyPressed)
					table->setRotation(table->getRotation() + 90);
				rotationKeyPressed = true;
			}
			else if (Input::isKeyPressed(Input::Period))
			{
				if (!rotationKeyPressed)
					table->setRotation(table->getRotation() - 90);
				rotationKeyPressed = true;
			}
			else
			{
				rotationKeyPressed = false;
			}

			if (m_startTile && Input::isMouseLeftPressed())
			{
				if (table)
				{
					XMINT3 s = table->getPosition();
					if (s.x != -1)
					{
						XMINT2 start;
						start.x = s.x;
						start.y = s.z;
						if (m_canBuildFurniture)
						{
							m_roomCtrl->AddRoomObject(table);
							m_inn->FurnitureStatAdd(table->getAttributes());
							
							
							if (!m_freeFurniture)
							{
								m_inn->Withdraw(table->getPrice());
							}
							else
							{
								lastSelected = -1;
								m_selectedThing = -1;
								m_freeFurniture = false;
								delete table;
								table = nullptr;
							}

							//m_inn->UpdateMoney();
						}

						m_buildStage = BuildStage::None;
						m_startTile = nullptr;
						m_selectedTile = nullptr;
						m_roomPlaceable = false;
						drawSelectedThing = true;
						m_canBuildFurniture = false;
					}
				}
			}

			else if (m_startTile)
			{
				if (table != nullptr)
				{
					DirectX::XMFLOAT3 p(m_startTile->getPosition());
					table->setPosition(p.x + 0.5f, p.y - 0.2f, p.z + 0.5f);
					table->setLightIndex(m_startTile->getLightIndex());
					DirectX::XMINT2 start;
					start.x = table->getPosition().x;
					start.y = table->getPosition().z;
					//this->grid->ResetTileColor(start, start);
					if (m_freeFurniture || m_inn->getMoney() >= table->getPrice())
					{
						m_canBuildFurniture = m_roomCtrl->CheckAndMarkTilesObject(start, table->getGridSize(), table->getRotation());
						if (!m_canBuildFurniture)
						{
							table->getObject3D().setColor(3.0f, 0.1f, 0.1f);
							m_startTile->setColor(XMFLOAT3(5.5f, 0.5f, 0.5f));
						}
						else
						{
							table->getObject3D().setColor(1, 1, 1);
						}
					}
					else
					{
						table->getObject3D().setColor(3.0f, 0.1f, 0.1f);
						m_startTile->setColor(XMFLOAT3(5.5f, 0.5f, 0.5f));
						//m_canBuildFurniture = m_roomCtrl->MarkAllTilesRedObject(start, table->getGridSize(), table->getRotation());
					}

				}


				//std::cout << m_startTile->getRotation().y << std::endl;
				//TEMP
				drawSelectedThing = true;
				twoStepThingy = true;
			}
			//std::cout << m_startTile->getRotation().y << std::endl;
			//TEMP
			drawSelectedThing = true;
			twoStepThingy = true;
		}
		else
			drawSelectedThing = false;
	}
	else
		drawSelectedThing = false;
}


BuildState::BuildState(Camera * cam,
	std::stack<Shape *>* pickingEvent,
	Grid * grid, RoomCtrl* roomCtrl, Inn * i, Staff* troll) : SubState(cam, pickingEvent)
{
	this->grid = grid;
	m_roomCtrl = roomCtrl;
	this->_init();
	m_innTroll = troll;
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

	m_inn = i;

	//TEMP
	door.setMesh(MeshHandler::getDoor());
	table = new Bed(DirectX::XMFLOAT3(0, 0, 0), MESH::RECEPTION_HIGH);
}

BuildState::~BuildState()
{
	if (m_roomCtrl->getIsBuildingDoor())
		m_roomCtrl->setIsBuildingDoor(false);
	if (m_selectedRoom)
		m_selectedRoom->Select();
	delete table;
	delete m_cm;
	delete m_hudSnap;
	delete m_hudClick;
}

void BuildState::_init()
{
	m_hudClick = OurMusic::LoadSound("trolls_inn/Resources/sounds/click_menu.wav");
	m_hudSnap = OurMusic::LoadSound("trolls_inn/Resources/sounds/snap_menu.wav");
	m_hudSnap->SetVolume(0.5f);
	m_madeFullResetMain = true;
	m_selectedThing = -1;
	m_selectedRoom = nullptr;
	p_HUD.LoadHud("trolls_inn/Resources/HUD/BuildHud/BuildHud.txt");
	for (int i = 0; i < p_HUD.getNrOfPickableButtons(); i++)
		m_hudButtonsPressed.push_back(false);

	m_roomHUD.LoadHud("trolls_inn/Resources/HUD/BuildHud/RoomBuild/RoomBuildHud.txt");

	m_doorHUD.LoadHud("trolls_inn/Resources/HUD/BuildHud/DoorBuild/DoorBuildHud.txt");

	m_furnitureHUD.LoadHud("trolls_inn/Resources/HUD/BuildHud/FurnitureBuild/FurnitureBuildHud.txt");

	m_drawFloatingText = false;

	m_cm = new ClickMenu(ClickMenu::FUR);

}

void BuildState::Update(double deltaTime)
{
	if (m_selectedRoom && Input::isKeyPressed(Input::Del))
	{
		DirectX::XMFLOAT3 p = m_selectedRoom->getPosition();
		DirectX::XMINT2 pos(static_cast<int32_t>(p.x + 0.5f), static_cast<int32_t>(p.z + 0.5f));
		std::vector<Tile*> backtiles;
		DirectX::XMINT2 delPos;
		DirectX::XMINT2 delSize;
		auto tupleReturn = m_roomCtrl->RemoveRoomTuple(pos, backtiles, delPos, delSize);
		if (std::get<0>(tupleReturn))
		{
			m_inn->Deposit(std::get<1>(tupleReturn));
			m_selectedRoom->Select();
			grid->insertTiles(delPos, delSize, backtiles);
			m_selectedRoom = nullptr;
		}
	}
	if (m_furnitureRemove)
	{
		switch (m_cm->ButtonClicked())
		{
		case 0:
			m_floatingText.setPosition(Input::getMousePositionLH().x, Input::getMousePositionLH().y);
			m_floatingText.setString("Pick Up");
			m_drawFloatingText = true;
			break;
		case 1:
			m_floatingText.setPosition(Input::getMousePositionLH().x, Input::getMousePositionLH().y);
			m_floatingText.setString("Level Up\n$" + std::to_string(m_furnitureRemove->getPriceToLevelUp()));
			m_drawFloatingText = true;
			break;
		case 2:
			m_floatingText.setPosition(Input::getMousePositionLH().x, Input::getMousePositionLH().y);
			m_floatingText.setString("Clean");
			m_drawFloatingText = true;
			break;
		case 3:
			m_floatingText.setPosition(Input::getMousePositionLH().x, Input::getMousePositionLH().y);
			m_floatingText.setString("Sell\n+$" + std::to_string(m_furnitureRemove->getPrice() / 2));
			m_drawFloatingText = true;
			break;
		default:
			m_drawFloatingText = false;
			break;
		}
	}

	if (m_furnitureRemove && m_furnitureDeleteMode && Input::isMouseLeftPressed() && !m_clickedLast)
	{
		m_clickedLast = true;
		if (m_cm->ButtonClicked() == 0 || m_cm->ButtonClicked() == 3)
		{
			if (m_cm->ButtonClicked() == 0)
			{
				float dirty = 1.0f/ float(m_furnitureRemove->getDirtyStat());
				m_furnitureRemove->getObject3D().setColor(dirty, 0, 0);
				m_selectedThing = m_furnitureRemove->getType();
				lastSelected = m_selectedThing;
				if (table) delete table;
				table = m_furnitureRemove->MakeCopy();

				DirectX::XMFLOAT3 lol(static_cast<float>(m_furnitureRemove->getPosition().x), static_cast<float>(m_furnitureRemove->getPosition().y), static_cast<float>(m_furnitureRemove->getPosition().z));
				table->setPosition(lol);
				table->setRotation(m_furnitureRemove->getRotation());
				table->setLevel(m_furnitureRemove->getLevel());
				m_freeFurniture = true;
			}
			else
			{
				int price = m_furnitureRemove->getPrice();
				m_inn->Deposit(price / 2);
			}
		
			DirectX::XMFLOAT3 p = m_furnitureRemove->getObject3D().getPosition();
			DirectX::XMINT2 pos(static_cast<int32_t>(p.x + 0.5f), static_cast<int32_t>(p.z + 0.5f));
			bool tem = m_roomCtrl->RemoveRoomObject(m_furnitureRemove);
			m_furnitureRemove = nullptr;
			m_furnitureDeleteMode = false;
		}
		else if (m_cm->ButtonClicked() == 1)
		{
			m_clickedLast = true;
			m_inn->Withdraw(m_furnitureRemove->AddLevel(m_inn->getMoney()));
			//m_cm->setInfo(m_furnitureRemove->WhatType());
			m_cm->ClearSubText();
			std::string s = "\n\n\n\n\n" + m_furnitureRemove->getInfo(m_furnitureRemove->getType()) +
				"\nLevel" + std::to_string(m_furnitureRemove->getLevel())
				+ "\nClean: " + std::to_string(m_furnitureRemove->getDirtyStat());

			m_cm->PushText(s);
		}
		else if (m_cm->ButtonClicked() == 2)
		{
		
			m_innTroll->setCleaning(true, m_furnitureRemove);

			m_clickedLast = true;

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
			if(table) table->Draw();
		break;
	default:
		break;
	}

	if (m_inn)
	{
		m_inn->Draw();
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
	if (m_furnitureRemove)
		m_cm->Draw();
	if (m_drawFloatingText)
		m_floatingText.Draw();
	
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
