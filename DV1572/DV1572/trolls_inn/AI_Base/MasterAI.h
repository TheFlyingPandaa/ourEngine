#pragma once
#include "AISolver.h"
#include "Customer.h"
#include "Staff.h"
#include "CustomerFlowControl.h"
#include "Inn.h"
#include <vector>
#include <chrono>

class MasterAI
{
private:
	Inn m_inn;
	AISolver m_solver;
	std::vector<Customer*> m_customers;
	std::vector<Customer*> m_leavingCustomers;
	std::vector<Staff> m_staff;
	CustomerFlowControl m_cFC;

	// Customer needs update variables
	std::chrono::high_resolution_clock m_clock;
	std::chrono::high_resolution_clock::time_point m_start, m_now;
	std::chrono::duration<double, std::ratio<1, 1>> m_time_span;
	
	void _sortVectorID(std::vector<int>& ID);
	void _swap(int index1, int index2, std::vector<int>& ID);

public:
	MasterAI();
	~MasterAI();
	Grid* GetGrid();
	
	void Update(Camera* cam);
	void Draw();

	// TEMP
	void spawn();
};