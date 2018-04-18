#pragma once
#include "Attributes.h"
#include "Customer.h"
#include "RandomNumberGenerator.h"

class CustomerFlowControl
{
private:
	Customer m_nextCustomer;
	RandomNumberGenerator m_rNG;

	// Race models
	// Box race
	Mesh box;
	// REAL_COOL_RACES_YES_YES_PLOX
	Mesh m_human;
	Mesh m_troll;
	Mesh m_orc;
	Mesh m_dwarf;

	// Determine what customers the inn attract
	Customer* _evaluate(Attributes innAttributes);
	Customer _generateCustomer(Race race);
	Customer* _generateRandomCustomer();


public:
	CustomerFlowControl();
	~CustomerFlowControl();

	Customer* update(Attributes innAttributes);

};