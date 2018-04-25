#pragma once
#include "Events.h"

class EventCollection : public Events
{
private:
	int m_currentCollected;
	int m_goalCollection;

	int m_totalMoney;
public:
	EventCollection(const int money = 0);
	~EventCollection();

	void Update(const int currentMoney);

	int getCurrentCollection() const;

	int getGoalCollection() const;
};
