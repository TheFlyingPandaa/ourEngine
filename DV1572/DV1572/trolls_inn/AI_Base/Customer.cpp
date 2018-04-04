#include "Customer.h"

Customer::Customer()
{
}

Customer::~Customer()
{
}

Attributes Customer::getAttributes() const
{
	return this->stats;
}

int Customer::getGold() const
{
	return this->gold;
}

Type Customer::getRace() const
{
	return this->race;
}

int Customer::getPosX() const
{
	return this->posX;
}

int Customer::getPosY() const
{
	return this->posY;
}
