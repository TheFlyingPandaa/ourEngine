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
	this->addStatsToInn(review);
	this->checkInnStatUpdate();
}

void Inn::furnitureStatAdd(Attributes furnitureStats)
{
	this->addStatsToInn(furnitureStats);
	this->checkInnStatUpdate();
}
