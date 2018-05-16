#include "Event.h"

void Event::exitState()
{

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
