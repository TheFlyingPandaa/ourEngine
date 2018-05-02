#pragma once
#include "Events.h"

class EventCollection : public Events
{
private:
	int m_currentCollected;
	int m_goalCollection;

	int m_totalMoney;//By making it money related it's easier to abstract

	enum CollectionTypes
	{
		FUCKSHIT = -1,
		MoneyCollection = 0,
		ObjectCollection = 1,
	};

	CollectionTypes m_EventType;

public:

	/*
		The first parameter will take a Int
		This Int will decide what type of event it will be
		 MoneyCollection = 0
		 ObjectCollection = 1
	*/
	EventCollection(const int decider ,const int goal = 0, const int reward = 0,const int money = 0);
	~EventCollection();

	void Update(const int currentMoney);

	int getCurrentCollection() const;

	int getGoalCollection() const;

	/*
		MoneyCollection = 0 
		ObjectCollection = 1
	 */
	CollectionTypes getEventType() const;

};
