#pragma once
#include "Attributes.h"
#include "../Bobby test/Room/Grid.h"
#include "Economy.h"
#include <iostream>
#include "../Time Management/GameTime.h"
struct innUpdateAttributes
{
	float creepy;
	float drinkQuality;
	float foodQuality;
	float prices;
	float reputation;
	float shady;
	float standard;
};

const int START_MONEY = 500;
const double UPDATE_FREQ = 0.420;

class Inn
{
	friend class AISolver;
private:
	
	Mesh m_m;
	Mesh m_kitchenTile;
	Mesh m_rect;
	Mesh m_door;

	// Inn stuff
	Attributes m_innAttributes;
	innUpdateAttributes m_iUA;
	int m_foodPrice;
	int m_sleepPrice;
	int m_drinkPrice;

	void _addStatsToInn(Attributes type);
	void _checkInnStatUpdate();
	Economy*		m_economy;
	int				m_profit,	m_staffSalary = 420;
	bool			m_staffSalaryApplyed;
	double			m_timer;
public:
	Inn();
	~Inn();


	int getFoodPrice() const;
	int getSleepPrice() const;
	int getDrinkPrice() const;

	Attributes getInnAttributes() const;

	void Update(double deltaTime, TIMEOFDAY TOD);
	int getMoney() const;

	void Deposit(int amount);
	void Withdraw(int amount);

	void changeStaffSalary(int amount);
	

	// Add a customers review to the inn stats
	void customerReview(Attributes review);
	// Add stats to the inn based on the furniture placed
	void furnitureStatAdd(Attributes furnitureStats);

	void Update(Camera* cam);
	void Draw();
};