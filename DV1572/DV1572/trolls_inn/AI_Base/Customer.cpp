#include "Customer.h"
#include "../../InGameConsole.h"
#include "RandomNumberGenerator.h"
#include "../Bobby test/Room/RoomCtrl.h"

//Only do this from time to time, no need to do it every frame
void Customer::searchForFreeFurniture(RoomCtrl* roomCtrl)
{
	if (GetAction() == SleepAction)
	{
		findNearestRoom(roomCtrl, Sleeping); 
	}
	else if (GetAction() == EatAction)
	{
		findNearestRoom(roomCtrl, Eating);
	}
	else if (GetAction() == DrinkAction)
	{
		findNearestRoom(roomCtrl, Drinking); 
	}
}

XMINT2 Customer::findNearestRoom(RoomCtrl* roomCtrl, CustomerState customerNeed)
{
	bool furnitureFound = false;
	std::vector<Room*> roomsOfNeededType; 

	if (customerNeed == Sleeping)
		roomsOfNeededType = roomCtrl->getAllRoomsOfType(bedroom);
	else if (customerNeed == Eating)
		roomsOfNeededType = roomCtrl->getAllRoomsOfType(kitchen);
	else
		roomsOfNeededType = roomCtrl->getAllRoomsOfType(bar); 

	std::vector<Furniture*> furniture; 
	XMINT2 targetPos; 
	targetPos = XMINT2(-1, -1); 
	if (m_ownedFurniture == nullptr)
	{
		for (int i = 0; i < roomsOfNeededType.size() && !furnitureFound; i++)
		{
			furniture = roomsOfNeededType[i]->getAllRoomFurnitures();

			if (furniture.size() > 0)
			{
				for (int k = 0; k < furniture.size() && !furnitureFound; k++)
				{
					int dirtyStat = furniture[k]->getDirtyStat();
					if (dirtyStat)
					{
						furniture[k]->getObject3D().setColor(float(dirtyStat) / 10.0f, 0.0f, 0.0f);
					}
					if (!furniture[k]->getIsBusy() &&
						(furniture[k]->WhatType() == "Bed" ||
							furniture[k]->WhatType() == "Table" ||
							furniture[k]->WhatType() == "Bar") && (
								dirtyStat < 10)
						)
					{
						targetPos = XMINT2(furniture[k]->getPosition().x, furniture[k]->getPosition().z);
						furniture[k]->setOwner(this);
						setOwnedFurniture(furniture[k]);
						m_ownedFurniture->setIsBusy(true);
						furniture[k]->increaseDirtyLevel();
						furnitureFound = true;
					}
				}
			}
		}
	}
	
	
	return targetPos; 
}

Customer::Customer()
{
	m_ownedFurniture = nullptr; 
	
}

Customer::Customer(Race race, int gold)
{
	this->m_start = this->m_clock.now();

	m_availableSpotFound = false;
	m_race = race;
	m_economy = Economy(gold);

	m_ownedFurniture = nullptr; 

	if (race == Elf)
	{
		m_hungryRate = 0.6f;
		m_tiredRate = 0.1f;
		m_thirstyRate = 0.4f;
		m_patience = 0.5f;
	}
	else
	{
		m_hungryRate = 0.4f;
		m_tiredRate = 0.3f;
		m_thirstyRate = 0.8f;
		m_patience = 3.0f;
	}
	SetAction(WalkToInn);
}

Customer::Customer(const Customer& other) : Character(other)
{
	m_review = other.m_review;
	m_economy = other.m_economy;
	m_race = other.m_race;
	m_stateQueue = other.m_stateQueue;
	m_availableSpotFound = other.m_availableSpotFound;
	m_hungry = other.m_hungry;
	m_tired = other.m_tired;
	m_thirsty = other.m_thirsty;
	m_hungryRate = other.m_hungryRate;
	m_tiredRate = other.m_tiredRate;
	m_thirstyRate = other.m_thirstyRate;
	m_patience = other.m_patience;
	m_ownedFurniture = nullptr; 
}

Customer::~Customer()
{
}

void Customer::releaseFurniture()
{
	if (m_ownedFurniture != nullptr)
	{
		m_ownedFurniture->releaseOwnerShip();
		m_ownedFurniture->setIsBusy(false);
		m_ownedFurniture = nullptr;
	}
}

Attributes& Customer::GetAttributes()
{
	return m_review;
}

Economy& Customer::GetEconomy()
{
	return m_economy;
}

Race Customer::GetRace() const
{
	return m_race;
}

XMINT2 Customer::getOwnerFurniturePosition() const
{
	if (m_ownedFurniture == nullptr)
		return XMINT2(-1, -1);
	return XMINT2(m_ownedFurniture->getPosition().x, m_ownedFurniture->getPosition().z);
}

const char* Customer::GetRaceStr() const
{
	switch (m_race)
	{
	case Elf:
		return "Elf";
	case Dwarf:
		return "Dwarf";
	}

	return "NO RACE!";
}

Action Customer::GetAction() const
{
	Action action;
	//std namespace max breaks code
	float value = max(max(this->m_tired, this->m_hungry), this->m_thirsty);
	action = (value == this->m_tired) ? SleepAction : (value == this->m_hungry) ? EatAction : DrinkAction;
	
	return action;
}

int Customer::GetQueueEmpty() const
{
	return m_stateQueue.empty();
}

CustomerState Customer::GetState() const
{
	return m_stateQueue.front();
}

CustomerState Customer::GetWaitingToDoState() const
{
	return m_waitingToDoState;
}

void Customer::PopToNextState()
{
	m_stateQueue.pop();
}

void Customer::PopStateQueue()
{
	while (0 < m_stateQueue.size())
		m_stateQueue.pop();
}

void Customer::setOwnedFurniture(Furniture * furnitureOwned)
{
	m_ownedFurniture = furnitureOwned; 
}

void Customer::SetAction(Action nextAction)
{
	switch (nextAction)
	{
	case WalkAction:
		this->m_stateQueue.push(Walking);
		break;
	case ThinkingAction:
		this->m_stateQueue.push(Thinking);
		break;
	case DrinkAction:
		this->m_stateQueue.push(Drinking);
		RestartClock();
		setThoughtBubble(THIRSTY);
		break;
	case EatAction:
		this->m_stateQueue.push(Eating);
		RestartClock();
		setThoughtBubble(HUNGRY);
		break;
	case SleepAction:
		this->m_stateQueue.push(Sleeping);
		RestartClock();
		setThoughtBubble(TIRED);
		break;
	case LeavingInnAction:
		this->m_stateQueue.push(LeavingInn);
		break;
	case WalkToInn:
		this->m_stateQueue.push(WalkingToInn);
		break;
	}

	// To return the customer to idle after it executed its action
	this->m_stateQueue.push(Idle);
}

void Customer::SetWaiting()
{
	CustomerState saveState = m_stateQueue.front();
	m_stateQueue.pop();
	m_stateQueue.pop();
	m_stateQueue.push(Waiting);
	m_stateQueue.push(saveState);
	m_stateQueue.push(Idle);
	m_waitingToDoState = saveState;
}

void Customer::GotPathSetNextAction(Action nextAction, RoomCtrl* roomCtrl)
{
	this->m_stateQueue.push(Walking);
	this->SetAction(nextAction);
}

const char* Customer::GetActionStr() const
{
	Action action = this->GetAction();

	switch (action)
	{
	case WalkAction:
		return "Walking";
	case ThinkingAction:
		return "Thinking";
	case DrinkAction:
		return "Drinking";
	case EatAction:
		return "Eating";
	case SleepAction:
		return "Sleeping";
	case LeavingInnAction:
		return "Leaving Trolls Inn";
	}

	return "Idle";
}

const char * Customer::GetStateStr() const
{
	CustomerState state = this->GetState();

	switch (state)
	{
	case WalkingToInn:
		return "Walking towards Trolls Inn";
	case Walking:
		return "Walking";
	case Thinking:
		return "Thinking";
	case Drinking:
		return "Drinking";
	case Eating:
		return "Eating";
	case Sleeping:
		return "Sleeping";
	case LeavingInn:
		return "Leaving Trolls Inn";
	}

	return "Idle";
}

bool Customer::GetAvailableSpotFound() const
{
	return this->m_availableSpotFound;
}

bool Customer::GetWaitingForSpot() const
{
	return this->m_waitingForSpot;
}

int Customer::GetWaitingForSpotMultiplier() const
{
	return this->m_waitingForSpotMultiplier;
}

void Customer::SetAvailableSpotFound(bool spotFound)
{
	this->m_availableSpotFound = spotFound;
}

void Customer::SetWaitingForSpot(bool waiting)
{
	this->m_waitingForSpot = waiting;
}

void Customer::SetWaitingForSpotMultiplier(int multiplier)
{
	this->m_waitingForSpotMultiplier = multiplier;
}

float Customer::GetHungry() const
{
	return this->m_hungry;
}

float Customer::GetTired() const
{
	return this->m_tired;
}

float Customer::GetThirsty() const
{
	return this->m_thirsty;
}

float Customer::GetHungryRate() const
{
	return m_hungryRate;
}

float Customer::GetTiredRate() const
{
	return m_tiredRate;
}

float Customer::GetThirstyRate() const
{
	return m_thirstyRate;
}

float Customer::GetPatience() const
{
	return m_patience;
}

void Customer::SetHungry(float value)
{
	this->m_hungry = min(max(value, 0), 10);
}

void Customer::SetTired(float value)
{
	this->m_tired = min(max(value, 0), 10);
}

void Customer::SetThirsty(float value)
{
	this->m_thirsty = min(max(value, 0), 10);
}

void Customer::DoEating()
{
	this->m_hungry-= 0.01f;
	if (m_hungry < 0)
		m_hungry = 0;
}

void Customer::DoSleeping()
{
	this->m_tired-= 0.01f;
	if (m_tired < 0)
		m_tired = 0;
}

void Customer::DoDrinking()
{
	this->m_thirsty-= 0.01f;
	if (m_thirsty < 0)
		m_thirsty = 0;
}

std::chrono::duration<double> Customer::GetTimeSpan()
{
	m_now = m_clock.now();
	m_time_span = std::chrono::duration_cast<std::chrono::duration<double>>(m_now - m_start);

	return m_time_span;
}

std::string Customer::getInfoText() const
{
	std::string returnStr;
	returnStr += "Gold " + std::to_string(int(m_economy.GetGold())) + "\n";
	returnStr += "Type ";
	returnStr += GetRaceStr();
	returnStr += "\n";
	returnStr += "Hungry: " + std::to_string(int(m_hungry)) + "\n"; 
	returnStr += "Tired: " + std::to_string(int(m_tired)) + "\n"; 
	returnStr += "Thirsty: " + std::to_string(int(m_thirsty)) + "\n";
	return returnStr;
}

void Customer::RestartClock()
{
	m_start = m_clock.now();
}

void Customer::Update()
{
	if (GetTimeSpan().count() > 5)
		DisableThinkingEmjois();

	Character::Update();
}

void Customer::CheckForPicking()
{
	getHitBox().CheckPick();
}
