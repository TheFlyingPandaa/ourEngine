#pragma once
#include "Attributes.h"
#include "Economy.h"

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

class Customer
{
private:
	Attributes stats;
	Economy economy;
	Race race;
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
	bool getLeavingInn() const;

	void setHungry(int value);
	void setTired(int value);
	void setThirsty(int value);
	void setLeavingInn(bool isLeaving);

	void move(int x, int y);
};