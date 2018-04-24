#pragma once
#include "Attributes.h"
#include "Economy.h"
struct innUpdateAttributes
{
	float creepy;
	float drinkQuality;
	float foodQuality;
	float prices;
	float reputation;
	float shady;
	float standard;
};

const int START_MONEY = 500;

class Inn
{
private:
	Attributes innAttributes;
	innUpdateAttributes iUA;
	int foodPrice;
	int sleepPrice;
	int drinkPrice;

	void addStatsToInn(Attributes type);
	void checkInnStatUpdate();

	Economy*		m_economy;

public:
	Inn();
	~Inn();

	int getFoodPrice() const;
	int getSleepPrice() const;
	int getDrinkPrice() const;

	Attributes getInnAttributes() const;

	void Update();
	int getMoney() const;

	// Add a customers review to the inn stats
	void customerReview(Attributes review);
	// Add stats to the inn based on the furniture placed
	void furnitureStatAdd(Attributes furnitureStats);

};