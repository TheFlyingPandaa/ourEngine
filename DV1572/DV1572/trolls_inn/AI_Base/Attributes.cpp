#include "Attributes.h"

Attributes::Attributes()
{
	m_stat = 0.0f;
//	// 0 is a neutral value, middle of the scale
//	this->m_reputation = 0;
//	this->m_standard = 0;
//	this->m_prices = 0;
//	this->m_creepy = 0;
//	this->m_shady = 0;
//	this->m_drinkQuality = 0;
//	this->m_foodQuality = 0;
}

//Attributes::Attributes(int rep, int sta, int pri, int cre, int shad, int drin, int fod)
//	: m_reputation(rep), m_standard(sta), m_prices(pri), m_creepy(cre), m_shady(shad), m_drinkQuality(drin), m_foodQuality(fod)
//{
//}

Attributes::Attributes(float stat) : m_stat(stat)
{
}

Attributes::~Attributes()
{
}

float Attributes::GetStat() const
{
	return m_stat;
}

void Attributes::SetStat(float stat)
{
	m_stat = stat;
}

void Attributes::AddStat(float stat)
{
	m_stat += stat;
}

//int Attributes::GetReputation() const
//{
//	return this->m_reputation;
//}
//
//int Attributes::GetStandard() const
//{
//	return this->m_standard;
//}
//
//int Attributes::GetPrices() const
//{
//	return this->m_prices;
//}
//
//int Attributes::GetCreepy() const
//{
//	return this->m_creepy;
//}
//
//int Attributes::GetShady() const
//{
//	return this->m_shady;
//}
//
//int Attributes::GetDrinkQuality() const
//{
//	return this->m_drinkQuality;
//}
//
//int Attributes::GetFoodQuality() const
//{
//	return this->m_foodQuality;
//}
//
//void Attributes::SetReputation(int value)
//{
//	this->m_reputation += value;
//	this->m_reputation = std::min(std::max(this->m_reputation, -10), 10);
//}
//
//void Attributes::SetStandard(int value)
//{
//	this->m_standard += value;
//	this->m_standard = std::min(std::max(this->m_standard, -10), 10);
//}
//
//void Attributes::SetPrices(int value)
//{
//	this->m_prices += value;
//	this->m_prices = std::min(std::max(this->m_prices, -10), 10);
//}
//
//void Attributes::SetCreepy(int value)
//{
//	
//	this->m_creepy += value;
//	this->m_creepy = std::min(std::max(this->m_creepy, -10), 10);
//}
//
//void Attributes::SetShady(int value)
//{
//	this->m_shady += value;
//	this->m_shady = std::min(std::max(this->m_shady, -10), 10);
//}
//
//void Attributes::SetDrinkQuality(int value)
//{
//	this->m_drinkQuality += value;
//	this->m_drinkQuality = std::min(std::max(this->m_drinkQuality, -10), 10);
//}
//
//void Attributes::SetFoodQuality(int value)
//{
//	this->m_foodQuality += value;
//	this->m_foodQuality = std::min(std::max(this->m_foodQuality, -10), 10);
//}
//
//void Attributes::SetReviewReputation(int value)
//{
//	this->m_reputation += value;
//}
//
//void Attributes::SetReviewStandard(int value)
//{
//	this->m_standard += value;
//}
//
//void Attributes::SetReviewPrices(int value)
//{
//	this->m_prices += value;
//}
//
//void Attributes::SetReviewCreepy(int value)
//{
//	this->m_creepy += value;
//}
//
//void Attributes::SetReviewShady(int value)
//{
//	this->m_shady += value;
//}
//
//void Attributes::SetReviewDrinkQuality(int value)
//{
//	this->m_drinkQuality += value;
//}
//
//void Attributes::SetReviewFoodQuality(int value)
//{
//	this->m_foodQuality += value;
//}
