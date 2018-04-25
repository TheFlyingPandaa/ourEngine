#pragma once
#include <algorithm>

class Attributes
{
private:
	int m_reputation;
	int m_standard;
	int m_prices;
	int m_creepy;
	int m_shady;
	int m_drinkQuality;
	int m_foodQuality;

public:
	Attributes();
	Attributes(int rep, int sta, int pri, int cre, int shad, int drin, int fod);
	~Attributes();

	int GetReputation() const;
	int GetStandard() const;
	int GetPrices() const;
	int GetCreepy() const;
	int GetShady() const;
	int GetDrinkQuality() const;
	int GetFoodQuality() const;

	// Adds the function argument to the attribute
	void SetReputation(int value);
	void SetStandard(int value);
	void SetPrices(int value);
	void SetCreepy(int value);
	void SetShady(int value);
	void SetDrinkQuality(int value);
	void SetFoodQuality(int value);

	// Same as previous sets, but for reviews
	void SetReviewReputation(int value);
	void SetReviewStandard(int value);
	void SetReviewPrices(int value);
	void SetReviewCreepy(int value);
	void SetReviewShady(int value);
	void SetReviewDrinkQuality(int value);
	void SetReviewFoodQuality(int value);

};

static Attributes orcAtr = { -5, -5, 2, -7, -7, 4, 6 };
static Attributes humanAtr = { 0, 0, 0, 0, 0, 0, 0 };
static Attributes trollAtr = { 0, -3, -5, 2, -1, 2, 2 };
static Attributes dwarfAtr = { -3, -5, 0, 0, 8, 2, 1 };

