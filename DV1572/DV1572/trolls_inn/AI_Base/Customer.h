#pragma once
#include "Attributes.h"
#include "../Bobby test/Character.h"
#include "Economy.h"
#include <queue>
//#include <chrono>

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
	LeavingInnAction,
	WalkToInn
};

enum CustomerState
{
	Idle,
	Thinking,
	Walking,
	Drinking,
	Eating,
	Sleeping,
	LeavingInn,
	WalkingToInn

};

class Customer : public Character
{
private:
	Attributes m_stats;
	Economy m_economy;
	Race m_race;
	std::queue<CustomerState> m_stateQueue;

	int m_hungry;
	int m_tired;
	int m_thirsty;

public:
	Customer();
	Customer(Race race, int gold);
	Customer(const Customer &other);
	~Customer();

	Attributes& GetAttributes();
	Economy& GetEconomy();
	Race GetRace() const;

	const char* GetRaceStr() const;

	// Get desired action
	Action GetAction() const;
	void SetAction(Action nextAction);
	void GotPathSetNextAction(Action nextAction);

	int GetQueueEmpty() const;
	CustomerState GetState() const;
	void PopToNextState();

	const char* GetActionStr() const;
	const char* GetStateStr() const;

	int GetHungry() const;
	int GetTired() const;
	int GetThirsty() const;

	void SetHungry(int value);
	void SetTired(int value);
	void SetThirsty(int value);

	// Reduces hungry by 1
	void DoEating();
	// Reduces tired by 1
	void DoSleeping();
	// Reduces thirsty by 1
	void DoDrinking();

	void Update();
};