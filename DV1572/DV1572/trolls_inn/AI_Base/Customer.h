#pragma once
#include "Attributes.h"
#include "Economy.h"

enum Race
{
	Human,
	Troll,
	Orc,
	Dwarf
};

class Customer
{
private:
	Attributes stats;
	Economy economy;
	Race race;
	int posX, posY;

	int hungry;
	int tired;
	int thirsty;

public:
	Customer();
	Customer(Race race, int gold);
	~Customer();

	Attributes& getAttributes();
	Economy& getEconomy();
	Race getRace() const;
	int getPosX() const;
	int getPosY() const;
	int getHungry() const;
	int getTired() const;
	int getThirsty() const;

	void setHungry(int value);
	void setTired(int value);
	void setThirsty(int value);

	void move(int x, int y);
};