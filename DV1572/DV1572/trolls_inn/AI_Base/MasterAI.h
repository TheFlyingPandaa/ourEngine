#pragma once
#include "Customer.h"
#include "Staff.h"
#include "InnDummyFile.h"
#include <vector>

class MasterAI
{
private:
	std::vector<Customer> customers;
	std::vector<Customer> leavingCustomers;
	std::vector<Staff> staff;
	InnDummyFile inn;
	// get day/night cycle to update customer needs

public:
	MasterAI();
	~MasterAI();
	void update();
};