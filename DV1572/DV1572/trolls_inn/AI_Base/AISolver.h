#pragma once
#include "Customer.h"
#include "..\Bobby test\Room\Grid.h"
#include "Staff.h"

class AISolver
{
private:
	Grid * m_grid;
public:
	AISolver(Grid* grid);
	~AISolver();

	void update(Customer& customer);
	void update(Customer& customer, Action desiredAction);
	void update(Staff& staff);
	void update(Staff& staff, Action desiredAction);
	// Get Path function (?)
private:
	bool _findPath();
	// Add update for Inn
};