#include "Customer.h"

Customer::Customer()
{
	int i = 0;
}

Customer::Customer(Race race, int gold)
{
	this->m_race = race;
	this->m_economy = Economy(gold);
	this->SetAction(WalkToInn);
}

Customer::Customer(const Customer& other) : Character(other)
{
	this->m_stats = other.m_stats;
	this->m_economy = other.m_economy;
	this->m_race = other.m_race;
	this->m_stateQueue = other.m_stateQueue;
	this->m_hungry = other.m_hungry;
	this->m_tired = other.m_tired;
	this->m_thirsty = other.m_thirsty;
}

Customer::~Customer()
{
}

Attributes& Customer::GetAttributes()
{
	return this->m_stats;
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
	Action action = GetAction();

	switch (action)
	{
	case WalkAction:
		return "Walking";
	case ThinkingAction:
		return "Thinking";
	case DrinkAction:
		return "Drink";
	case EatAction:
		return "Eat";
	case SleepAction:
		return "Sleep";
	case LeavingInnAction:
		return "Leaving Trolls Inn";
	}

	return "Idle";
}

const char* Customer::GetNextActionStr() const
{
	Action action = GetAction();

	switch (action)
	{
	case WalkAction:
		return "Walking";
	case ThinkingAction:
		return "Thinking";
	case DrinkAction:
		return "Drink";
	case EatAction:
		return "Eat";
	case SleepAction:
		return "Sleep";
	case LeavingInnAction:
		return "Leaving Trolls Inn";
	}

	return "Idle";
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
	this->m_hungry = value;
}

void Customer::SetTired(int value)
{
	this->m_tired = value;
}

void Customer::SetThirsty(int value)
{
	this->m_thirsty = value;
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
