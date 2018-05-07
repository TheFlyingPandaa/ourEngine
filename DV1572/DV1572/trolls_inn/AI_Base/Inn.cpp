#include "Inn.h"
#include "FroggeDebug.h"

void Inn::_addStatsToInn(Attributes type)
{
	this->m_innUpdateAttributes.SetReviewCreepy(type.GetCreepy());
	this->m_innUpdateAttributes.SetReviewDrinkQuality(type.GetDrinkQuality());
	this->m_innUpdateAttributes.SetReviewFoodQuality(type.GetFoodQuality());
	this->m_innUpdateAttributes.SetReviewPrices(type.GetPrices());
	this->m_innUpdateAttributes.SetReviewReputation(type.GetReputation());
	this->m_innUpdateAttributes.SetReviewShady(type.GetShady());
	this->m_innUpdateAttributes.SetReviewStandard(type.GetStandard());
}

void Inn::_checkInnStatUpdate()
{
	if (this->m_innUpdateAttributes.GetCreepy() >= 10)
	{
		this->m_innAttributes.SetCreepy(1);
		this->m_innUpdateAttributes.SetReviewCreepy(-10);
	}
	else if (this->m_innUpdateAttributes.GetCreepy() <= -10)
	{
		this->m_innAttributes.SetCreepy(-1);
		this->m_innUpdateAttributes.SetReviewCreepy(10);
	}
	if (this->m_innUpdateAttributes.GetDrinkQuality() >= 10)
	{
		this->m_innAttributes.SetDrinkQuality(1);
		this->m_innUpdateAttributes.SetReviewDrinkQuality(-10);
	}
	else if (this->m_innUpdateAttributes.GetDrinkQuality() <= -10)
	{
		this->m_innAttributes.SetDrinkQuality(-1);
		this->m_innUpdateAttributes.SetReviewDrinkQuality(10);
	}
	if (this->m_innUpdateAttributes.GetFoodQuality() >= 10)
	{
		this->m_innAttributes.SetFoodQuality(1);
		this->m_innUpdateAttributes.SetReviewFoodQuality(-10);
	}
	else if (this->m_innUpdateAttributes.GetFoodQuality() <= -10)
	{
		this->m_innAttributes.SetFoodQuality(-1);
		this->m_innUpdateAttributes.SetReviewFoodQuality(10);
	}
	if (this->m_innUpdateAttributes.GetPrices() >= 10)
	{
		this->m_innAttributes.SetPrices(1);
		this->m_innUpdateAttributes.SetReviewPrices(-10);
	}
	else if (this->m_innUpdateAttributes.GetPrices() <= -10)
	{
		this->m_innAttributes.SetPrices(-1);
		this->m_innUpdateAttributes.SetReviewPrices(10);
	}
	if (this->m_innUpdateAttributes.GetReputation() >= 10)
	{
		this->m_innAttributes.SetReputation(1);
		this->m_innUpdateAttributes.SetReviewReputation(-10);
	}
	else if (this->m_innUpdateAttributes.GetReputation() <= -10)
	{
		this->m_innAttributes.SetReputation(-1);
		this->m_innUpdateAttributes.SetReviewReputation(10);
	}
	if (this->m_innUpdateAttributes.GetShady() >= 10)
	{
		this->m_innAttributes.SetShady(1);
		this->m_innUpdateAttributes.SetReviewShady(-10);
	}
	else if (this->m_innUpdateAttributes.GetShady() <= -10)
	{
		this->m_innAttributes.SetShady(-1);
		this->m_innUpdateAttributes.SetReviewShady(10);
	}
	if (this->m_innUpdateAttributes.GetStandard() >= 10)
	{
		this->m_innAttributes.SetStandard(1);
		this->m_innUpdateAttributes.SetReviewStandard(-10);
	}
	else if (this->m_innUpdateAttributes.GetStandard() <= -10)
	{
		this->m_innAttributes.SetStandard(-1);
		this->m_innUpdateAttributes.SetReviewStandard(10);
	}
	std::cout << "Creepy: " << this->m_innAttributes.GetCreepy() << std::endl;
	std::cout << "Drink Quality: " << this->m_innAttributes.GetDrinkQuality() << std::endl;
	std::cout << "Food Quality: " << this->m_innAttributes.GetFoodQuality() << std::endl;
	std::cout << "Prices: " << this->m_innAttributes.GetPrices() << std::endl;
	std::cout << "Reputation: " << this->m_innAttributes.GetReputation() << std::endl;
	std::cout << "Shady: " << this->m_innAttributes.GetShady() << std::endl;
	std::cout << "Standard: " << this->m_innAttributes.GetStandard() << std::endl;
}

Inn::Inn()
{
	m_economy = new Economy(START_MONEY);
	m_profit = 0;
	m_timer = 0;
	m_text = new Text();
	m_text->setColor(0.0f, 1.0f, 0.0f);
	m_text->setRelative(static_cast<Text::RelativeTo>(1));
	m_text->setPosition(198.0f, 32.0f);
	m_text->setScale(0.4f);
	m_text->setRotation(0.0f);
	m_text->setTextString("");
	m_text->setAllignment(TXT::Right);
	this->m_foodPrice = 10;
	this->m_drinkPrice = 15;
	this->m_sleepPrice = 20;

	m_receptionPos = DirectX::XMINT2(16, 1);

	m_rent = 0;
}

Inn::~Inn()
{
	delete m_economy;
	delete m_text;
	m_text = nullptr;
}

DirectX::XMINT2 Inn::getReceptionPos()
{
	return m_receptionPos;
}

int Inn::GetFoodPrice() const
{
	return this->m_foodPrice;
}

int Inn::GetSleepPrice() const
{
	return this->m_sleepPrice;
}

int Inn::GetDrinkPrice() const
{
	return this->m_drinkPrice;
}

Attributes Inn::GetInnAttributes() const
{
	return this->m_innAttributes;
}

void Inn::Update(double deltaTime, TIMEOFDAY TOD)
{
	if (TOD == TIMEOFDAY::EVENINGTONIGHT && !m_staffSalaryApplyed) {
		m_profit -= m_staffSalary - m_rent;
		m_staffSalaryApplyed = true;
	}

	m_timer += deltaTime;
	if (m_timer > UPDATE_FREQ)
	{
		m_timer = 0;
		if (m_profit > 0)
			m_economy->Deposit(std::abs(m_profit));
		else
			m_economy->Withdraw(std::abs(m_profit));

		if (TOD != TIMEOFDAY::EVENINGTONIGHT)
			m_staffSalaryApplyed = false;

		m_profit = 0;
		m_text->setTextString("$" + std::to_string(m_economy->GetGold()));
		
	}
}

void Inn::UpdateMoney()
{
	m_text->setTextString("$" + std::to_string(m_economy->GetGold()));
}

int Inn::getMoney() const
{
	return m_economy->GetGold();
}

void Inn::Deposit(int amount)
{
	m_profit += amount;
}

void Inn::Withdraw(int amount)
{
	m_economy->Withdraw(amount);
}

void Inn::IncreaseRent(int amount)
{
	m_rent += amount;
}

void Inn::DecreaseRent(int amount)
{
	m_rent -= amount;
}

void Inn::changeStaffSalary(int amount)
{
	m_staffSalary += amount;
	if (m_staffSalary < 0)
		m_staffSalary = 0;
}

Text * Inn::GetText()
{
	return m_text;
}

// Change to one standard function for all stat adds?
void Inn::CustomerReview(Attributes review)
{
	this->_addStatsToInn(review);
	this->_checkInnStatUpdate();
}

void Inn::FurnitureStatAdd(Attributes furnitureStats)
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
