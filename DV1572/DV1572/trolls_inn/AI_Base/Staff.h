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
	Idle, 
	Thinking,
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


	std::chrono::high_resolution_clock m_timer; 
	std::chrono::high_resolution_clock::time_point m_timeStart, m_timeNow;
	std::chrono::duration<double, std::ratio<1,1>> m_duration;
	
public:
	Staff();
	Staff(Professions profession);
	Staff(Professions profession, int level);
	~Staff();

	bool getQueueEmpty(); 
	
	Professions getProfession() const;
	StaffAction getCurrentAction() const;
	StaffAction getCurrentAction() const; 
	LevelTracker& getLevelTracker();
	std::queue<StaffAction>& getActionQueue();
	std::chrono::high_resolution_clock& getTimer(); 

	void setCurrentAction(StaffAction staffAction); 
	void setDurationOfActivity(std::chrono::time_point <std::chrono::high_resolution_clock> durration);

	std::chrono::duration<double, std::ratio<1, 1>> getDuration();


	std::chrono::time_point <std::chrono::high_resolution_clock> getDurration(); 
};