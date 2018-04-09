#pragma once
#include <algorithm>
class Attributes
{
private:
	int reputation;
	int standard;
	int prices;
	int creepy;
	int shady;
	int drinkQuality;
	int foodQuality;

public:
	Attributes();
	Attributes(int rep, int sta, int pri, int cre, int shad, int drin, int fod);
	~Attributes();

	int getReputation() const;
	int getStandard() const;
	int getPrices() const;
	int getCreepy() const;
	int getShady() const;
	int getDrinkQuality() const;
	int getFoodQuality() const;

	// Adds the function argument to the attribute
	void setReputation(int value);
	void setStandard(int value);
	void setPrices(int value);
	void setCreepy(int value);
	void setShady(int value);
	void setDrinkQuality(int value);
	void setFoodQuality(int value);

};

static Attributes orcAtr = { -5, -5, 2, -7, -7, 4, 6 };
static Attributes humanAtr = { 0, 0, 0, 0, 0, 0, 0 };
static Attributes trollAtr = { 0, -3, -5, 2, -1, 2, 2 };
static Attributes dwarfAtr = { -3, -5, 0, 0, 8, 2, 1 };

