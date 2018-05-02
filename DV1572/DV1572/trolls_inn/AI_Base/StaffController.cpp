#include "StaffController.h"

StaffController::StaffController()
{
	float m_chanceOfCleaning = 0.0f;
	float m_chanceOfStealing = 0.0f;
	float m_chanceOfMurdering = 0.0f;
	float m_chanceOfGuarding = 0.0f;
	float m_chanceOfCooking = 0.0f;
	float m_chanceOfMakingDrink = 0.0f;

	m_cleanPriority = PriorityObject("Clean", 7); 
	m_murderPriority = PriorityObject("Murder", 6); 
	m_stealPriority = PriorityObject("Steal", 5); 
	m_guardPriority = PriorityObject("Guard", 4); 
	m_cookPriority = PriorityObject("Cook", 3); 
    m_makeDrinkPriority = PriorityObject("Make Drink", 2);
	m_doNothing = PriorityObject("Do Nothing", 1); 

	m_staffMembers.reserve(10); 
	
	m_concentrationQueue.push(m_cleanPriority); 
	m_concentrationQueue.push(m_murderPriority);
	m_concentrationQueue.push(m_stealPriority);
	m_concentrationQueue.push(m_guardPriority);
	m_concentrationQueue.push(m_cookPriority);
	m_concentrationQueue.push(m_makeDrinkPriority);
	m_concentrationQueue.push(m_doNothing); 

	addStaff(); 
}

StaffController::~StaffController()
{
}

void StaffController::addStaff()
{
	m_staffMembers.push_back(&Staff()); 
	m_staffMembers.at(m_staffMembers.size())->setCurrentProfession(Unassigned); 
}

void StaffController::addStaff(int startLevel)
{
	m_staffMembers.push_back(&Staff(startLevel)); 
}

std::vector<Staff*> StaffController::getStaffMembers()
{
	return m_staffMembers; 
}

void StaffController::updateConcentration(PriorityObject& priorityObject, int priorityPointsToAdd)
{
	bool replaceActionFound = false; 
	priorityObject.priority = priorityPointsToAdd;
	std::vector<PriorityObject> priorityObjectVector; 

	for (int i = 0; i < m_concentrationQueue.size(); i++)
	{
		priorityObjectVector.push_back(m_concentrationQueue.top()); 
		m_concentrationQueue.pop(); 
	}

	for (int i = 0; i < priorityObjectVector.size(); i++)
	{
		if (priorityObjectVector.at(i).task == priorityObject.task)
		{
			priorityObjectVector.at(i).priority = priorityPointsToAdd; 
		}
	}

	for (int i = 0; i < priorityObjectVector.size(); i++)
	{
		m_concentrationQueue.push(priorityObjectVector.at(i)); 
	}
}

void StaffController::giveNewAction(Staff* staffMemeber)
{
	staffMemeber->getActionQueue().push(calculateNewStaffProfession()); 
}

StaffProfession StaffController::calculateNewStaffProfession()
{

	//Take out the priotiyObjects and put them in a vector for management. 
	std::vector<PriorityObject> possibleProfessions; 
	for (int i = 0; i < m_concentrationQueue.size(); i++)
	{
		possibleProfessions.push_back(m_concentrationQueue.top());
		m_concentrationQueue.pop(); 
	}
	StaffProfession professionToReturn; 
	
	//Set the chance based on the priority. 
	for (int i = 0; i < possibleProfessions.size(); i++)
	{
		if (possibleProfessions.at(i).task == "Maid")
		{
			m_chanceOfCleaning = possibleProfessions.at(i).priority * 14.0f; 
		}
		else if (possibleProfessions.at(i).task == "Unassigned")
		{
			m_chanceOfDoingNothing = possibleProfessions.at(i).priority * 14.0f;
		}
		else if (possibleProfessions.at(i).task == "Assasin")
		{
			m_chanceOfMurdering = possibleProfessions.at(i).priority * 14.0f;
		}
		else if (possibleProfessions.at(i).task == "Theif")
		{
			m_chanceOfStealing = possibleProfessions.at(i).priority * 14.0f;
		}
		else if (possibleProfessions.at(i).task == "Guard")
		{
			m_chanceOfGuarding = possibleProfessions.at(i).priority * 14.0f;
		}
		else if (possibleProfessions.at(i).task == "Chef")
		{
			m_chanceOfCooking = possibleProfessions.at(i).priority * 14.0f;
		}
		else
		{
			m_chanceOfMakingDrink = possibleProfessions.at(i).priority * 14.0f;
		}
	}

	//Calculate which task that is to be done based upon priority and chance.
	RandomNumberGenerator rndNumGenerator; 
	int checkNumber = rndNumGenerator.generateRandomNumber(0, 100);
	int checkedRange = 0;

	if (checkNumber <= m_chanceOfCleaning)
	{
		professionToReturn = Maid; 
	}
	//Expand
	checkedRange += m_chanceOfCleaning;

	if (checkNumber > m_chanceOfCleaning && checkNumber <=  (m_chanceOfCooking + checkedRange))
	{
		professionToReturn = Chef; 
	}
	checkedRange += m_chanceOfCooking; 
	
	if (checkNumber > m_chanceOfCooking && checkNumber <= (m_chanceOfDoingNothing + checkedRange))
	{
		professionToReturn = Unassigned; 
	}
	checkedRange += m_chanceOfDoingNothing; 

	if (checkNumber > m_chanceOfDoingNothing && checkNumber <= (m_chanceOfGuarding + checkedRange))
	{
		professionToReturn = Guard; 
	}
	checkedRange += m_chanceOfGuarding; 

	if (checkNumber > m_chanceOfGuarding && checkNumber <= (m_chanceOfMakingDrink + checkedRange))
	{
		professionToReturn = Bartender; 
	}
	checkedRange += m_chanceOfMakingDrink; 

	if (checkNumber > m_chanceOfMakingDrink && checkedRange <= (m_chanceOfMurdering + checkedRange))
	{
		professionToReturn = Assasin; 
	}
	checkedRange += m_chanceOfMakingDrink; 

	if (checkNumber > m_chanceOfMurdering && checkNumber <= (m_chanceOfStealing + checkedRange))
	{
		professionToReturn = Theif; 
	}

	//Refill the concentration queue. 
	for (int i = 0; i < possibleProfessions.size(); i++)
	{
		m_concentrationQueue.push(possibleProfessions.at(i));
	}

	return professionToReturn; 
}

StaffController::PriorityObject::PriorityObject(std::string task, int priority)
{
	this->task = task; 
	this->priority = priority; 
}

StaffController::PriorityObject::~PriorityObject()
{
}

bool StaffController::PriorityObject::operator<(PriorityObject & other)
{
	return priority < other.priority; 
}


