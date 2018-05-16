#include "../../../OurEngine.h"
#pragma once

#include "HUD/HUD.h"
#include "SubStates\SubState.h"

class State
{
private:

	virtual void _init() = 0;

protected:
	bool	m_exitState = false;
	State * m_newState = nullptr;
	Camera * m_cam;

	std::stack<Shape *>*	p_pickingEvent;
	std::stack<int>*		p_keyEvents;
	HUD						m_stateHUD;

	std::stack<SubState*>	m_subStates;

public:
	State(std::stack<Shape *>* pickingEvent, std::stack<int>* keyEvent) : m_exitState(false), m_newState(nullptr), p_pickingEvent(nullptr), p_keyEvents(nullptr) {
		this->p_pickingEvent = pickingEvent;
		this->p_keyEvents = keyEvent;
	}
	virtual ~State() {}

	virtual void Update(double deltaTime) = 0;
	virtual void Draw() = 0;
	virtual void DrawHUD() = 0;

	virtual State * NewState() 
	{ 
		State * ret = this->m_newState;
		this->m_newState = nullptr;
		return ret;
	}
	virtual bool Exit() { return this->m_exitState; }

	
};

