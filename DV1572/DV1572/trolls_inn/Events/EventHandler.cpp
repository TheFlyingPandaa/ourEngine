#include "EventHandler.h"
#include <iostream>
#include "../Mesh Manager/MeshManager.h"
#include <thread>
#include "TutorialEvent.h"

EventHandler::EventHandler(Inn * inn, RoomCtrl * roomctl, std::vector<std::shared_ptr<Node>> firstPath, std::vector<std::shared_ptr<Node>> secondPath)
{
	this->m_inn = inn;
	this->m_roomCtrl = roomctl;
	this->m_pathToInn = firstPath;
	this->m_pathOutInn = secondPath;

	m_eventStack.push(new TutorialEvent(inn,roomctl));

}

EventHandler::~EventHandler()
{
	while(!m_eventStack.empty())
	{
		delete m_eventStack.top();
		m_eventStack.pop();
	}
}



void EventHandler::Update(double deltaTime)
{
	if (!m_eventStack.empty())
	{
		if (m_eventStack.top()->exit()) {
			delete m_eventStack.top();
			m_eventStack.pop();
		}
		else {
			m_eventStack.top()->Update();
		}
	}
	

}

void EventHandler::StartCollectEvent()
{
	
}

void EventHandler::EndEvent()
{
	
}

void EventHandler::Draw()
{

}
