#include "GameState.h"
#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <thread>
#include <future>
#include "../../../ourEngine/interface/light/PointLight.h"
#include "../../../ourEngine/core/Dx.h"
#include "../StateManager/SubStates/BuildState.h"
#include "../../Mesh Manager/MeshManager.h"
#include "../../Furniture/Table.h"

GameState::GameState(std::stack<Shape*>* pickingEvent, std::stack<int>* keyEvent, Camera * cam) : State(pickingEvent, keyEvent)
{
	m_stage = GameStage::Play;
	m_clickedLastFrame = false;
	m_madeFullReset = true;
	m_lastClickedIndex = -1;
	// Building END
	m_Rpressed = false;
	_setHud();

	int nrOfButtons = m_stateHUD.getNrOfPickableButtons();
	{
	for (int i = 0; i < nrOfButtons; i++)
		m_hudButtonsPressed.push_back(false);
	}
	c.setModel(MeshHandler::getBox());
	c.setPosition(5 + 0.5f, 5 + 0.5f);

	table.LoadModel("trolls_inn/Resources/Stol.obj");
	
	int startSize = 32;
	int firstRoomSizeX = 4;
	int firstRoomSizeY = 3;

	int secondRoomSizeX = 4;
	int secondRoomSizeY = 2;

	this->m_cam = cam;
	this->_init();
	m_grid = new Grid(0, 0, startSize, startSize);	
	m_roomctrl = new RoomCtrl();
	m_roomctrl->AddRoom(DirectX::XMINT2((startSize / 2) - firstRoomSizeX / 2, 4), DirectX::XMINT2(firstRoomSizeX, firstRoomSizeY), RoomType::reception, m_grid->extractTiles(DirectX::XMINT2((startSize / 2) - firstRoomSizeX / 2, 4), DirectX::XMINT2(firstRoomSizeX, firstRoomSizeY)));

	m_mai = new MasterAI(m_roomctrl, m_grid);
	previousKey = -1;	

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
		this->m_mai->spawn();	
	}
	lol = lol2;


	this->m_cam->update();
	c.Update();
	m_roomctrl->Update(m_cam);
	//this->grid->Update(this->m_cam);
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
	inn.Update(deltaTime, gameTime.getTimePeriod());
	m_mai->Update(this->m_cam);
	gameTime.updateCurrentTime(static_cast<float>(deltaTime));
	//auto currentTime = std::chrono::high_resolution_clock::now();
	if (Input::isKeyPressed('N')) {
		//m_newState = new MainMenu(p_pickingEvent, p_keyEvents, m_cam);
	}
	auto currentTime = std::chrono::high_resolution_clock::now();
	
	
	auto time = std::chrono::high_resolution_clock::now();
	auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(time - currentTime).count();
	

	if (Input::isKeyPressed('D'))
	{
		m_roomctrl->PickRoomTiles();
	}
	if (Input::isKeyPressed('C'))
	{
		m_roomctrl->PickWalls();
	}
	

	//<TEMP>
	/*c.Update();
	if (c.walkQueueDone())
	{
		if ((int)((c.getPosition().x - 0.5) / 1) == m_mainDoorPos.x && (int)(round_n(c.getPosition().y, 1)) == m_mainDoorPos.y && m_justMoved == false)
		{
			c.Move(Character::UP);
			std::cout << " " << c.getPosition().x << " " << c.getPosition().y << std::endl;
			m_justMoved = true;
		}
		else if ((int)((c.getPosition().x - 0.5) / 1) == m_mainDoorPos.x && (int)(round_n(c.getPosition().y, 1)) == m_mainDoorPos.y + 1 && m_justMoved == false)
		{
			c.Move(Character::DOWN);
			std::cout << " " << c.getPosition().x << " " << c.getPosition().y << std::endl;
			m_justMoved = true;
		}
	}*/
}

void GameState::Draw()
{
	gameTime.m_cpyLightToGPU();
	
	m_roomctrl->Draw();
	this->m_grid->Draw();

	//TEST
	c.Draw();
	//this->grid2->Draw();

	//m_mai.Draw();
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
	kitchenTile.MakeRectangle();
	kitchenTile.setDiffuseTexture("trolls_inn/Resources/Untitled.bmp");
	kitchenTile.setNormalTexture("trolls_inn/Resources/BatmanNormal.png");
	rect.MakeRectangle();
	rect.setDiffuseTexture("trolls_inn/Resources/Grass.jpg");
	rect.setNormalTexture("trolls_inn/Resources/GrassNormal.png");
	//door.LoadModel("trolls_inn/Resources/door/Door.obj");
	//door.setNormalTexture("trolls_inn/Resources/door/SickDoorNormal.png");
	this->m.LoadModel("trolls_inn/Resources/Wall3.obj");
	this->m.setNormalTexture("trolls_inn/Resources/woodNormalMap.jpg");
}

void GameState::_setHud()
{
	m_stateHUD.LoadHud("trolls_inn/Resources/HUD/MainHud/MainHud.txt");
}

void GameState::_handlePicking()
{
	bool hudWasPicked = _handleHUDPicking();
	if (m_stage == GameStage::Play)
	{
		m_grid->PickTiles();
		m_roomctrl->PickRoomTiles();
	}

	if (hudWasPicked)
		while (!p_pickingEvent->empty()) this->p_pickingEvent->pop();

	while (!p_pickingEvent->empty())
	{
		Shape * obj = this->p_pickingEvent->top();
		this->p_pickingEvent->pop();

		
		if (!m_subStates.empty())
		{
			SubState* ss = m_subStates.top();
			ss->HandlePicking(obj);
		}
		
		/*if (Input::isKeyPressed('G'))
		{
			Table fut = Table(obj->getPosition(), &table);
			bool test = this->grid->CheckAndMarkTilesObject(DirectX::XMINT2(obj->getPosition().x, obj->getPosition().z), fut.getGridSize(), fut.getRotation());
			if (test)
			{
				this->grid->AddRoomObject(fut);
			}
			
		}*/

		/*if (Input::isKeyPressed('C'))
		{
			m_roomctrl->CreateDoor(obj->getPosition());
		}
		else if (Input::isKeyPressed('D'))
		{

			XMINT2 delPos = { static_cast<int>(obj->getPosition().x), static_cast<int>(obj->getPosition().z) };

			std::vector<Tile*> tiles;
			XMINT2 roomPos;
			XMINT2 roomSize;
			bool remove = m_roomctrl->RemoveRoom(delPos,tiles,roomPos,roomSize);
			if(remove)
				grid->insertTiles(roomPos,roomSize,tiles);
		
		}
	
		

*/
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
		if (c.walkQueueDone())
		{
			////Shape * obj = this->p_pickingEvent->top();
			//XMFLOAT2 charPos = c.getPosition(); // (x,y) == (x,z,0)

			//int xTile = (int)(round_n(charPos.x, 1) - 0.5f);
			//int yTile = (int)(round_n(charPos.y, 1) - 0.5f);


			//// Ta reda på om det är outside -> inside
			//
			//
			//XMINT2 targetPosition = { (int)obj->getPosition().x , (int)obj->getPosition().z };
			//
			//XMINT2 startPosition = { xTile, yTile };

			//auto path = getPathAndEatAss(startPosition, targetPosition);
			//

			//XMFLOAT3 oldPos = { float(xTile),0.0f, float(yTile) };

			//if (path.size() != 0)
			//{
			//	m_justMoved = false;

			//	c.Move(c.getDirectionFromPoint(oldPos, path[0]->tile->getQuad().getPosition()));

			//	for (int i = 0; i < path.size() - 1; i++)
			//		c.Move(c.getDirectionFromPoint(path[i]->tile->getQuad().getPosition(), path[i + 1]->tile->getQuad().getPosition()));
			//}

		}
	}
	
	

}


bool GameState::_handleHUDPicking()
{
	bool pickedHUD = false;
	int index = m_stateHUD.PickHud(Input::getMousePositionLH());

	if (index != -2) pickedHUD = true;
		
	
	if (index >= 0)
	{
		float cH = 5.0f;
		float cHL = 2.0f;
		float cC = 50.0f;
		m_madeFullReset = false;
		if (!m_clickedLastFrame && Input::isMouseLeftPressed())
		{
			m_lastClickedIndex = index;
			// Clicked a button
			while (!m_subStates.empty())
			{
				SubState * s = m_subStates.top();
				m_subStates.pop();
				delete s;
			}
			_resetHudButtonPressedExcept(index);
			m_hudButtonsPressed[index] = !m_hudButtonsPressed[index];
			m_clickedLastFrame = true;

			switch (index)
			{
			case 0:
				// Crew Button
				std::cout << "Crew Button Pressed\n";
				m_stateHUD.SetColorOnButton(index, cC, cHL, cHL);
				break;
			case 1:
				// Build Button
				std::cout << "Build Button Pressed\n";
				m_stateHUD.SetColorOnButton(index, cHL, cC, cHL);
				if (m_hudButtonsPressed[index])
					m_subStates.push(new BuildState(m_cam, p_pickingEvent, m_grid, m_roomctrl));
				break;
			case 2:
				// Event Button
				std::cout << "Event Button Pressed\n";
				m_stateHUD.SetColorOnButton(index, cHL, cHL, cC);
				break;
			case 3:
				// Stats Button
				std::cout << "Stats Button Pressed\n";
				m_stateHUD.SetColorOnButton(index, cC, cHL / 4, cC);
				break;
			}

		}
		else
		{
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
		for (size_t i = 0; i < m_hudButtonsPressed.size(); i++)
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

		
	if (m_stage == GameStage::Play && m_subStates.empty())
	{
		if (Input::isMouseLeftPressed())
		{
			//this->grid->PickTiles();
			m_move = true;
		}
		else
			m_move = false;
	}

	if (Input::isKeyPressed('P'))
		m_stage = GameStage::Play;

	

	if (Input::isKeyPressed('R') && !m_Rpressed)
		_setHud();
	else if (!Input::isKeyPressed('R'))
		m_Rpressed = false;
}

std::vector<std::shared_ptr<Node>> GameState::getPathAndEatAss(XMINT2 startPosition, XMINT2 targetPosition)
{

	Tile* startTile = m_grid->getTile(startPosition.x, startPosition.y);
	Tile* targetTile = m_grid->getTile(targetPosition.x, targetPosition.y);

	std::vector<std::shared_ptr<Node>> path;
	// Outside -> OutSide
	if (startTile && targetTile)
	{
		path = m_grid->findPath(startTile, targetTile);
	}
	// Outside -> inside
	else if (startTile && 0 == targetTile)
	{
		XMINT2 pos = targetPosition;
		int index = m_roomctrl->_intersect(pos);
		Room* targetRoom = m_roomctrl->getRoomAt(index);

		if (0 == m_roomctrl->getRoomConnections(index))
			return std::vector<std::shared_ptr<Node>>();

		RoomCtrl::DoorPassage entranceDoor = m_roomctrl->getClosestEntranceDoor(startPosition);
		// Now do we wanna walk to the entrance
		path = m_grid->findPath(startTile, m_grid->getTile(entranceDoor.one.x, entranceDoor.one.y));

		//Lets not talk about this one(This is so we walk straight through the door...)
		auto walkThroughDoor = targetRoom->findPath(m_grid->getTile(entranceDoor.one.x, entranceDoor.one.y), targetRoom->getTile(entranceDoor.two.x, entranceDoor.two.y));
		path.insert(path.end(), walkThroughDoor.begin(), walkThroughDoor.end());

		//// Easy check, if its the main room we are wanting then we only need to perform pathfinding inside that room
		//// and not start the higher level room path finding
		//if (*endTile->getRoom() == *m_roomCtrl.getMainRoom())
		//{
		//	auto doorToEndTile = findPath(m_tiles[m_roomCtrl.getMainDoorPosLeave().x][m_roomCtrl.getMainDoorPosLeave().y], endTile, false);
		//	path.insert(path.end(), doorToEndTile.begin(), doorToEndTile.end());
		//}
		//// We want to be advanced :P
		//else
		//{
		//	std::vector<int> roomIndexes = m_roomCtrl.roomTraversal(m_tiles[m_roomCtrl.getMainDoorPosLeave().x][m_roomCtrl.getMainDoorPosLeave().y], endTile);

		//	Room* startRoom = m_roomCtrl.getMainRoom();
		//	roomIndexes.erase(roomIndexes.begin());
		//	XMINT2 DoorLeavePos;
		//	XMINT2 startPosition = m_roomCtrl.getMainDoorPosLeave();
		//	for (auto index : roomIndexes)
		//	{
		//		// Between this rooms leave door and other rooms enter door
		//		XMINT2 DoorEnterPos = m_roomCtrl.getRoomEnterPos(startRoom, index);
		//		auto toOtherRoom = findPath(m_tiles[startPosition.x][startPosition.y], m_tiles[DoorEnterPos.x][DoorEnterPos.y], false);
		//		path.insert(path.end(), toOtherRoom.begin(), toOtherRoom.end());

		//		// Smooth Entering
		//		DoorLeavePos = m_roomCtrl.getRoomLeavePos(startRoom, index);
		//		auto dontAsk = findPath(m_tiles[DoorEnterPos.x][DoorEnterPos.y], m_tiles[DoorLeavePos.x][DoorLeavePos.y], false);
		//		path.insert(path.end(), dontAsk.begin(), dontAsk.end());
		//		startRoom = m_roomCtrl.getRoomAt(index);
		//		startPosition = DoorLeavePos;

		//	}


		//	auto toTarget = findPath(m_tiles[DoorLeavePos.x][DoorLeavePos.y], endTile, false);
		//	path.insert(path.end(), toTarget.begin(), toTarget.end());

		//}

	}
	//// Inside -> Inside
	//else if (1 == startTile->getIsInside() && 1 == endTile->getIsInside())
	//{
	//	if (0 == endTile->getRoom()->hasConnectedRooms())
	//		return std::vector<std::shared_ptr<Node>>();

	//	if (*startTile->getRoom() == *endTile->getRoom())
	//	{
	//		auto doorToEndTile = findPath(startTile, endTile, false);
	//		path.insert(path.end(), doorToEndTile.begin(), doorToEndTile.end());
	//	}
	//	else
	//	{
	//		std::vector<int> roomIndexes = m_roomCtrl.roomTraversal(startTile, endTile);

	//		Room* startRoom = startTile->getRoom();
	//		roomIndexes.erase(roomIndexes.begin());
	//		XMINT2 DoorLeavePos;
	//		XMINT2 startPosition = { startTile->getPosX(), startTile->getPosY() };
	//		for (auto index : roomIndexes)
	//		{
	//			// Between this rooms leave door and other rooms enter door
	//			XMINT2 DoorEnterPos = m_roomCtrl.getRoomEnterPos(startRoom, index);
	//			auto toOtherRoom = findPath(m_tiles[startPosition.x][startPosition.y], m_tiles[DoorEnterPos.x][DoorEnterPos.y], false);
	//			path.insert(path.end(), toOtherRoom.begin(), toOtherRoom.end());

	//			// Smooth Entering
	//			DoorLeavePos = m_roomCtrl.getRoomLeavePos(startRoom, index);
	//			auto dontAsk = findPath(m_tiles[DoorEnterPos.x][DoorEnterPos.y], m_tiles[DoorLeavePos.x][DoorLeavePos.y], false);
	//			path.insert(path.end(), dontAsk.begin(), dontAsk.end());
	//			startRoom = m_roomCtrl.getRoomAt(index);
	//			startPosition = DoorLeavePos;

	//		}
	//		auto toTarget = findPath(m_tiles[DoorLeavePos.x][DoorLeavePos.y], endTile, false);
	//		path.insert(path.end(), toTarget.begin(), toTarget.end());
	//	}

	//}
	////Inside to outside
	//else
	//{
	//	if (*startTile->getRoom() == *m_roomCtrl.getMainRoom())
	//	{
	//		// Path to door
	//		auto toDoor = findPath(startTile, m_tiles[m_roomCtrl.getMainDoorPosLeave().x][m_roomCtrl.getMainDoorPosLeave().y], false);
	//		path.insert(path.end(), toDoor.begin(), toDoor.end());

	//		//Lets not talk about this one(This is so we walk straight through the door...)
	//		auto walkThroughDoor = findPath(m_tiles[m_roomCtrl.getMainDoorPosLeave().x][m_roomCtrl.getMainDoorPosLeave().y], m_tiles[m_roomCtrl.getMainDoorPosEnter().x][m_roomCtrl.getMainDoorPosEnter().y], false);
	//		path.insert(path.end(), walkThroughDoor.begin(), walkThroughDoor.end());

	//		auto walkToTarget = findPath(m_tiles[m_roomCtrl.getMainDoorPosEnter().x][m_roomCtrl.getMainDoorPosEnter().y], endTile, true);
	//		path.insert(path.end(), walkToTarget.begin(), walkToTarget.end());
	//	}
	//	else
	//	{
	//		// We need to find the entrace from inside then we path to the destination
	//		std::vector<int> roomIndexes = m_roomCtrl.roomTraversal(startTile, m_tiles[m_roomCtrl.getMainDoorPosLeave().x][m_roomCtrl.getMainDoorPosLeave().y]);

	//		Room* startRoom = startTile->getRoom();
	//		roomIndexes.erase(roomIndexes.begin());
	//		XMINT2 DoorLeavePos;
	//		XMINT2 startPosition = { startTile->getPosX(), startTile->getPosY() };
	//		for (auto index : roomIndexes)
	//		{
	//			// Between this rooms leave door and other rooms enter door
	//			XMINT2 DoorEnterPos = m_roomCtrl.getRoomEnterPos(startRoom, index);
	//			auto toOtherRoom = findPath(m_tiles[startPosition.x][startPosition.y], m_tiles[DoorEnterPos.x][DoorEnterPos.y], false);
	//			path.insert(path.end(), toOtherRoom.begin(), toOtherRoom.end());

	//			// Smooth Entering
	//			DoorLeavePos = m_roomCtrl.getRoomLeavePos(startRoom, index);
	//			auto dontAsk = findPath(m_tiles[DoorEnterPos.x][DoorEnterPos.y], m_tiles[DoorLeavePos.x][DoorLeavePos.y], false);
	//			path.insert(path.end(), dontAsk.begin(), dontAsk.end());

	//			startRoom = m_roomCtrl.getRoomAt(index);
	//			startPosition = DoorLeavePos;

	//		}
	//		auto toMainLeave = findPath(m_tiles[DoorLeavePos.x][DoorLeavePos.y], m_tiles[m_roomCtrl.getMainDoorPosLeave().x][m_roomCtrl.getMainDoorPosLeave().y], false);
	//		path.insert(path.end(), toMainLeave.begin(), toMainLeave.end());

	//		auto smooth = findPath(m_tiles[m_roomCtrl.getMainDoorPosLeave().x][m_roomCtrl.getMainDoorPosLeave().y], m_tiles[m_roomCtrl.getMainDoorPosEnter().x][m_roomCtrl.getMainDoorPosEnter().y], false);
	//		path.insert(path.end(), smooth.begin(), smooth.end());

	//		auto finallyTarget = findPath(m_tiles[m_roomCtrl.getMainDoorPosEnter().x][m_roomCtrl.getMainDoorPosEnter().y], endTile, true);
	//		path.insert(path.end(), finallyTarget.begin(), finallyTarget.end());

	//	}



	//}
	return path;
}

