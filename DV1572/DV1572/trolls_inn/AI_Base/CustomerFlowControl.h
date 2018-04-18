#pragma once
#include "Attributes.h"
#include "Customer.h"
#include "RandomNumberGenerator.h"

class CustomerFlowControl
{
private:
	Customer nextCustomer;
	RandomNumberGenerator rNG;

	// Race models
	// Box race
	Mesh box;
	// REAL_COOL_RACES_YES_YES_PLOX
	Mesh human;
	Mesh troll;
	Mesh orc;
	Mesh dwarf;

	// Determine what customers the inn attract
	Customer evaluate(Attributes innAttributes);
	Customer generateCustomer(Race race);
	Customer generateRandomCustomer();


public:
	CustomerFlowControl();
	~CustomerFlowControl();

	Customer update(Attributes innAttributes);

};