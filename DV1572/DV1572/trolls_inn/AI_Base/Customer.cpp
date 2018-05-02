#include "Customer.h"

Customer::Customer()
{
	int i = 0;
}

Customer::Customer(Race race, int gold)
{
	this->m_availableSpotFound = false;
	//this->m_movingTowardsActionArea = false;
	this->m_race = race;
	this->m_economy = Economy(gold);
	this->SetAction(WalkToInn);
	this->m_review.SetReputation(-3); // TEST
	this->m_review.SetStandard(-5); // TEST
	this->m_review.SetDrinkQuality(2); // TEST
	this->m_review.SetFoodQuality(1); // TEST
	this->m_review.SetShady(8); // TEST
}

Customer::Customer(const Customer& other) : Character(other)
{
	this->m_review = other.m_review;
	this->m_economy = other.m_economy;
	this->m_race = other.m_race;
	this->m_stateQueue = other.m_stateQueue;
	this->m_availableSpotFound = other.m_availableSpotFound;
	//this->m_movingTowardsActionArea = other.m_movingTowardsActionArea;
	this->m_hungry = other.m_hungry;
	this->m_tired = other.m_tired;
	this->m_thirsty = other.m_thirsty;
}

Customer::~Customer()
{
}

Attributes& Customer::GetAttributes()
{
	return this->m_review;
}

Economy& Customer::GetEconomy()
{
	return this->m_economy;
}

Race Customer::GetRace() const
{
	return this->m_race;
}

const char* Customer::GetRaceStr() const
{
	switch (m_race)
	{
	case Human:
		return "Human";
	case Troll:
		return "Troll";
	case Orc:
		return "Orc";
	case Dwarf:
		return "Dwarf";
	}

	return "NO RACE!";
}

Action Customer::GetAction() const
{
	Action action;
	//std namespace max breaks code
	int value = max(max(this->m_tired, this->m_hungry), this->m_thirsty);
	action = (value == this->m_tired) ? SleepAction : (value == this->m_hungry) ? EatAction : DrinkAction;
	
	return action;
}

int Customer::GetQueueEmpty() const
{
	return this->m_stateQueue.empty();
}

CustomerState Customer::GetState() const
{
	return this->m_stateQueue.front();
}

void Customer::PopToNextState()
{
	this->m_stateQueue.pop();
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
		break;
	case EatAction:
		this->m_stateQueue.push(Eating);
		break;
	case SleepAction:
		this->m_stateQueue.push(Sleeping);
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

void Customer::GotPathSetNextAction(Action nextAction)
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

//bool Customer::GetMovingTowardsActionArea() const
//{
//	return this->m_movingTowardsActionArea;
//}

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

//void Customer::SetMovingTowardsActionArea(bool moving)
//{
//	this->m_movingTowardsActionArea = moving;
//}

void Customer::SetWaitingForSpot(bool waiting)
{
	this->m_waitingForSpot = waiting;
}

void Customer::SetWaitingForSpotMultiplier(int multiplier)
{
	this->m_waitingForSpotMultiplier = multiplier;
}

int Customer::GetHungry() const
{
	return this->m_hungry;
}

int Customer::GetTired() const
{
	return this->m_tired;
}

int Customer::GetThirsty() const
{
	return this->m_thirsty;
}

void Customer::SetHungry(int value)
{
	//this->m_hungry = value;
	this->m_hungry = min(max(value, 0), 10);
}

void Customer::SetTired(int value)
{
	//this->m_tired = value;
	this->m_tired = min(max(value, 0), 10);
}

void Customer::SetThirsty(int value)
{
	//this->m_thirsty = value;
	this->m_thirsty = min(max(value, 0), 10);
}

void Customer::DoEating()
{
	this->m_hungry--;
}

void Customer::DoSleeping()
{
	this->m_tired--;
}

void Customer::DoDrinking()
{
	this->m_thirsty--;
}

void Customer::Update()
{
	// Character update
	Character::Update();
}
