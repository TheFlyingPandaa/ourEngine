#include "CustomerFlowControl.h"
#include <iostream>

Customer* CustomerFlowControl::_evaluate(Attributes innAttributes)
{
	// Update inn attributes
	auto getPoints = [&](Race atri) 
	{
		Attributes attributes;
		switch (atri)
		{
		case Human:
			attributes = humanAtr;
			break;
		case Troll:
			attributes = trollAtr;
			break;
		case Orc:
			attributes = orcAtr;
			break;
		case Dwarf:
			attributes = dwarfAtr;
			break;
		}
		int points = 0;
		points += ((10 + attributes.GetCreepy()) > (innAttributes.GetCreepy() + 10)) ? 1 : 0;
		points += ((10 + attributes.GetDrinkQuality()) > (innAttributes.GetDrinkQuality() + 10)) ? 1 : 0;
		points += ((10 + attributes.GetFoodQuality()) > (innAttributes.GetFoodQuality() + 10)) ? 1 : 0;
		points += ((10 + attributes.GetPrices()) > (innAttributes.GetPrices() + 10)) ? 1 : 0;
		points += ((10 + attributes.GetReputation()) > (innAttributes.GetReputation() + 10)) ? 1 : 0;
		points += ((10 + attributes.GetShady()) > (innAttributes.GetShady() + 10)) ? 1 : 0;
		points += ((10 + attributes.GetStandard()) > (innAttributes.GetStandard() + 10)) ? 1 : 0;

		return points;
	};
	Race race;
	race = Orc;
	
	int highestScore = 0;
	// Human is first and dwarf is the last in the enum structure. IMPORTANT
	for (int currentRace = Human; currentRace != Dwarf; currentRace++)
	{
		Race cr = static_cast<Race>(currentRace);
		int cp = getPoints(cr);

		if (highestScore < cp)
		{
			race = cr;
			highestScore = cp;
		}
	}

	return new Customer(race, this->m_rNG.GenerateRandomNumber(50, 150));
}

Customer CustomerFlowControl::_generateCustomer(Race race)
{
	// Generate a customer with the desired race and a random amount of gold
	Customer newCustomer(race, this->m_rNG.GenerateRandomNumber(50, 150));
	newCustomer.SetHungry(this->m_rNG.GenerateRandomNumber(-10, 10));
	newCustomer.SetThirsty(this->m_rNG.GenerateRandomNumber(-10, 10));
	newCustomer.SetTired(this->m_rNG.GenerateRandomNumber(-10, 10));

	return newCustomer;
}

Customer* CustomerFlowControl::_generateRandomCustomer()
{
	Customer* newCustomer;
	int randomRace = this->m_rNG.GenerateRandomNumber(1, 4);

	switch (randomRace)
	{
	case 1:
		newCustomer = new Customer(Troll, this->m_rNG.GenerateRandomNumber(50, 150));
		break;		  
	case 2:			  
		newCustomer = new Customer(Orc, this->m_rNG.GenerateRandomNumber(50, 150));
		break;		  
	case 3:			  
		newCustomer = new Customer(Dwarf, this->m_rNG.GenerateRandomNumber(50, 150));
		break;		  
	default:		  
		newCustomer = new Customer(Human, this->m_rNG.GenerateRandomNumber(50, 150));
		break;
	}

	return newCustomer;
}

CustomerFlowControl::CustomerFlowControl()
{
	box.LoadModel("trolls_inn/Resources/box.obj");
}

CustomerFlowControl::~CustomerFlowControl()
{
}

Customer* CustomerFlowControl::Update(Attributes innAttributes)
{
	Customer* nextCustomer;

	// Evaluate what customer to spawn next
	if (this->m_rNG.GenerateRandomNumber(1, 100) > 1)
		nextCustomer = this->_evaluate(innAttributes);
	else
		nextCustomer = this->_generateRandomCustomer();

	std::cout << "A " << nextCustomer->GetRaceStr()<< " has arrived!" << std::endl;

	// Set this to path entrance
	nextCustomer->setPosition(0 + 0.5f, -3.f + 0.5f);
	nextCustomer->setModel(&box);
	nextCustomer->setFloor(0);

	// Set needs for the customer
	nextCustomer->SetHungry(this->m_rNG.GenerateRandomNumber(0, 4));
	nextCustomer->SetTired(this->m_rNG.GenerateRandomNumber(0, 4));
	nextCustomer->SetThirsty(this->m_rNG.GenerateRandomNumber(0, 4));

	return nextCustomer;
}