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

Attributes::Attributes(int rep, int sta, int pri, int cre, int shad, int drin, int fod)
	: reputation(rep), standard(sta), prices(pri), creepy(cre), shady(shad), drinkQuality(drin), foodQuality(fod)
{
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
	this->reputation = std::min(std::max(this->reputation, -10), 10);
}

void Attributes::setStandard(int value)
{
	this->standard += value;
	this->standard = std::min(std::max(this->standard, -10), 10);
}

void Attributes::setPrices(int value)
{
	this->prices += value;
	this->prices = std::min(std::max(this->prices, -10), 10);
}

void Attributes::setCreepy(int value)
{
	
	this->creepy += value;
	this->creepy = std::min(std::max(this->creepy, -10), 10);
}

void Attributes::setShady(int value)
{
	this->shady += value;
	this->shady = std::min(std::max(this->shady, -10), 10);
}

void Attributes::setDrinkQuality(int value)
{
	this->drinkQuality += value;
	this->drinkQuality = std::min(std::max(this->drinkQuality, -10), 10);
}

void Attributes::setFoodQuality(int value)
{
	this->foodQuality += value;
	this->foodQuality = std::min(std::max(this->foodQuality, -10), 10);
}