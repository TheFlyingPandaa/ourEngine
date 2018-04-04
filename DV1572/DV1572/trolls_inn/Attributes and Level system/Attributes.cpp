#include "Attributes.h"

Attributes::Attributes()
{
	// 0 is a neutral value, middle of the scale
	this->reputation = 0;
	this->standard = 0;
	this->prices = 0;
	this->creepy = 0;
	this->shady = 0;
	this->drinkQuality = 0;
	this->foodQuality = 0;
}

Attributes::~Attributes()
{
}

int Attributes::getReputation() const
{
	return this->reputation;
}

int Attributes::getStandard() const
{
	return this->standard;
}

int Attributes::getPrices() const
{
	return this->prices;
}

int Attributes::getCreepy() const
{
	return this->creepy;
}

int Attributes::getShady() const
{
	return this->shady;
}

int Attributes::getDrinkQuality() const
{
	return this->drinkQuality;
}

int Attributes::getFoodQuality() const
{
	return this->foodQuality;
}

void Attributes::setReputation(int value)
{
	this->reputation += value;
}

void Attributes::setStandard(int value)
{
	this->standard += value;
}

void Attributes::setPrices(int value)
{
	this->prices += value;
}

void Attributes::setCreepy(int value)
{
	this->creepy += value;
}

void Attributes::setShady(int value)
{
	this->shady += value;
}

void Attributes::setDrinkQuality(int value)
{
	this->drinkQuality += value;
}

void Attributes::setFoodQuality(int value)
{
	this->foodQuality += value;
}