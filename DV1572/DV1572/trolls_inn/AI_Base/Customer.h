#pragma once
#include "Attributes.h"
#include "../Bobby test/Character.h"
#include "Economy.h"
#include <queue>
#include <stack>
#include <chrono>

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
	Attributes m_review;
	Economy m_economy;
	Race m_race;
	std::queue<CustomerState> m_stateQueue;

	// Add a room stack with all the rooms of a specific type (?)
	// For pathfinding purposes

	// Add angry face count (?)

	// Needs variables
	bool m_availableSpotFound;
	//bool m_movingTowardsActionArea;
	bool m_waitingForSpot;
	int m_waitingForSpotMultiplier;

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

	bool GetAvailableSpotFound() const;
	//bool GetMovingTowardsActionArea() const;
	bool GetWaitingForSpot() const;
	int GetWaitingForSpotMultiplier() const;
	void SetAvailableSpotFound(bool spotFound);
	//void SetMovingTowardsActionArea(bool moving);
	void SetWaitingForSpot(bool waiting);
	void SetWaitingForSpotMultiplier(int multiplier);
	
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