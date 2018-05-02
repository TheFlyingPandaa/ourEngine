#include "StaffController.h"

StaffController::StaffController()
{
	m_pushExtraCleaning = false; 
	m_pushExtraCooking = false; 
	m_pushExtraDrinkMaking = false; 
	m_pushExtraGuarding = false; 
	m_pushExtraMureder = false; 
	m_pushExtraStealing = false;  

	m_cleanPriority = PriorityObject("Clean", 0); 
	m_murderPriority = PriorityObject("Murder", 0); 
	m_stealPriority = PriorityObject("Steal", 0); 
	m_guardPriority = PriorityObject("Guard", 0); 
	m_cookPriority = PriorityObject("Cook", 0); 
    m_makeDrinkPriority = PriorityObject("Make Drink", 0);

	m_staffMembers.reserve(10); 
	
	m_concentrationQueue.push(m_cleanPriority); 
	m_concentrationQueue.push(m_murderPriority);
	m_concentrationQueue.push(m_stealPriority);
	m_concentrationQueue.push(m_guardPriority);
	m_concentrationQueue.push(m_cookPriority);
	m_concentrationQueue.push(m_makeDrinkPriority);
}

StaffController::~StaffController()
{
}

void StaffController::addStaff()
{
	m_staffMembers.push_back(&Staff()); 
}

void StaffController::addStaff(int startLevel)
{
	m_staffMembers.push_back(&Staff(startLevel)); 
}

std::vector<Staff*> StaffController::getStaffMembers()
{
	return m_staffMembers; 
}

void StaffController::updateConcentration(PriorityObject& staffAction, int priorityPointsToAdd)
{
	bool replaceActionFound = false; 
	staffAction.priority = priorityPointsToAdd;
	
	for (int i = 0; m_concentrationQueue.size() && !replaceActionFound; i++)
	{
		if (m_concentrationQueue.top().task == staffAction.task)
		{
			m_concentrationQueue.pop(); 
			replaceActionFound = true; 
		}
	}
	m_concentrationQueue.push(staffAction);
}

StaffController::PriorityObject::PriorityObject(std::string task, int priority)
{
	this->task = task; 
	this->priority = priority; 
}

StaffController::PriorityObject::~PriorityObject()
{
}

bool StaffController::PriorityObject::operator>(const PriorityObject & other)
{
	return this->priority > other.priority; 
}


