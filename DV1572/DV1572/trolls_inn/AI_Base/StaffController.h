#pragma once
#include "Staff.h"
#include <queue>

class StaffController
{
private:
	bool m_pushExtraCleaning; 
	bool m_pushExtraMureder; 
	bool m_pushExtraStealing;
	bool m_pushExtraGuarding; 
	bool m_pushExtraCooking; 
	bool m_pushExtraDrinkMaking; 

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

	void updateConcentration(PriorityObject& staffAction, int priorityPointsToAdd); 
	void update(float dt); 
};