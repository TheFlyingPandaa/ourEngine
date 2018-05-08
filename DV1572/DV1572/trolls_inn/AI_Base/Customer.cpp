#include "Customer.h"

Customer::Customer()
{
}

Customer::Customer(Race race, int gold)
{
	this->m_start = this->m_clock.now();

	m_availableSpotFound = false;
	m_race = race;
	m_economy = Economy(gold);
	m_review.SetStat(0.1);

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
}

Customer::~Customer()
{
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

void Customer::PopToNextState()
{
	m_stateQueue.pop();
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

	//POPUP HERE (Henrik)
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
	this->m_hungry--;
	if (m_hungry < 0)
		m_hungry = 0;
}

void Customer::DoSleeping()
{
	this->m_tired--;
	if (m_tired < 0)
		m_tired = 0;
}

void Customer::DoDrinking()
{
	this->m_thirsty--;
	if (m_thirsty < 0)
		m_thirsty = 0;
}

std::chrono::duration<double> Customer::GetTimeSpan()
{
	m_now = m_clock.now();
	m_time_span = std::chrono::duration_cast<std::chrono::duration<double>>(m_now - m_start);

	return m_time_span;
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
