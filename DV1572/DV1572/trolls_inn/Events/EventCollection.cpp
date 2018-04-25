#include "EventCollection.h"
#include <iostream>

EventCollection::EventCollection(const int money)
{
	p_reward.setGoldReward(100);

	m_currentCollected = 0;
	m_goalCollection = 100;

	m_totalMoney = money;
	
}

EventCollection::~EventCollection()
{
}

void EventCollection::Update(const int currentMoney)
{
	if (currentMoney > m_totalMoney)
	{
		m_currentCollected += (currentMoney - m_totalMoney);
		m_totalMoney = currentMoney;
	}
	else
	{
		m_totalMoney = currentMoney;
	}
	if (m_currentCollected >= m_goalCollection)
	{
		setExitState(true);
	}
}

int EventCollection::getCurrentCollection() const
{
	return m_currentCollected;
}

int EventCollection::getGoalCollection() const
{
	return m_goalCollection;
}
