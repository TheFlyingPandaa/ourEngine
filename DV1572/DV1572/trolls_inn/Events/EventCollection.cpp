#include "OurEngine.h"
#include "EventCollection.h"

EventCollection::EventCollection(const int decider,const int goal, const int reward,const int money)
{
	m_EventType = CollectionTypes::FUCKSHIT;
	if (decider == 0)
		m_EventType = CollectionTypes::MoneyCollection;
	else if (decider == 1)
		m_EventType = CollectionTypes::ObjectCollection;

	//TODO: Change to suit a more flexible system
	p_reward.setGoldReward(reward);
	m_currentCollected = 0;
	m_goalCollection = goal;
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

EventCollection::CollectionTypes EventCollection::getEventType() const
{
	return m_EventType;
}
