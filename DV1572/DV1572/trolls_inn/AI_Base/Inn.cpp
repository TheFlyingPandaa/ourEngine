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
	m_economy = new Economy(START_MONEY);
	m_profit = 0;
	m_timer = 0;
}

Inn::~Inn()
{
	delete m_economy;
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

void Inn::Update(double deltaTime, TIMEOFDAY TOD)
{
	if (TOD == TIMEOFDAY::EVENINGTONIGHT && !m_staffSalaryApplyed) {
		m_profit -= m_staffSalary;
		m_staffSalaryApplyed = true;
	}
	

	m_timer += deltaTime;
	if (m_timer > UPDATE_FREQ)
	{
		m_timer = 0;



		if (m_profit > 0)
			m_economy->deposit(std::abs(m_profit));
		else
			m_economy->withdraw(std::abs(m_profit));

		if (TOD != TIMEOFDAY::EVENINGTONIGHT)
			m_staffSalaryApplyed = false;

		m_profit = 0;
		std::cout << m_economy->getGold() << std::endl;
	}
}

int Inn::getMoney() const
{
	return m_economy->getGold();
}

void Inn::Deposit(int amount)
{
	m_profit += amount;
}

void Inn::Withdraw(int amount)
{
	m_economy->withdraw(amount);
}

void Inn::changeStaffSalary(int amount)
{
	m_staffSalary += amount;
	if (m_staffSalary < 0)
		m_staffSalary = 0;
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
