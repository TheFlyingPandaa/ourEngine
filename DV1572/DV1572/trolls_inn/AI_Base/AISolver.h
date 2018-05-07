#pragma once
#include "Customer.h"
#include "..\Bobby test\Room\RoomCtrl.h"
#include "..\Bobby test\Room\Grid.h"
#include "Staff.h"
#include "Inn.h"
#include "RandomNumberGenerator.h"
#include <chrono>

const int UPDATE_FREQUENCY_EAT_DRINK_SLEEP_WAIT = 1;
const int WAITING_FOR_SPOT_TIME_LIMIT = 20;

class AISolver
{
private:
	Grid * m_grid;
	RoomCtrl* m_roomctrl;
	// Customer needs update variables
	std::chrono::high_resolution_clock m_clock;
	std::chrono::high_resolution_clock::time_point m_start, m_now;
	std::chrono::duration<double, std::ratio<1, 1>> m_time_span;
	RandomNumberGenerator m_rNG;
	
	void _checkSpotInRoom(Inn& inn, Customer& customer);
	void _doWaiting(Customer& customer);
	std::vector<std::shared_ptr<Node>> GetPathAndSmokeGrass(XMINT2 startPosition, XMINT2 targetPosition);
public:
	AISolver(RoomCtrl *roomctrl, Grid* grid);
	~AISolver();

	std::chrono::duration<double> getTimeSpan() const;
	void restartClock();

	void Update(Customer& customer, Inn& inn);
	void Update(Customer& customer, Action desiredAction);
	//void Update(Customer& customer, Action desiredAction, int price = 0);
	void Update(Staff& staff);
	void Update(Staff& staff, Action desiredAction);
	// Get Path function (?)

	void GetPath(Character& character, RoomType targetRoom);
};