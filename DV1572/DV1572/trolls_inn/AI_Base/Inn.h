#pragma once
#include "Attributes.h"

class Inn
{
private:
	Attributes innAttributes;
	Attributes innUpdateAttributes;

public:
	Inn();
	~Inn();

	int getFoodPrice();
	int getSleepPrice();
	int getDrinkPrice();

	void customerReview(Attributes review);

};