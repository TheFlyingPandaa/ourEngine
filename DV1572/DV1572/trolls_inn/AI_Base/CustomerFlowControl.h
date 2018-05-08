#pragma once
#include "Attributes.h"
#include "Customer.h"
#include "RandomNumberGenerator.h"

class CustomerFlowControl
{
private:
	RandomNumberGenerator m_rNG;
	// INSERT MESH Henrik
	Mesh m_thinkingMesh;

	// Race models
	Mesh m_elf;
	Mesh m_dwarf;

	// Determine what customers the inn attract
	Customer* _evaluate(Attributes innAttributes);
	Customer* _generateRandomCustomer();


public:
	CustomerFlowControl();
	~CustomerFlowControl();

	Customer* Update(Attributes innAttributes);

};