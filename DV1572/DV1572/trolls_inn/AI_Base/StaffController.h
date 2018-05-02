#pragma once
#include "Staff.h"
#include "RandomNumberGenerator.h"
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
	float m_chanceOfDoingNothing; 

	class PriorityObject
	{
	public:
		int priority;
		std::string task;

		PriorityObject(std::string task = "None", int  priority = 0);
		~PriorityObject();

		bool operator<(PriorityObject& other);
		
		friend bool operator<(const PriorityObject& Left, const PriorityObject& Right)
		{
			return Left < Right;
		}
	};

	std::priority_queue<PriorityObject> m_concentrationQueue;

	PriorityObject m_cleanPriority;
	PriorityObject m_murderPriority;
	PriorityObject m_stealPriority;
	PriorityObject m_guardPriority;
	PriorityObject m_cookPriority;
	PriorityObject m_makeDrinkPriority;
	PriorityObject m_doNothing; 

	std::vector<Staff*> m_staffMembers; 
public:
	StaffController(); 
	~StaffController(); 

	void addStaff();
	void addStaff(int startLevel); 

	std::vector<Staff*> getStaffMembers();

	void updateConcentration(PriorityObject& staffAction, int priorityPointsToAdd); 
	void giveNewAction(Staff* staffMember); 
	StaffProfession calculateNewStaffProfession(); 
};