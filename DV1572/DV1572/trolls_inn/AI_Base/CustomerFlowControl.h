#pragma once
#include "Attributes.h"
#include "Customer.h"
#include "RandomNumberGenerator.h"

class CustomerFlowControl
{
private:
	Attributes innAttributes;
	Customer nextCustomer;
	RandomNumberGenerator rNG;

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