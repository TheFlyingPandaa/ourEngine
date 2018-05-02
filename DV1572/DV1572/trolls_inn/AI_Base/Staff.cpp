#include "Staff.h"

Staff::Staff()
{
	m_level.setLevel(1); 
	m_currentProfession = Unassigned; 
	m_timeStart = m_timer.now();
}

Staff::Staff(int startLevel)
{
	m_level.setLevel(startLevel);
	m_currentProfession = Unassigned;
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

StaffProfession Staff::getCurrentProfession() const
{
	return 	m_currentProfession;
}

LevelTracker& Staff::getLevelTracker()
{
	return m_level;
}

void Staff::setCurrentProfession(StaffProfession staffProfession)
{
	m_currentProfession = staffProfession; 
}

void Staff::setTaskCompleted(bool taskCompleted)
{
	m_taskCompleted = taskCompleted; 
}

void Staff::resetClock()
{
	m_timeStart = m_timer.now(); 
}

std::queue<StaffProfession>& Staff::getActionQueue()
{
	return m_staffActionQueue; 
}

bool Staff::getTaskCompleted()
{
	return m_taskCompleted; 
}

std::chrono::duration<double, std::ratio<1, 1>> Staff::getDuration()
{
	return m_duration;
}


