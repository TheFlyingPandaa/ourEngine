#pragma once
#include "LevelTracker.h"
#include <queue>
#include <chrono>

enum StaffProfession
{
	Unassigned, 
	Maid,
	Assasin,
	Theif,
	Guard,
	Chef,
	Bartender
};

class Staff
{
private:
	LevelTracker m_level;

	std::queue<StaffProfession> m_staffActionQueue; 
	StaffProfession m_currentProfession; 

	bool m_taskCompleted; 

	std::chrono::high_resolution_clock m_timer; 
	std::chrono::high_resolution_clock::time_point m_timeStart, m_timeNow;
	std::chrono::duration<double, std::ratio<1, 1>> m_duration; 
	
public:
	Staff();
	Staff(int startLevel);
	~Staff();

	void update(); 

	bool getQueueEmpty(); 
	
	StaffProfession getCurrentProfession() const; 
	LevelTracker& getLevelTracker();
	bool getTaskCompleted();

	void setCurrentProfession(StaffProfession staffProfession);
	void setTaskCompleted(bool taskCompleted); 
	void resetClock(); 

	std::queue<StaffProfession>& getActionQueue(); 

	std::chrono::duration<double, std::ratio<1, 1>> getDuration();

};