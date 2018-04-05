#include "CustomerFlowControl.h"
#include <iostream>

/*int CustomerFlowControl::randomInt(int lowerBound, int upperBound)
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	int lower_bound = lowerBound;
	int upper_bound = upperBound;
	std::uniform_int_distribution<int> unif(lower_bound, upper_bound);
	std::default_random_engine randomEngine(seed);
	int a_random_int = unif(randomEngine);

	return a_random_int;
}*/

Customer CustomerFlowControl::evaluate()
{
	// update inn attributes
	this->innAttributes.setCreepy(this->rNG.generateRandomNumber(-1, 1));
	this->innAttributes.setDrinkQuality(this->rNG.generateRandomNumber(-1, 1));
	this->innAttributes.setFoodQuality(this->rNG.generateRandomNumber(-1, 1));
	this->innAttributes.setPrices(this->rNG.generateRandomNumber(-1, 1));
	this->innAttributes.setReputation(this->rNG.generateRandomNumber(-1, 1));
	this->innAttributes.setShady(this->rNG.generateRandomNumber(-1, 1));
	this->innAttributes.setStandard(this->rNG.generateRandomNumber(-1, 1));
	// match attributes with customers lookup table

	// send new customers to the MasterAI

	return Customer();
}

Customer CustomerFlowControl::generateCustomer(Race race)
{
	// generate a customer with the desired race and a random amount of gold
	Customer newCustomer(race, this->rNG.generateRandomNumber(50, 150));
	newCustomer.setHungry(this->rNG.generateRandomNumber(0, 10));
	newCustomer.setThirsty(this->rNG.generateRandomNumber(0, 10));
	newCustomer.setTired(this->rNG.generateRandomNumber(0, 10));

	return newCustomer;
}

Customer CustomerFlowControl::generateRandomCustomer()
{
	Customer newCustomer;
	int randomRace = this->rNG.generateRandomNumber(1, 3);

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
	// For test purposes only
	this->innAttributes.setCreepy(this->rNG.generateRandomNumber(-10, 10));
	this->innAttributes.setDrinkQuality(this->rNG.generateRandomNumber(-10, 10));
	this->innAttributes.setFoodQuality(this->rNG.generateRandomNumber(-10, 10));
	this->innAttributes.setPrices(this->rNG.generateRandomNumber(-10, 10));
	this->innAttributes.setReputation(this->rNG.generateRandomNumber(-10, 10));
	this->innAttributes.setShady(this->rNG.generateRandomNumber(-10, 10));
	this->innAttributes.setStandard(this->rNG.generateRandomNumber(-10, 10));
}

CustomerFlowControl::~CustomerFlowControl()
{
}

Customer CustomerFlowControl::update()
{
	Customer nextCustomer;
	// evaluate what customer to spawn
	if (this->rNG.generateRandomNumber(0, 100) > 1)
		nextCustomer = this->evaluate();
	// bonus customer of random race (?)
	else
		nextCustomer = this->generateRandomCustomer();

	return nextCustomer;
}

// For test purposes only
void CustomerFlowControl::print()
{
	std::cout << "Creepy: " << this->innAttributes.getCreepy() << std::endl;
	std::cout << "Drink: " << this->innAttributes.getDrinkQuality() << std::endl;
	std::cout << "Food: " << this->innAttributes.getFoodQuality() << std::endl;
	std::cout << "Price: " << this->innAttributes.getPrices() << std::endl;
	std::cout << "Rep: " << this->innAttributes.getReputation() << std::endl;
	std::cout << "Shady " << this->innAttributes.getShady() << std::endl;
	std::cout << "Standard: " << this->innAttributes.getStandard() << std::endl;
}
