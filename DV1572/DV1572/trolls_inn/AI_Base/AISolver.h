#pragma once
#include "Customer.h"
#include "Staff.h"

class AISolver
{
private:

public:
	void update(Customer& customer);
	void update(Customer& customer, Action desiredAction);
	void update(Staff& staff);
	void update(Staff& staff, Action desiredAction);
	// Get Path function (?)
	// Add update for Inn
};