#pragma once
#include "Attributes.h"
#include "../Bobby test/Character.h"
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

class Customer : public Character
{
private:
	Attributes stats;
	Economy economy;
	Race race;
	std::queue<CustomerState> stateQueue;

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

	// Get desired action
	Action getAction() const;
	void setAction(Action nextAction);
	void gotPathSetNextAction(Action nextAction);

	int getQueueEmpty() const;
	CustomerState getState() const;
	void popToNextState();

	const char* getActionStr() const;
	const char* getNextActionStr() const;

	int getHungry() const;
	int getTired() const;
	int getThirsty() const;

	void setHungry(int value);
	void setTired(int value);
	void setThirsty(int value);

	// Reduces hungry by 1
	void eating();
	// Reduces tired by 1
	void sleeping();
	// Reduces thirsty by 1
	void drinking();
};