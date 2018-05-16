#pragma once
#include "Attributes.h"
#include "../Bobby test/Character.h"
#include "Economy.h"
#include <queue>
#include <stack>
#include <chrono>
//#include "../Bobby test/Room/RoomCtrl.h"
#include <iostream>

class RoomCtrl; 
class Furniture;

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
	WaitingAction,
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
	Waiting,
	WalkingToInn
};

class Customer : public Character
{
private:
	Attributes m_review;
	Economy m_economy;
	Race m_race;
	std::queue<CustomerState> m_stateQueue;
	CustomerState m_waitingToDoState;

	Furniture* m_ownedFurniture; 

	// Add a room stack with all the rooms of a specific type (?)
	// For pathfinding purposes

	// Needs variables
	bool m_availableSpotFound;
	bool m_waitingForSpot;
	int m_waitingForSpotMultiplier;

	void searchForFreeFurniture(RoomCtrl* roomCtrl); 

	// Customer interests update variables
	std::chrono::high_resolution_clock m_clock;
	std::chrono::high_resolution_clock::time_point m_start, m_now;
	std::chrono::duration<double, std::ratio<1, 1>> m_time_span;

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

	void releaseFurniture(); 
	bool findNearestRoom(RoomCtrl* roomCtrl, CustomerState customerNeed);

	Attributes& GetAttributes();
	Economy& GetEconomy();
	Race GetRace() const;

	const char* GetRaceStr() const;

	// Get desired action
	Action GetAction() const;
	void SetAction(Action nextAction);
	void SetWaiting();
	void GotPathSetNextAction(Action nextAction, RoomCtrl* roomCtrl);

	int GetQueueEmpty() const;
	CustomerState GetState() const;
	CustomerState GetWaitingToDoState() const;
	void PopToNextState();
	void PopStateQueue();
	
	void setOwnedFurniture(Furniture* furnitureOwned); 

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

	std::chrono::duration<double> GetTimeSpan();
	std::string getInfoText() const;
	void RestartClock();

	void Update();

	void CheckForPicking();
};