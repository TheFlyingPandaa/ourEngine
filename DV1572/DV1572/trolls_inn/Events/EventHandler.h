#pragma once
#include "EventCollection.h"
#include <stack>
#include "../AI_Base/Inn.h"

class EventHandler
{
private:
	//Events
	std::stack<EventCollection> m_currentCollectEvents;

	//Info to the events
	Inn * m_inn;
	RoomCtrl * m_roomCtrl;

	//MailMan related
	bool m_eventStart = false;
	std::vector<std::shared_ptr<Node>> m_pathToInn;
	std::vector<std::shared_ptr<Node>> m_pathOutInn;
	Character m_mailMan;

	//Private Functions
	void _queueMailManPath(const bool inOrOut);
public:
	EventHandler(Inn * inn, RoomCtrl * roomCtrl, std::vector<std::shared_ptr<Node>> firstPath, std::vector<std::shared_ptr<Node>> secondPath);
	~EventHandler();
	
	void Update();
	void StartCollectEvent();	//Starts a collection event
	void EndEvent();	//only for debug right now

	void Draw();
};
