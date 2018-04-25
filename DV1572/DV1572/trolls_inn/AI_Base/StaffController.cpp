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

void StaffController::update(float dt)
{
	for (auto& member : m_staffMembers)
	{
		if (member->getCurrentProfession() == Maid)
		{
			//Find dirty rooms and clean them.
		}
		else if (member->getCurrentProfession() == Theif)
		{
			//Find customer with alot of gold and steal
		}
		else if (member->getCurrentProfession() == Assasin)
		{
			//Murder special victims or regular customers 
			//to take their gold. 
		}
		else if (member->getCurrentProfession() == Guard)
		{
			//Look for outlaws or mischief.
		}
		else if (member->getCurrentProfession() == Chef)
		{
			//Cook food and and stuff
		}
		else if (member->getCurrentProfession() == Bartender)
		{
			//Make Drinks. 
		}

		//Check if activity is finnished, if so, check what the priorityQueue 
		//says that the current staff member should be assigned to do. 
		//If not, continue current task. 
	}
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


