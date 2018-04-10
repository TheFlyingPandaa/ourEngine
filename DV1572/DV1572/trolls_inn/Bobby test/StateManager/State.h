#pragma once
#include <stack>
#include "../../../ourEngine/interface/Interface.h"
#include "HUD/HUD.h"

class State
{
private:

	virtual void _init() = 0;

protected:
	bool	m_exitState = false;
	State * m_newState = nullptr;
	Camera * m_cam;

	std::stack<Shape *>*	p_pickingEvent;
	std::stack<int>*		p_keyEvents;//TODO do this
	HUD						m_stateHUD;


public:
	State(std::stack<Shape *>* pickingEvent, std::stack<int>* keyEvent) : m_exitState(false), m_newState(nullptr), p_pickingEvent(nullptr), p_keyEvents(nullptr) {
		this->p_pickingEvent = pickingEvent;
		this->p_keyEvents = keyEvent;
	}
	virtual ~State() {}

	virtual void Update(double deltaTime) = 0;
	virtual void Draw() = 0;

	virtual State * NewState() { return this->m_newState; }
	virtual bool Exit() { return this->m_exitState; }

	
};

