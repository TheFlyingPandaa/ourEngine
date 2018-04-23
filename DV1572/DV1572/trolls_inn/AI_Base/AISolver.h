#pragma once
#include "Customer.h"
#include "..\Bobby test\Room\Grid.h"
#include "Staff.h"
#include "Inn.h"
#include <chrono>

class AISolver
{
private:
	Grid * m_grid;

	// Customer needs update variables
	std::chrono::high_resolution_clock m_clock;
	std::chrono::high_resolution_clock::time_point m_start, m_now;
	std::chrono::duration<double, std::ratio<1, 1>> m_time_span;


public:
	AISolver(Grid* grid);
	~AISolver();

	std::chrono::duration<double> getTimeSpan() const;
	void restartClock();

	void Update(Customer& customer, std::chrono::duration<double> time_span);
	void Update(Customer& customer, Action desiredAction);
	void Update(Staff& staff);
	void Update(Staff& staff, Action desiredAction);
	// Get Path function (?)
};