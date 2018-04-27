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

	bool m_eventStart = false;
	std::vector<std::shared_ptr<Node>> m_pathToInn;
	std::vector<std::shared_ptr<Node>> m_pathOutInn;

	Character m_mailMan;
public:
	EventHandler(Inn * inn, RoomCtrl * roomCtrl, std::vector<std::shared_ptr<Node>> firstPath, std::vector<std::shared_ptr<Node>> secondPath);
	~EventHandler();
	
	void Update();

	void StartCollectEvent();

	void EndEvent();

	void Draw();
};
