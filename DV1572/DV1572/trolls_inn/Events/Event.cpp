#include "Event.h"

void Event::exitState()
{
	m_exit = true;
}

Event::Event(Inn * inn, RoomCtrl * roomctrl)
{
	m_inn = inn;
	m_roomCtrl = roomctrl;
}

Event::~Event()
{
}

bool Event::exit() const
{
	return m_exit;
}
