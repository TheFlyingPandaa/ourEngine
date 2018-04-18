#pragma once
#include "Customer.h"
#include "..\Bobby test\Room\Grid.h"
#include "Staff.h"

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
	void update(Staff& staff);
	void update(Staff& staff, Action desiredAction);
	// Get Path function (?)

	// Add update for Inn
};