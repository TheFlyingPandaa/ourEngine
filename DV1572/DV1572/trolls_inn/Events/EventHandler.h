#pragma once
#include "EventCollection.h"
#include <stack>
#include "../AI_Base/Inn.h"

class EventHandler
{
private:
	std::stack<EventCollection> m_currentCollectEvents;
	Inn * m_inn;
	RoomCtrl * m_roomCtrl;

public:
	EventHandler(Inn * inn, RoomCtrl * roomCtrl);
	~EventHandler();
	
	void Update();

	void StartCollectEvent();

	void EndEvent();

};
