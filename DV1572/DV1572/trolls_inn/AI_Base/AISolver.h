#pragma once
#include "Customer.h"
#include "..\Bobby test\Room\Grid.h"
#include "Staff.h"
#include "StaffController.h"

class AISolver
{
private:
	Grid* map;

public:
	AISolver();
	AISolver(Grid* grid);
	~AISolver();

	void update(Customer& customer);
	void update(Customer& customer, Action desiredAction);
	void update(Staff& staff, std::vector<Customer>& customerVector, StaffController& staffController);
	void update(Staff& staff); 
	// Get Path function (?)

	// Add update for Inn
};