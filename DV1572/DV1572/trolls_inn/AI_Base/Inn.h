#pragma once
#include "Attributes.h"

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

class Inn
{
private:
	Attributes innAttributes;
	//Attributes innUpdateAttributes;
	innUpdateAttributes iUA;
	int foodPrice;
	int sleepPrice;
	int drinkPrice;

public:
	Inn();
	~Inn();

	int getFoodPrice() const;
	int getSleepPrice() const;
	int getDrinkPrice() const;

	Attributes getInnAttributes() const;

	void customerReview(Attributes review);

};