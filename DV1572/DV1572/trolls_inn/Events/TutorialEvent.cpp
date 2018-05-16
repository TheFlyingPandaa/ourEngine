#include "TutorialEvent.h"

TutorialEvent::TutorialEvent(Inn * inn, RoomCtrl * roomctrl) : Event(inn,roomctrl)
{

	/*m_queue.push(&TutorialEvent::_BuildReception);
	m_queue.push(&TutorialEvent::_BuildReceptionFurniture);*/
	q.Push(&TutorialEvent::_BuildReception);
	q.Push(&TutorialEvent::_BuildReceptionFurniture);
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
	InGameConsole::pushStringEvent("Build a reception room");
	std::vector<Room*> rooms = m_roomCtrl->getAllTheRooms();
	for (auto room : rooms)
	{
		if (room->getRoomType() == RoomType::reception)
		{
			InGameConsole::pushString("Snyggt byggt, frasig reception");
			q.Pop();
		}
	}
	
}

void TutorialEvent::_BuildReceptionFurniture()
{
	InGameConsole::RemoveStringEvent();
	InGameConsole::pushStringEvent("Build a reception desk");
	std::vector<Room*> rooms = m_roomCtrl->getAllTheRooms();
	for (auto room : rooms)
	{
		std::vector<Furniture*> fur = room->getAllRoomFurnitures();
		for (auto furniture : fur)
		{
			if (furniture->WhatType() == "Reception")
			{
				InGameConsole::pushString("Snyggt byggt, frasig reception");
				q.Pop();
			}
		}
	}
}

