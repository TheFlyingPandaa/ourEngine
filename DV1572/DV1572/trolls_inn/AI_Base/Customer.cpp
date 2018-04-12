#include "Customer.h"

Customer::Customer()
{
}

Customer::Customer(Race race, int gold)
{
	this->race = race;
	this->economy = Economy(gold);
	this->setAction(WalkAction);
}

Customer::~Customer()
{
}

Attributes& Customer::getAttributes()
{
	return this->stats;
}

Economy& Customer::getEconomy()
{
	return this->economy;
}

Race Customer::getRace() const
{
	return this->race;
}

const char * Customer::getRaceStr() const
{
	switch (race)
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

Action Customer::getAction() const
{
	// tired 1
	// hungry 2
	// thristy 5
	Action action;
	int value = std::max(std::max(tired, hungry), thirsty);
	action = (value == tired) ? SleepAction : (value == hungry) ? EatAction : DrinkAction;
	return action;
}

int Customer::getQueueEmpty() const
{
	return this->stateQueue.empty();
}

CustomerState Customer::getState() const
{
	return this->stateQueue.front();
}

void Customer::popToNextState()
{
	this->stateQueue.pop();
}

void Customer::setAction(Action nextAction)
{
	switch (nextAction)
	{
	case WalkAction:
		this->stateQueue.push(Walking);
		break;
	case ThinkingAction:
		this->stateQueue.push(Thinking);
		break;
	case DrinkAction:
		this->stateQueue.push(Drinking);
		break;
	case EatAction:
		this->stateQueue.push(Eating);
		break;
	case SleepAction:
		this->stateQueue.push(Sleeping);
		break;
	case LeavingInnAction:
		this->stateQueue.push(LeavingInn);
		break;
	}
	// to return the customer to idle after it executed its action
	this->stateQueue.push(Idle);
}

const char * Customer::getActionStr() const
{
	Action action = getAction();
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
	return "Idle"; // return thinking instead (?)
	//return "NO ACTION";
}

int Customer::getPosX() const
{
	return this->posX;
}

int Customer::getPosY() const
{
	return this->posY;
}

void Customer::setPosX(int x)
{
	this->posX = x;
}

void Customer::setPosY(int y)
{
	this->posY = y;
}

int Customer::getHungry() const
{
	return this->hungry;
}

int Customer::getTired() const
{
	return this->tired;
}

int Customer::getThirsty() const
{
	return this->thirsty;
}

void Customer::setHungry(int value)
{
	this->hungry = value;
}

void Customer::setTired(int value)
{
	this->tired = value;
}

void Customer::setThirsty(int value)
{
	this->thirsty = value;
}

void Customer::move(int x, int y)
{
	this->posX += x;
	this->posY += y;
}