#include "CustomerFlowControl.h"
#include <iostream>

Customer* CustomerFlowControl::_evaluate(Attributes innAttributes)
{
	// Update inn attributes
	/*auto getPoints = [&](Race atri) 
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
		case Elf:
			attributes = elfAtr;
			break;
		case Dwarf:
			attributes = dwarfAtr;
			break;
		}
		float points = 0;

		points += abs((10 + attributes.GetCreepy()) - (10 + innAttributes.GetCreepy()));
		points += abs((10 + attributes.GetDrinkQuality()) - (10 + innAttributes.GetDrinkQuality()));
		points += abs((10 + attributes.GetFoodQuality()) - (10 + innAttributes.GetFoodQuality()));
		points += abs((10 + attributes.GetPrices()) - (10 + innAttributes.GetPrices()));
		points += abs((10 + attributes.GetReputation()) - (10 + innAttributes.GetReputation()));
		points += abs((10 + attributes.GetShady()) - (10 + innAttributes.GetShady()));
		points += abs((10 + attributes.GetStandard()) - (10 + innAttributes.GetStandard()));

		return points;
	};*/
	Customer* newCustomer = nullptr;
	Race race;
	//race = Elf;
	//int closestMatch = 200;
	// Human is first and dwarf is the last in the enum structure. IMPORTANT
	/*for (int currentRace = Elf; currentRace <= Dwarf; currentRace++)
	{
		Race cr = static_cast<Race>(currentRace);
		int cp = getPoints(cr);

		if (closestMatch > cp)
		{
			race = cr;
			closestMatch = cp;
		}
	}*/
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
	/*case 1:
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
		break;*/
	}

	return newCustomer;
}

CustomerFlowControl::CustomerFlowControl()
{
	// Load all race models
	//box.LoadModel("trolls_inn/Resources/box.obj");
	m_boxBillboard.LoadModel("trolls_inn/resources/SpritesheetSample/floor.obj");
}

CustomerFlowControl::~CustomerFlowControl()
{
}

Customer* CustomerFlowControl::Update(Attributes innAttributes)
{
	Customer* nextCustomer;

	// Evaluate what customer to spawn next
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
	nextCustomer->setModel(&m_boxBillboard);
	nextCustomer->setFloor(0);

	// Set needs for the customer
	nextCustomer->SetHungry(this->m_rNG.GenerateRandomNumber(0, 4));
	nextCustomer->SetTired(this->m_rNG.GenerateRandomNumber(0, 4));
	nextCustomer->SetThirsty(this->m_rNG.GenerateRandomNumber(0, 4));

	return nextCustomer;
}