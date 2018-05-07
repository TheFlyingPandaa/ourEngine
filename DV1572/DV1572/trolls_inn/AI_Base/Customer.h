#pragma once
#include "Attributes.h"
#include "../Bobby test/Character.h"
#include "Economy.h"
#include <queue>
#include <stack>
#include <chrono>

enum Race
{
	Elf,
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
	bool m_waitingForSpot;
	int m_waitingForSpotMultiplier;

	// Customer interests update variables
	std::chrono::high_resolution_clock m_clock;
	std::chrono::high_resolution_clock::time_point m_start, m_now;
	std::chrono::duration<double, std::ratio<1, 1>> m_time_span;
	bool m_showingInterests;

	float m_hungry;
	float m_tired;
	float m_thirsty;

	// Multipliers
	float m_hungryRate;
	float m_tiredRate;
	float m_thirstyRate;
	float m_patience;

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
	bool GetWaitingForSpot() const;
	int GetWaitingForSpotMultiplier() const;
	void SetAvailableSpotFound(bool spotFound);
	void SetWaitingForSpot(bool waiting);
	void SetWaitingForSpotMultiplier(int multiplier);
	
	float GetHungry() const;
	float GetTired() const;
	float GetThirsty() const;

	// Multiplies
	float GetHungryRate() const;
	float GetTiredRate() const;
	float GetThirstyRate() const;
	float GetPatience() const;

	void SetHungry(float value);
	void SetTired(float value);
	void SetThirsty(float value);

	// Reduces hungry by 1
	void DoEating();
	// Reduces tired by 1
	void DoSleeping();
	// Reduces thirsty by 1
	void DoDrinking();

	std::chrono::duration<double> getTimeSpan();
	void restartClock();
	bool getShowingInterests() const;
	void setShowingInterests(bool showingInterests);

	void Update();
};