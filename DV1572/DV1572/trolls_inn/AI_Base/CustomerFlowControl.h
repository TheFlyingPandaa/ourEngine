#pragma once
#include "Attributes.h"
#include "Customer.h"

class CustomerFlowControl
{
private:
	Attributes innAttributes;
	Customer nextCustomer;

	// Random int generator
	int randomInt(int lowerBound, int upperBound);

	// Determine what customers the inn attract
	void evaluate();
	Customer generateCustomer(Race race);


public:
	CustomerFlowControl();
	~CustomerFlowControl();

	void update();

};