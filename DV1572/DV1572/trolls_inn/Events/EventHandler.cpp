#include "EventHandler.h"
#include <iostream>

EventHandler::EventHandler(Inn * inn)
{
	this->inn = inn;
}

EventHandler::~EventHandler()
{
}

void EventHandler::Update()
{
	if (!m_currentCollectEvents.empty())
	{
		m_currentCollectEvents.top().Update(inn->getMoney());
	}
	if (!m_currentCollectEvents.empty())
	{
		if (m_currentCollectEvents.top().exitState())
		{
			Reward wop = m_currentCollectEvents.top().getReward();
			inn->Deposit(wop.getGoldReward());
			m_currentCollectEvents.pop();
			std::cout << "EventEnded" << std::endl;
		}
	}
}

void EventHandler::StartCollectEvent()
{
	m_currentCollectEvents.push(EventCollection(inn->getMoney()));
}

void EventHandler::EndEvent()
{
	if (!m_currentCollectEvents.empty())
	{
		
			inn->getMoney();
			Reward wop = m_currentCollectEvents.top().getReward();
			inn->Deposit(wop.getGoldReward());
			inn->getMoney();
			m_currentCollectEvents.pop();
			std::cout << "EventEnded" << std::endl;
		
	}
}
