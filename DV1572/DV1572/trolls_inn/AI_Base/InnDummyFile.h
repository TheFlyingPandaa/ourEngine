#pragma once

class InnDummyFile
{
private:

public:
	InnDummyFile();
	~InnDummyFile();

	int getFoodPrice();
	int getSleepPrice();
	int getDrinkPrice();

	void customerReview(Attributes review);
};