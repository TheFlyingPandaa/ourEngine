#pragma once
#include "AISolver.h"
#include "Customer.h"
#include "Staff.h"
#include "CustomerFlowControl.h"
#include "Inn.h"
#include <vector>
#include <chrono>
#include "../../InGameConsole.h"

const int UPDATE_FREQUENCY_CUSTOMER_NEEDS = 10;
const int CHECK_CUSTOMER_SPAWN = 5;

class MasterAI
{
private:
	Inn * m_inn;
	AISolver m_solver;
	Customer* m_nextCustomer;
	std::vector<Customer*> m_customers;
	std::vector<Customer*> m_leavingCustomers;
	std::vector<Staff> m_staff;
	Staff* m_InnTroll; 
	CustomerFlowControl m_cFC;
	bool m_customerSpawned;

	Mesh			m_thinkingMesh;

	// Customer needs update variables
	std::chrono::high_resolution_clock m_clock;
	std::chrono::high_resolution_clock::time_point m_start, m_now;
	std::chrono::duration<double, std::ratio<1, 1>> m_time_span;

	// Customer spawn variables
	std::chrono::high_resolution_clock::time_point m_customer_start, m_customer_now;
	std::chrono::duration<double, std::ratio<1, 1>> m_customer_spawn_timer;
	
	void _sortVectorID(std::vector<int>& ID);
	void _swap(int index1, int index2, std::vector<int>& ID);

	void _generateCustomer();

public:
	MasterAI(RoomCtrl* roomCtrl, Grid* grid, Inn * inn);
	~MasterAI();
	
	Staff* getTroll(); 

	void Update(Camera* cam);
	void Draw();

	// TEMP
	void spawn();
};