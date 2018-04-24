#include "Staff.h"

Staff::Staff()
{
	m_currentAction = Idle; 
}

Staff::Staff(Professions profession)
{
	m_profession = profession;
	m_currentAction = Idle;
	m_timeStart = m_timer.now(); 
}

Staff::Staff(Professions profession, int level)
{
	m_profession = profession;
	m_level.setLevel(level);
	m_currentAction = Idle;
	m_timeStart = m_timer.now(); 
}

Staff::~Staff()
{
}

void Staff::update()
{
	m_timeNow = m_timer.now(); 
	m_duration = std::chrono::duration_cast<std::chrono::duration<double>>(m_timeNow - m_timeStart); 
}

bool Staff::getQueueEmpty()
{
	return m_staffActionQueue.empty(); 
}

Professions Staff::getProfession() const
{
	return m_profession;
}

StaffAction Staff::getCurrentAction() const
{
	return m_currentAction; 
}

LevelTracker& Staff::getLevelTracker()
{
	return m_level;
}

std::queue<StaffAction>& Staff::getActionQueue()
{
	return m_staffActionQueue; 
}

void Staff::setCurrentAction(StaffAction staffAction)
{
	m_currentAction = staffAction; 
}

void Staff::resetClock()
{
	m_timeStart = m_timer.now(); 
}

std::chrono::duration<double, std::ratio<1, 1>> Staff::getDuration()
{
	return m_duration;
}


