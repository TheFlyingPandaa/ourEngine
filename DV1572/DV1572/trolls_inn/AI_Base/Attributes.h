#pragma once

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
	~Attributes();

	int getReputation() const;
	int getStandard() const;
	int getPrices() const;
	int getCreepy() const;
	int getShady() const;
	int getDrinkQuality() const;
	int getFoodQuality() const;

	// adds the function argument to the attribute
	void setReputation(int value);
	void setStandard(int value);
	void setPrices(int value);
	void setCreepy(int value);
	void setShady(int value);
	void setDrinkQuality(int value);
	void setFoodQuality(int value);

};