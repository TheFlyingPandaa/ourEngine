#include "CustomerFlowControl.h"
#include <iostream>

Customer* CustomerFlowControl::_evaluate(Attributes innAttributes)
{
	Customer* newCustomer = nullptr;
	int probabilityForCustomerSpawnOfSpecificRace = abs(100 * innAttributes.GetStat());
	int rNGUpperLimit = 100 - probabilityForCustomerSpawnOfSpecificRace;
	
	std::cout << "---------------------------------------------------------\n";
	std::cout << "Probability: " << probabilityForCustomerSpawnOfSpecificRace << std::endl;
	std::cout << "Inn stats: " << innAttributes.GetStat() << std::endl;

	if (rNGUpperLimit == 0)
	{
		newCustomer = _generateCustomerBasedOnInnAttributes(innAttributes);
	}
	else
	{
		//int customerRandomProbabilityPercentage = m_rNG.GenerateRandomNumber(0, rNGUpperLimit);
		int randomNumber = m_rNG.GenerateRandomNumber(1, 100);
		
		//std::cout << "Random percentage: " << customerRandomProbabilityPercentage << std::endl;
		std::cout << "Random number: " << randomNumber << std::endl;
		std::cout << "---------------------------------------------------------\n";

		if (randomNumber > rNGUpperLimit)
		//if (customerRandomProbabilityPercentage <= randomNumber)
			newCustomer = _generateCustomerBasedOnInnAttributes(innAttributes);
		else
			newCustomer = _generateRandomCustomer();
	}

	return newCustomer;
}

Customer* CustomerFlowControl::_generateCustomerBasedOnInnAttributes(Attributes innAttributes)
{
	Customer* newCustomer;
	Race race;

	if (innAttributes.GetStat() > 0.0)
	{
		race = Elf;
		newCustomer = new Customer(race, m_rNG.GenerateRandomNumber(100, 200));
	}
	else
	{
		race = Dwarf;
		newCustomer = new Customer(race, m_rNG.GenerateRandomNumber(50, 100));
	}

	return newCustomer;
}

Customer* CustomerFlowControl::_generateRandomCustomer()
{
	Customer* newCustomer;
	int randomRace = this->m_rNG.GenerateRandomNumber(1, 2);

	switch (randomRace)
	{
	case 1:
		newCustomer = new Customer(Elf, m_rNG.GenerateRandomNumber(50, 150));
		break;
	default:
		newCustomer = new Customer(Dwarf, m_rNG.GenerateRandomNumber(50, 150));
		break;
	}

	return newCustomer;
}

CustomerFlowControl::CustomerFlowControl()
{
	// Load all race models
	m_thinkingMesh.LoadModel("trolls_inn/Resources/Thoughts/Bubble.obj");
	m_elf.LoadModel("trolls_inn/resources/SpritesheetSample/elf.obj");
	m_dwarf.LoadModel("trolls_inn/resources/SpritesheetSample/dwarf.obj");
}

CustomerFlowControl::~CustomerFlowControl()
{
}

Customer* CustomerFlowControl::Update(Attributes innAttributes)
{
	Customer* nextCustomer;

	// Evaluate what customer to spawn next
	// Inn attributes affect what customers that spawn
	if (this->m_rNG.GenerateRandomNumber(1, 100) > 5)
		nextCustomer = this->_evaluate(innAttributes);
	else
		nextCustomer = this->_generateRandomCustomer();

	if (nextCustomer->GetRace() == Elf)
		std::cout << "An " << nextCustomer->GetRaceStr() << " has arrived!" << std::endl;
	else
		std::cout << "A " << nextCustomer->GetRaceStr() << " has arrived!" << std::endl;

	// Set this to path entrance
	nextCustomer->setPosition(0 + 0.5f, -3.f + 0.5f);

	if (nextCustomer->GetRace() == Elf)
		nextCustomer->setModel(&m_elf);
	else
		nextCustomer->setModel(&m_dwarf);

	nextCustomer->setFloor(0);
	nextCustomer->setThoughtBubbleMesh(&m_thinkingMesh);

	// Set needs for the customer
	nextCustomer->SetHungry(this->m_rNG.GenerateRandomNumber(0, 4));
	nextCustomer->SetTired(this->m_rNG.GenerateRandomNumber(0, 4));
	nextCustomer->SetThirsty(this->m_rNG.GenerateRandomNumber(0, 4));

	return nextCustomer;
}