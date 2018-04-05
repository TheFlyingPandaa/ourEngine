#include "CustomerFlowControl.h"
#include <random>
#include <chrono>

int CustomerFlowControl::randomInt(int lowerBound, int upperBound)
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	int lower_bound = lowerBound;
	int upper_bound = upperBound;
	std::uniform_int_distribution<int> unif(lower_bound, upper_bound);
	std::default_random_engine randomEngine(seed);
	int a_random_int = unif(randomEngine);

	return a_random_int;
}

void CustomerFlowControl::evaluate()
{
	// update inn attributes

	// check inn attributes

	// match attributes with customers lookup table

	// bonus customer of random race (?)

	// send new customers to the MasterAI
}

Customer CustomerFlowControl::generateCustomer(Race race)
{
	// generate a customer with the desired race and a random amount of gold
	Customer newCustomer(race, this->randomInt(50, 150));
	newCustomer.setHungry(this->randomInt(0, 10));
	newCustomer.setThirsty(this->randomInt(0, 10));
	newCustomer.setTired(this->randomInt(0, 10));

	return newCustomer;
}

CustomerFlowControl::CustomerFlowControl()
{
}

CustomerFlowControl::~CustomerFlowControl()
{
}

void CustomerFlowControl::update()
{
}