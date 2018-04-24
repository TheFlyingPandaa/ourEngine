#pragma once
#include "LevelTracker.h"
#include <queue>
#include <chrono>

enum Professions
{
	Maid,
	Assassin,
	Thief,
	Cook,
	Bartender,
	Guard
};

enum StaffAction
{
	IdleStaff, 
	StaffThinking,
	Cleaning,
	Murdering,
	Stealing,
	Arresting,
	PatrollingWalk,
	Cooking
};

class Staff
{
private:
	LevelTracker m_level;
	Professions m_profession;
	std::queue<StaffAction> m_staffActionQueue; 
	StaffAction m_currentAction; 

	bool m_taskCompleted; 

	std::chrono::high_resolution_clock m_timer; 
	std::chrono::high_resolution_clock::time_point m_timeStart, m_timeNow;
	std::chrono::duration<double, std::ratio<1,1>> m_duration;
	
public:
	Staff();
	Staff(Professions profession);
	Staff(Professions profession, int level);
	~Staff();

	void update(); 

	bool getQueueEmpty(); 
	
	Professions getProfession() const;
	StaffAction getCurrentAction() const; 
	LevelTracker& getLevelTracker();
	std::queue<StaffAction>& getActionQueue();
	bool getTaskCompleted();

	void setCurrentAction(StaffAction staffAction);
	void setTaskCompleted(bool taskCompleted); 
	void resetClock(); 

	std::chrono::duration<double, std::ratio<1, 1>> getDuration();

};