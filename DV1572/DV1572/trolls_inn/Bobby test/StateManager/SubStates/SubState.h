#pragma once
#include "../../../../ourEngine/interface/Interface.h"
#include <stack>
#include "../HUD/HUD.h"
class SubState
{
private:
	bool		m_exitState = false;
	SubState*	m_newState = nullptr;
protected:
	std::stack<Shape *>*	p_pickingEvent;
	Camera *				p_camera;
	HUD						p_HUD;

	virtual void _init() = 0;
	virtual void _exit() { m_exitState = true; }

public:
	SubState(Camera * cam, std::stack<Shape *>* pickingEvent) : m_exitState(false), m_newState(nullptr), p_pickingEvent(nullptr), p_camera(nullptr)
	{
		p_camera = cam;
		p_pickingEvent = pickingEvent;
	}
	virtual~SubState(){}

	virtual bool		exitState() { return m_exitState; }
	virtual SubState*	newState() { return m_newState; }
	
	virtual void		Update(double deltaTime) = 0;
	virtual void		Draw() = 0;
	virtual void		DrawHUD() = 0;
};

