#pragma once
#include "Attributes.h"
#include "../Bobby test/Room/Grid.h"

class Inn
{
	friend class AISolver;
private:
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

	// Grid stuff
	Grid* m_grid;
	Mesh m_m;
	Mesh m_kitchenTile;
	Mesh m_rect;
	Mesh m_door;

	// Inn stuff
	Attributes m_innAttributes;
	innUpdateAttributes m_iUA;
	int m_foodPrice;
	int m_sleepPrice;
	int m_drinkPrice;

	void _addStatsToInn(Attributes type);
	void _checkInnStatUpdate();

public:
	Inn();
	~Inn();


	int GetFoodPrice() const;
	int GetSleepPrice() const;
	int GetDrinkPrice() const;

	Attributes GetInnAttributes() const;
	Grid* GetGrid();

	// Add a customers review to the inn stats
	void CustomerReview(Attributes review);
	// Add stats to the inn based on the furniture placed
	void FurnitureStatAdd(Attributes furnitureStats);

	void Update(Camera* cam);
	void Draw();
};