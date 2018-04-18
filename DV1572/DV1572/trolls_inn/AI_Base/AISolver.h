#pragma once
#include "Customer.h"
#include "..\Bobby test\Room\Grid.h"
#include "Staff.h"

class AISolver
{
private:
	bool _findPath();
	// Add update for Inn

public:
	AISolver();
	~AISolver();

	void Update(Customer& customer);
	void Update(Customer& customer, Action desiredAction);
	void Update(Staff& staff);
	void Update(Staff& staff, Action desiredAction);
	// Get Path function (?)
};