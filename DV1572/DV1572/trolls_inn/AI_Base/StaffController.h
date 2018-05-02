#pragma once
#include "Staff.h"
#include <queue>

class StaffController
{
private:
	
	float m_chanceOfCleaning; 
	float m_chanceOfStealing; 
	float m_chanceOfMurdering; 
	float m_chanceOfGuarding;
	float m_chanceOfCooking; 
	float m_chanceOfMakingDrink; 

	class PriorityObject
	{
	public:
		int priority;
		std::string task;

		PriorityObject(std::string task = "None", int  priority = 0);
		~PriorityObject();

		bool operator>(const PriorityObject& other);
	};

	std::priority_queue<PriorityObject> m_concentrationQueue;

	PriorityObject m_cleanPriority;
	PriorityObject m_murderPriority;
	PriorityObject m_stealPriority;
	PriorityObject m_guardPriority;
	PriorityObject m_cookPriority;
	PriorityObject m_makeDrinkPriority;

	std::vector<Staff*> m_staffMembers; 
public:
	StaffController(); 
	~StaffController(); 

	void addStaff();
	void addStaff(int startLevel); 

	std::vector<Staff*> getStaffMembers();

	void updateConcentration(PriorityObject& staffAction, int priorityPointsToAdd); 
};