#pragma once
#include "Attributes.h"

enum Type
{
	Human,
	Troll
};

class Customer
{
private:
	Attributes stats;
	int gold;
	Type race;
	int posX, posY;

public:
	Customer();
	Customer(Type race, int gold);
	~Customer();

	Attributes getAttributes() const;
	int getGold() const;
	Type getRace() const;
	int getPosX() const;
	int getPosY() const;
};