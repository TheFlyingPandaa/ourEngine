#include "TutorialEvent.h"

TutorialEvent::TutorialEvent(Inn * inn, RoomCtrl * roomctrl) : Event(inn,roomctrl)
{

	/*m_queue.push(&TutorialEvent::_BuildReception);
	m_queue.push(&TutorialEvent::_BuildReceptionFurniture);*/
	q.Push(&TutorialEvent::_BuildReception);
	q.Push(&TutorialEvent::_BuildReceptionFurniture);
	q.Push(&TutorialEvent::_BuildDoor);
	q.Push(&TutorialEvent::_BuildBedRoom);
	q.Push(&TutorialEvent::_BuildBedRoomBed);
	q.Push(&TutorialEvent::_BuildKitchenRoom);
	
}

TutorialEvent::~TutorialEvent()
{
}

void TutorialEvent::Update()
{
	if (!q.Empty())
	{
		//CallFunction(q.q.front());
		q.Execute(this);
	}
	
	if (q.Empty())
	{
		exitState();
		InGameConsole::RemoveStringEvent();
	}
}

void TutorialEvent::Draw()
{
}

void TutorialEvent::_BuildReception()
{
	InGameConsole::RemoveStringEvent();
	InGameConsole::pushStringEvent("Build a reception room : Reward $350");
	std::vector<Room*> rooms = m_roomCtrl->getAllTheRooms();
	for (auto room : rooms)
	{
		if (room->getRoomType() == RoomType::reception)
		{
			InGameConsole::pushString("Snyggt byggt, frasig reception");
			m_inn->Deposit(350);
			q.Pop();
		}
	}
	
}

void TutorialEvent::_BuildReceptionFurniture()
{
	InGameConsole::RemoveStringEvent();
	InGameConsole::pushStringEvent("Build a reception desk : Reward $200");
	std::vector<Room*> rooms = m_roomCtrl->getAllTheRooms();
	for (auto room : rooms)
	{
		std::vector<Furniture*> fur = room->getAllRoomFurnitures();
		for (auto furniture : fur)
		{
			if (furniture->WhatType() == "Reception")
			{
				InGameConsole::pushString("Snyggt byggt, frasig reception");
				m_inn->Deposit(200);
				q.Pop();
			}
		}
	}
}

void TutorialEvent::_BuildDoor()
{
	InGameConsole::RemoveStringEvent();
	InGameConsole::pushStringEvent("Build a door : Reward $100");
	std::vector<Room*> rooms = m_roomCtrl->getAllTheRooms();
	for (auto room : rooms)
	{
		std::vector<Wall*>* doors = room->getAllWalls();
		for (auto furniture : *doors)
		{
			if (furniture->getIsDoor())
			{
				InGameConsole::pushString("Snyggt byggt, frasig door");
				m_inn->Deposit(100);
				q.Pop();
			}
		}
	}
}

void TutorialEvent::_BuildBedRoom()
{
	InGameConsole::RemoveStringEvent();
	InGameConsole::pushStringEvent("Build a Bedroom : Reward $150");
	std::vector<Room*> rooms = m_roomCtrl->getAllTheRooms();
	for (auto room : rooms)
	{
		if (room->getRoomType() == RoomType::bedroom)
		{
			InGameConsole::pushString("Snyggt byggt, frasig Bedroom");
			m_inn->Deposit(150);
			q.Pop();
		}
	}
}

void TutorialEvent::_BuildBedRoomBed()
{
	InGameConsole::RemoveStringEvent();
	InGameConsole::pushStringEvent("Build a Bed : Reward $100");
	std::vector<Room*> rooms = m_roomCtrl->getAllTheRooms();
	for (auto room : rooms)
	{
		std::vector<Furniture*> fur = room->getAllRoomFurnitures();
		for (auto furniture : fur)
		{
			if (furniture->WhatType() == "Bed")
			{
				InGameConsole::pushString("Snyggt byggt, frasig Bed");
				m_inn->Deposit(100);
				q.Pop();
			}
		}
	}
}

void TutorialEvent::_BuildKitchenRoom()
{
	InGameConsole::RemoveStringEvent();
	InGameConsole::pushStringEvent("Build a Kitchen : Reward $350");
	std::vector<Room*> rooms = m_roomCtrl->getAllTheRooms();
	for (auto room : rooms)
	{
		if (room->getRoomType() == RoomType::kitchen)
		{
			InGameConsole::pushString("Snyggt byggt, frasig Kitchen");
			m_inn->Deposit(350);
			q.Pop();
		}
	}
}

