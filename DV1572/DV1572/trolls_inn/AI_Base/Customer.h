#pragma once
#include "Attributes.h"
#include "Economy.h"
#include <queue>

enum Race
{
	Human,
	Troll,
	Orc,
	Dwarf
};

enum Action
{
	DrinkAction,
	EatAction,
	SleepAction
};

enum CustomerState
{
	Idle,
	Walking,
	Drinking,
	Eating,
	Sleeping
};

class Customer
{
private:
	Attributes stats;
	Economy economy;
	Race race;
	std::queue<CustomerState> stateQueue;
	int posX, posY;

	int hungry;
	int tired;
	int thirsty;
	bool leavingInn;

public:
	Customer();
	Customer(Race race, int gold);
	~Customer();

	Attributes& getAttributes();
	Economy& getEconomy();
	Race getRace() const;

	const char* getRaceStr() const;

	Action getAction() const;

	const char* getActionStr() const;

	int getPosX() const;
	int getPosY() const;

	void setPosX(int x);
	void setPosY(int y);

	int getHungry() const;
	int getTired() const;
	int getThirsty() const;
	CustomerState getCustomerState() const;
	void getNextCustomerState();
	bool getLeavingInn() const;

	void setHungry(int value);
	void setTired(int value);
	void setThirsty(int value);
	void setCustomerState(CustomerState newState);
	void setLeavingInn(bool isLeaving);

	void move(int x, int y);
};