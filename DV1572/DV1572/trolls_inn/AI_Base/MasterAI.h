#pragma once
#include "AISolver.h"
#include "Customer.h"
#include "Staff.h"
#include "CustomerFlowControl.h"
#include "Inn.h"
#include <vector>

class MasterAI
{
private:
	AISolver solver;
	std::vector<Customer*> customers;
	std::vector<Customer*> leavingCustomers;
	std::vector<Staff> staff;
	CustomerFlowControl cFL;
	Inn inn;
	// get day/night cycle to update customer needs

public:
	MasterAI();
	~MasterAI();
	Grid* getGrid();
	void update(Camera* cam);
	void Draw();

	// TEMP
	void spawn();
};