#pragma once
#include "Attributes.h"
#include "Customer.h"
#include "RandomNumberGenerator.h"
//#include <random>
//#include <chrono>

class CustomerFlowControl
{
private:
	Attributes innAttributes;
	Customer nextCustomer;
	RandomNumberGenerator rNG;

	// Random int generator
	//int randomInt(int lowerBound, int upperBound);

	// Determine what customers the inn attract
	Customer evaluate();
	Customer generateCustomer(Race race);
	Customer generateRandomCustomer();


public:
	CustomerFlowControl();
	~CustomerFlowControl();

	Customer update();
	void print();

};