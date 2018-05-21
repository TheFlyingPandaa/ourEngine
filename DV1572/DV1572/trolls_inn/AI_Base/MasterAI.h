#pragma once
#include "AISolver.h"
#include "Customer.h"
#include "Staff.h"
#include "CustomerFlowControl.h"
#include "Inn.h"
#include <vector>
#include <chrono>
#include "../../InGameConsole.h"
#include "../Bobby test/StateManager/HUD/ClickMenu.h"

const int UPDATE_FREQUENCY_CUSTOMER_NEEDS = 10;
const int CHECK_CUSTOMER_SPAWN = 5;
const int DWARF_SPAWN_RATIO = 2;
const int ELF_SPAWN_RATIO = 5;
const int CUSTOMER_SUPERFAST_RATIO_LIMIT = 40;//80;
const int CUSTOMER_FASTER_RATIO_LIMIT = 25;//40;
const int CUSTOMER_FAST_RATIO_LIMIT = 15;
const int CUSTOMER_MEDIUM_RATIO_LIMIT = 5;

enum CustomerSpawnRatios
{
	Slow,
	Medium,
	Fast,
	Faster,
	SuperFast
};

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

	Mesh			m_thinkingMesh;
	

	// Customer needs update variables
	std::chrono::high_resolution_clock m_clock;
	std::chrono::high_resolution_clock::time_point m_start, m_now;
	std::chrono::duration<double, std::ratio<1, 1>> m_time_span;

	// Customer spawn variables
	std::chrono::high_resolution_clock::time_point m_customer_start, m_customer_now;
	std::chrono::duration<double, std::ratio<1, 1>> m_customer_spawn_timer;
	bool m_customerSpawned;
	int m_customersSpawned;
	CustomerSpawnRatios m_spawnRatio;
	
	void _sortVectorID(std::vector<int>& ID);
	void _swap(int index1, int index2, std::vector<int>& ID);

	void _generateCustomer();
	void _spawnCustomer();

	


	// Customer selection
	enum CUST_SEL_AC
	{
		KILL,
		STEAL
	} m_selectedCustAction;
	int m_selectedCustomer;
	int m_selectedCustomerID;

	void _trollInnChase();

	void CharacterMenu();

	bool m_showMenu;
	ClickMenu* m_customerMenu;

	struct TROLL_CHASE
	{
		XMFLOAT2 customerpath;
		int charIndex;
		int pathReturn;
	}*currentChase;
	bool _checkValidSelectedCustomer(int index);
	void _killCustomer(int customerIndex);
	// !customer Selection
public:
	MasterAI(RoomCtrl* roomCtrl, Grid* grid, Inn * inn);
	~MasterAI();
	
	Staff* getTroll(); 

	// Move the time point with a specified duration
	// Use to remove the impact pausing the game causes
	std::chrono::high_resolution_clock::time_point SaveTimePoint() const;
	void UpdateCustomerSpawnTimePoint(std::chrono::duration<double, std::ratio<1, 1>> duration);
	void UpdateCustomerNeedsTimePoint(std::chrono::duration<double, std::ratio<1, 1>> duration);

	void Update(Camera* cam);

	void PickCustomers();
	void PickedCustomerShape(Shape* shape);
	void Draw();

	// TEMP
	void spawn();
};