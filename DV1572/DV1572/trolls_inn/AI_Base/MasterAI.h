#pragma once
#include "AISolver.h"
#include "Customer.h"
#include "Staff.h"
#include "CustomerFlowControl.h"
#include "Inn.h"
#include <vector>
#include "RandomNumberGenerator.h"
#include "StaffController.h"

class MasterAI
{
private:
	AISolver solver;
	StaffController m_staffController; 
	std::vector<Customer> customers;
	std::vector<Customer> leavingCustomers;
	CustomerFlowControl cFL;
	Inn inn;
	RandomNumberGenerator m_rndNumGen; 
	// get day/night cycle to update customer needs
public:
	MasterAI();
	~MasterAI();
	void update();
};