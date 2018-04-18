#include "Inn.h"

void Inn::addStatsToInn(Attributes type)
{
	this->iUA.creepy += type.getCreepy();
	this->iUA.drinkQuality += type.getDrinkQuality();
	this->iUA.foodQuality += type.getFoodQuality();
	this->iUA.prices += type.getPrices();
	this->iUA.reputation += type.getReputation();
	this->iUA.shady += type.getShady();
	this->iUA.standard += type.getStandard();
}

void Inn::checkInnStatUpdate()
{
	if (this->iUA.creepy >= 10.0f)
	{
		this->innAttributes.setCreepy((int)(this->iUA.creepy * 0.1));
		this->iUA.creepy -= 10.0f;
	}
	else if (this->iUA.drinkQuality >= 10.0f)
	{
		this->innAttributes.setDrinkQuality((int)(this->iUA.drinkQuality * 0.1));
		this->iUA.drinkQuality -= 10.0f;
	}
	else if (this->iUA.foodQuality >= 10.0f)
	{
		this->innAttributes.setFoodQuality((int)(this->iUA.foodQuality * 0.1));
		this->iUA.foodQuality -= 10.0f;
	}
	else if (this->iUA.prices >= 10.0f)
	{
		this->innAttributes.setPrices((int)(this->iUA.prices * 0.1));
		this->iUA.prices -= 10.0f;
	}
	else if (this->iUA.reputation >= 10.0f)
	{
		this->innAttributes.setReputation((int)(this->iUA.reputation * 0.1));
		this->iUA.reputation -= 10.0f;
	}
	else if (this->iUA.shady >= 10.0f)
	{
		this->innAttributes.setShady((int)(this->iUA.shady * 0.1));
		this->iUA.shady -= 10.0f;
	}
	else
	{
		this->innAttributes.setStandard((int)(this->iUA.standard * 0.1));
		this->iUA.standard -= 10.0f;
	}
}

Inn::Inn()
{
}

Inn::~Inn()
{
}

int Inn::getFoodPrice() const
{
	return this->foodPrice;
}

int Inn::getSleepPrice() const
{
	return this->sleepPrice;
}

int Inn::getDrinkPrice() const
{
	return this->drinkPrice;
}

Attributes Inn::getInnAttributes() const
{
	return this->innAttributes;
}

// Change to one standard function for all stat adds?
void Inn::customerReview(Attributes review)
{
	this->addStatsToInn(review);
	this->checkInnStatUpdate();
}

void Inn::furnitureStatAdd(Attributes furnitureStats)
{
	this->addStatsToInn(furnitureStats);
	this->checkInnStatUpdate();
}
