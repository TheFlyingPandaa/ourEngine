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
	WalkAction,
	ThinkingAction,
	DrinkAction,
	EatAction,
	SleepAction,
	LeavingInnAction
};

enum CustomerState
{
	Idle,
	Thinking,
	Walking,
	Drinking,
	Eating,
	Sleeping,
	LeavingInn
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

public:
	Customer();
	Customer(Race race, int gold);
	~Customer();

	Attributes& getAttributes();
	Economy& getEconomy();
	Race getRace() const;

	const char* getRaceStr() const;

	Action getAction() const;
	void getNextAction();
	void setAction(Action nextAction);

	const char* getActionStr() const;

	int getPosX() const;
	int getPosY() const;

	void setPosX(int x);
	void setPosY(int y);

	int getHungry() const;
	int getTired() const;
	int getThirsty() const;

	void setHungry(int value);
	void setTired(int value);
	void setThirsty(int value);

	void move(int x, int y);
};