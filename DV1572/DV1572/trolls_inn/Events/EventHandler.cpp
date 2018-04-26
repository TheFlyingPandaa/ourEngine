#include "EventHandler.h"
#include <iostream>

EventHandler::EventHandler(Inn * inn, RoomCtrl * roomctl)
{
	this->m_inn = inn;
	this->m_roomCtrl = roomctl;
}

EventHandler::~EventHandler()
{
	
}

void EventHandler::Update()
{
	if (!m_currentCollectEvents.empty())
	{
		if (m_currentCollectEvents.top().getEventType() == 0)
		{
			m_currentCollectEvents.top().Update(m_inn->getMoney());
		}
		else if(m_currentCollectEvents.top().getEventType() == 1)
		{
			int amountOfObjects = 0;
			for (size_t i = 0; i < m_roomCtrl->getAllTheRooms().size(); i++)
			{
				amountOfObjects += m_roomCtrl->getAllTheRooms().at(i)->getAmountOfObjects();
			}
			m_currentCollectEvents.top().Update(amountOfObjects);
		}
	}
	if (!m_currentCollectEvents.empty())
	{
		if (m_currentCollectEvents.top().exitState())
		{
			Reward wop = m_currentCollectEvents.top().getReward();
			m_inn->Deposit(wop.getGoldReward());
			m_currentCollectEvents.pop();
			std::cout << "EventEnded" << std::endl;
		}
	}
}

void EventHandler::StartCollectEvent()
{
	//TODO: Spawn a mailman or quest giver
	int amountOfObjects = 0;
	for (size_t i = 0; i < m_roomCtrl->getAllTheRooms().size(); i++)
	{
		amountOfObjects += m_roomCtrl->getAllTheRooms().at(i)->getAmountOfObjects();
	}
	m_currentCollectEvents.push(EventCollection(1, amountOfObjects));
}

void EventHandler::EndEvent()
{
	if (!m_currentCollectEvents.empty())
	{
		
			m_inn->getMoney();
			Reward wop = m_currentCollectEvents.top().getReward();
			m_inn->Deposit(wop.getGoldReward());
			m_inn->getMoney();
			m_currentCollectEvents.pop();
			std::cout << "EventEnded" << std::endl;
		
	}
}
