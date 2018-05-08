#pragma once
#include "Attributes.h"
#include "../Bobby test/Room/Grid.h"
#include "Economy.h"
#include <iostream>
#include "../Time Management/GameTime.h"
#include "../Bobby test/StateManager/HUD/HUD.h"

const int START_MONEY = 500;
const double UPDATE_FREQ = 2.0;

class Inn
{
private:
	
	Mesh m_m;
	Mesh m_kitchenTile;
	Mesh m_rect;
	Mesh m_door;

	// Inn stuff
	Attributes m_innAttributes;
	//Attributes m_innUpdateAttributes;
	int m_foodPrice;
	int m_sleepPrice;
	int m_drinkPrice;
	bool m_recievedReview;

	//void _addStatsToInn(Attributes type);
	//void _checkInnStatUpdate();
	Economy*		m_economy;
	int				m_profit,	m_staffSalary = 420;
	int				m_rent;
	bool			m_staffSalaryApplyed;
	double			m_timer;

	Text*			m_text;
	std::vector<Text> m_withdrawText;
	std::vector<Text> m_depositText;
	std::vector<int> m_depositAmount;
	std::vector<int> m_removeVec;

	DirectX::XMINT2 m_receptionPos;	//The way we set this will be changed

	void WithdrawText();
public:
	Inn();
	~Inn();

	DirectX::XMINT2 getReceptionPos();

	int GetFoodPrice() const;
	int GetSleepPrice() const;
	int GetDrinkPrice() const;

	bool GetRecievedReview() const;
	void SetRecievedReviewToFalse();

	Attributes GetInnAttributes() const;

	void Update(double deltaTime, TIMEOFDAY TOD);
	void UpdateMoney();
	int getMoney() const;

	void Deposit(int amount);
	void Withdraw(int amount);
	void GetRefund(int amount);

	void IncreaseRent(int amount);
	void DecreaseRent(int amount);

	void changeStaffSalary(int amount);
	
	Text* GetText();

	// Add a customers review to the inn stats
	void CustomerReview(Attributes review);
	// Add stats to the inn based on the furniture placed
	void FurnitureStatAdd(Attributes furnitureStats);

	void Draw();
};