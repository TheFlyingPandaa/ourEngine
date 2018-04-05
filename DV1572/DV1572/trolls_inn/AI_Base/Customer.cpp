#include "Customer.h"

Customer::Customer()
{
}

Customer::Customer(Race race, int gold)
{
	this->race = race;
	this->economy = Economy(gold);
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

const char * Customer::getActionStr() const
{
	Action action = getAction();
	switch (action)
	{
	case DrinkAction:
		return "Drink";
	case EatAction:
		return "Eat";
	case SleepAction:
		return "Sleep";
	}
	return "NO ACTION";
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