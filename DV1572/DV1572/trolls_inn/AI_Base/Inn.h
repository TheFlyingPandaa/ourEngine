#pragma once
#include "Attributes.h"
#include "../Bobby test/Room/Grid.h"

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
	friend class AISolver;
private:
	// Grid stuff
	Grid * m_grid;
	Mesh m;
	Mesh kitchenTile;
	Mesh rect;
	Mesh door;

	// Inn stuff
	Attributes innAttributes;
	innUpdateAttributes iUA;
	int foodPrice;
	int sleepPrice;
	int drinkPrice;

	void addStatsToInn(Attributes type);
	void checkInnStatUpdate();

public:
	Inn();
	~Inn();


	int getFoodPrice() const;
	int getSleepPrice() const;
	int getDrinkPrice() const;

	Attributes getInnAttributes() const;
	Grid* getGrid();

	// Add a customers review to the inn stats
	void customerReview(Attributes review);
	// Add stats to the inn based on the furniture placed
	void furnitureStatAdd(Attributes furnitureStats);

	void update(Camera* cam);
	void Draw();
};