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
	Inn m_inn;
	AISolver m_solver;
	std::vector<Customer*> m_customers;
	std::vector<Customer*> m_leavingCustomers;
	std::vector<Staff> m_staff;
	CustomerFlowControl m_cFC;
	// get day/night cycle to update customer needs

public:
	MasterAI();
	~MasterAI();
	Grid* GetGrid();
	
	void Update(Camera* cam);
	void Draw();

	// TEMP
	void spawn();
};