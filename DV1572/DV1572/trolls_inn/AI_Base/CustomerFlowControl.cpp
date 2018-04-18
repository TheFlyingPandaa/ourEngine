#include "CustomerFlowControl.h"
#include <iostream>

Customer CustomerFlowControl::evaluate(Attributes innAttributes)
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
		points += ((10 + attributes.getCreepy()) > (innAttributes.getCreepy() + 10)) ? 1 : 0;
		points += ((10 + attributes.getDrinkQuality()) > (innAttributes.getDrinkQuality() + 10)) ? 1 : 0;
		points += ((10 + attributes.getFoodQuality()) > (innAttributes.getFoodQuality() + 10)) ? 1 : 0;
		points += ((10 + attributes.getPrices()) > (innAttributes.getPrices() + 10)) ? 1 : 0;
		points += ((10 + attributes.getReputation()) > (innAttributes.getReputation() + 10)) ? 1 : 0;
		points += ((10 + attributes.getShady()) > (innAttributes.getShady() + 10)) ? 1 : 0;
		points += ((10 + attributes.getStandard()) > (innAttributes.getStandard() + 10)) ? 1 : 0;

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

	return Customer(race, this->rNG.generateRandomNumber(50, 150));
}

Customer CustomerFlowControl::generateCustomer(Race race)
{
	// Generate a customer with the desired race and a random amount of gold
	Customer newCustomer(race, this->rNG.generateRandomNumber(50, 150));
	newCustomer.setHungry(this->rNG.generateRandomNumber(-10, 10));
	newCustomer.setThirsty(this->rNG.generateRandomNumber(-10, 10));
	newCustomer.setTired(this->rNG.generateRandomNumber(-10, 10));

	return newCustomer;
}

Customer CustomerFlowControl::generateRandomCustomer()
{
	Customer newCustomer;
	int randomRace = this->rNG.generateRandomNumber(1, 4);

	switch (randomRace)
	{
	case 1:
		newCustomer = Customer(Troll, this->rNG.generateRandomNumber(50, 150));
		break;
	case 2:
		newCustomer = Customer(Orc, this->rNG.generateRandomNumber(50, 150));
		break;
	case 3:
		newCustomer = Customer(Dwarf, this->rNG.generateRandomNumber(50, 150));
		break;
	default:
		newCustomer = Customer(Human, this->rNG.generateRandomNumber(50, 150));
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

Customer CustomerFlowControl::update(Attributes innAttributes)
{
	Customer nextCustomer;

	// Evaluate what customer to spawn next
	if (this->rNG.generateRandomNumber(1, 100) > 1)
		nextCustomer = this->evaluate(innAttributes);
	else
		nextCustomer = this->generateRandomCustomer();

	std::cout << "A " << nextCustomer.getRaceStr()<< " has arrived!" << std::endl;

	// Set this to path entrance
	nextCustomer.setPosition(7 + 0.5f, 2 + 0.5f);
	nextCustomer.setModel(&box);
	nextCustomer.setFloor(0);

	return nextCustomer;
}