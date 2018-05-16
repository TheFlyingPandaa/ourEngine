#include "Inn.h"

Inn::Inn()
{
	m_economy = new Economy(START_MONEY);
	
	m_timer = 0;
	m_text = new Text();
	m_text->setFontType(TXT::Constantina_Big);
	m_text->setColor(0.02f, 0.3f, 0.0f);
	m_text->setRelative(Text::RelativeTo::BR);
	m_text->setPosition(198.0f, 36.0f);
	m_text->setScale(0.5f);
	m_text->setRotation(0.0f);
	m_text->setTextString("");
	m_text->setAllignment(TXT::Right);
	this->m_foodPrice = 10;
	this->m_drinkPrice = 15;
	this->m_sleepPrice = 20;

	m_receptionPos = DirectX::XMINT2(16, 1);

	m_rent = 0;

	m_innLevel = 1;
	m_innXPLevel = 0;
}

Inn::~Inn()
{
	delete m_economy;
	/*
	delete m_text;
	m_text = nullptr;*/
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

bool Inn::GetRecievedReview() const
{
	return m_recievedReview;
}

void Inn::SetRecievedReviewToFalse()
{
	m_recievedReview = false;
}

void Inn::SetTrue()
{
	m_recievedReview = true;
}

Attributes Inn::GetInnAttributes() const
{
	return this->m_innAttributes;
}

Attributes& Inn::getInnAttributesRef()
{
	return m_innAttributes;
}

void Inn::Update(double deltaTime, TIMEOFDAY TOD)
{
	if (TOD == TIMEOFDAY::EVENINGTONIGHT && !m_staffSalaryApplyed) {
		
		m_staffSalaryApplyed = true;
		if (m_AngryCustomers >= 1)
		{
			m_AngryCustomers -= 1;
		}
	}

	m_timer += deltaTime;
	if (m_timer > UPDATE_FREQ)
	{
		m_timer = 0;

		if (TOD != TIMEOFDAY::EVENINGTONIGHT)
			m_staffSalaryApplyed = false;

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
	m_depositText.push_back(Text());
	m_depositText.at(m_depositText.size() - 1).setColor(0.0f, 100.0f, 0.0f, 1.0f);
	m_depositText.at(m_depositText.size() - 1).setRelative(static_cast<Text::RelativeTo>(1));
	m_depositText.at(m_depositText.size() - 1).setPosition(198.0f, 182.0f);
	m_depositText.at(m_depositText.size() - 1).setScale(0.4f);
	m_depositText.at(m_depositText.size() - 1).setRotation(0.0f);
	m_depositText.at(m_depositText.size() - 1).setTextString("$ " + std::to_string(amount));
	m_depositText.at(m_depositText.size() - 1).setAllignment(TXT::Right);
	m_depositAmount.push_back(amount);
	//UpdateMoney();
}

void Inn::Withdraw(int amount)
{
	m_withdrawText.push_back(Text());
	m_withdrawText.at(m_withdrawText.size() - 1).setColor(100.0f, 0.0f, 0.0f,1.0f);
	m_withdrawText.at(m_withdrawText.size() - 1).setRelative(static_cast<Text::RelativeTo>(1));
	m_withdrawText.at(m_withdrawText.size() - 1).setPosition(198.0f, 32.0f);
	m_withdrawText.at(m_withdrawText.size() - 1).setScale(0.4f);
	m_withdrawText.at(m_withdrawText.size() - 1).setRotation(0.0f);
	m_withdrawText.at(m_withdrawText.size() - 1).setTextString("$ " + std::to_string(amount));
	m_withdrawText.at(m_withdrawText.size() - 1).setAllignment(TXT::Right);
	m_economy->Withdraw(amount);
	UpdateMoney();
}

void Inn::IncreaseRent(int amount)
{
	m_rent += amount;
}

void Inn::DecreaseRent(int amount)
{
	m_rent -= amount;
}

void Inn::IncreaseXP(const int amount)
{
	m_innXPLevel += amount;
	if (m_innXPLevel >= m_innLevel * 50)
	{
		m_innLevel++;
		m_innXPLevel = 0;
		Deposit(m_innLevel * 50);
		std::string temp;
		temp = "Level UP: " + std::to_string(m_innLevel);
		InGameConsole::pushString(temp);
		std::cout << "LEVEL UP" << std::endl;
	}
}

void Inn::AddAngryCustomer()
{
	m_AngryCustomers++;
}

int Inn::getAngryCustomers() const
{
	return m_AngryCustomers;
}

int Inn::getAngryCustomersCap() const
{
	return m_angryCustomerCap;
}

void Inn::GetRefund(int amount)
{
	m_economy->GetRefund(amount);
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

void Inn::CustomerReview(Attributes review)
{
	std::cout << "A customer added " << review.GetStat() << " stats." << std::endl;
	if (m_innAttributes.GetStat() > 0 && review.GetStat() < 0)
	{
		m_innAttributes.AddStat(review.GetStat());
		m_recievedReview = true;
	}
	else if (m_innAttributes.GetStat() < 0 && review.GetStat() > 0)
	{
		m_innAttributes.AddStat(review.GetStat());
		m_recievedReview = true;
	}
}

void Inn::FurnitureStatAdd(Attributes furnitureStats)
{
	m_innAttributes.AddStat(furnitureStats.GetStat());
}

void Inn::Draw()
{
	for (int i = 0; i < m_withdrawText.size(); i++)
	{
		DirectX::XMVECTOR pos = m_withdrawText.at(i).getPosition();
		m_withdrawText.at(i).setPosition(XMVectorGetX(pos), XMVectorGetY(pos) + 1);
		auto color = m_withdrawText.at(i).getColor();
		m_withdrawText.at(i).setColor(XMVectorGetX(color), XMVectorGetY(color), XMVectorGetZ(color), XMVectorGetW(color) - 0.01f);
		if (XMVectorGetY(pos) >= 182)
		{
			//m_withdrawText.erase(m_withdrawText.begin() + i);
			m_removeVec.push_back(i);
		}
		m_withdrawText.at(i).Draw();
	}
	if (m_removeVec.size() >0)
	{
		for (auto element : m_removeVec)
		{
			m_withdrawText.erase(m_withdrawText.begin() + element);
			break;
		}
		m_removeVec.clear();
	}
	for (int i = 0; i < m_depositText.size(); i++)
	{
		DirectX::XMVECTOR pos = m_depositText.at(i).getPosition();
		m_depositText.at(i).setPosition(198, XMVectorGetY(pos) - 1);
		auto color = m_depositText.at(i).getColor();
		m_depositText.at(i).setColor(XMVectorGetX(color), XMVectorGetY(color), XMVectorGetZ(color), XMVectorGetW(color) - 0.005f);
		if (XMVectorGetY(pos) <= 32)
		{
			//m_withdrawText.erase(m_withdrawText.begin() + i);
			m_economy->Deposit(m_depositAmount.at(i));
			UpdateMoney();
			m_removeVec.push_back(i);
		}
		m_depositText.at(i).Draw();
	}
	if (m_removeVec.size() >0)
	{
		for (auto element : m_removeVec)
		{
			m_depositAmount.erase(m_depositAmount.begin() + element);
			m_depositText.erase(m_depositText.begin() + element);
			break;
		}
		m_removeVec.clear();
	}
}

void Inn::WithdrawText()
{
	
}