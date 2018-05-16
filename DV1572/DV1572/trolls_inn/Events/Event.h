#pragma once
#include "../../trolls_inn/AI_Base/Inn.h"
#include "../../InGameConsole.h"
class Event
{
private:
	bool m_exit = false;
protected:
	Inn * m_inn;
	RoomCtrl * m_roomCtrl;
	void exitState();

public:
	Event(Inn * inn, RoomCtrl * roomctrl);
	virtual ~Event();

	virtual void Update() = 0;
	virtual void Draw() = 0;

	bool exit() const;
};
