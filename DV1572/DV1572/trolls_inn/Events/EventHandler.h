#pragma once
#include "EventCollection.h"
#include <stack>
#include "../AI_Base/Inn.h"

class EventHandler
{
private:
	std::stack<EventCollection> m_currentCollectEvents;

	Inn * inn;

public:
	EventHandler(Inn * inn);
	~EventHandler();
	
	void Update();

	void StartCollectEvent();

	void EndEvent();

};
