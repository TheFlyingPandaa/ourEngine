#include "Staff.h"

Staff::Staff()
{
	m_currentAction = Idle; 
}

Staff::Staff(Professions profession)
{
	m_profession = profession;
	m_currentAction = Idle;
}

Staff::Staff(Professions profession, int level)
{
	m_profession = profession;
	m_level.setLevel(level);
	m_currentAction = Idle;
}

Staff::~Staff()
{
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

std::chrono::high_resolution_clock & Staff::getTimer()
{
	return m_timer; 
}

void Staff::setCurrentAction(StaffAction staffAction)
{
	m_currentAction = staffAction; 
}

void Staff::setDurationOfActivity(std::chrono::time_point <std::chrono::high_resolution_clock> durration)
{
	m_durrationStart = durration; 
}

void Staff::setTimerAndDuration(int duration)
{

	m_timePoint = m_timer.now();
}

std::chrono::time_point <std::chrono::high_resolution_clock> Staff::getDurration()
{
	return m_durrationStart;
}
