#include "Inn.h"
#include "FroggeDebug.h"

void Inn::_addStatsToInn(Attributes type)
{
	this->m_iUA.creepy += type.GetCreepy();
	this->m_iUA.drinkQuality += type.GetDrinkQuality();
	this->m_iUA.foodQuality += type.GetFoodQuality();
	this->m_iUA.prices += type.GetPrices();
	this->m_iUA.reputation += type.GetReputation();
	this->m_iUA.shady += type.GetShady();
	this->m_iUA.standard += type.GetStandard();
}

void Inn::_checkInnStatUpdate()
{
	if (this->m_iUA.creepy >= 10.0f)
	{
		this->m_innAttributes.SetCreepy(1);
		//this->m_innAttributes.SetCreepy((int)(this->m_iUA.creepy * 0.1));
		this->m_iUA.creepy -= 10.0f;
	}
	else if (this->m_iUA.drinkQuality >= 10.0f)
	{
		this->m_innAttributes.SetDrinkQuality(1);
		this->m_iUA.drinkQuality -= 10.0f;
	}
	else if (this->m_iUA.foodQuality >= 10.0f)
	{
		this->m_innAttributes.SetFoodQuality(1);
		this->m_iUA.foodQuality -= 10.0f;
	}
	else if (this->m_iUA.prices >= 10.0f)
	{
		this->m_innAttributes.SetPrices(1);
		this->m_iUA.prices -= 10.0f;
	}
	else if (this->m_iUA.reputation >= 10.0f)
	{
		this->m_innAttributes.SetReputation(1);
		this->m_iUA.reputation -= 10.0f;
	}
	else if (this->m_iUA.shady >= 10.0f)
	{
		this->m_innAttributes.SetShady(1);
		this->m_iUA.shady -= 10.0f;
	}
	else
	{
		this->m_innAttributes.SetStandard(1);
		this->m_iUA.standard -= 10.0f;
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
	return this->m_foodPrice;
}

int Inn::getSleepPrice() const
{
	return this->m_sleepPrice;
}

int Inn::getDrinkPrice() const
{
	return this->m_drinkPrice;
}

Attributes Inn::getInnAttributes() const
{
	return this->m_innAttributes;
}

// Change to one standard function for all stat adds?
void Inn::customerReview(Attributes review)
{
	this->_addStatsToInn(review);
	this->_checkInnStatUpdate();
}

void Inn::furnitureStatAdd(Attributes furnitureStats)
{
	this->_addStatsToInn(furnitureStats);
	this->_checkInnStatUpdate();
}

void Inn::Update(Camera * cam)
{
	
}

void Inn::Draw()
{
	
}
