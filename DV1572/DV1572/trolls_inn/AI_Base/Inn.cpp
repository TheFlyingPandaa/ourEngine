#include "Inn.h"
#include "FroggeDebug.h"

void Inn::_addStatsToInn(Attributes type)
{
	this->m_innUpdateAttributes.SetReviewCreepy(type.GetCreepy());
	this->m_innUpdateAttributes.SetReviewDrinkQuality(type.GetDrinkQuality());
	this->m_innUpdateAttributes.SetReviewFoodQuality(type.GetFoodQuality());
	this->m_innUpdateAttributes.SetReviewPrices(type.GetPrices());
	this->m_innUpdateAttributes.SetReviewReputation(type.GetReputation());
	this->m_innUpdateAttributes.SetReviewShady(type.GetShady());
	this->m_innUpdateAttributes.SetReviewStandard(type.GetStandard());
}

void Inn::_checkInnStatUpdate()
{
	if (this->m_innUpdateAttributes.GetCreepy() >= 10)
	{
		this->m_innAttributes.SetCreepy(1);
		this->m_innUpdateAttributes.SetReviewCreepy(-10);
	}
	else if (this->m_innUpdateAttributes.GetCreepy() <= -10)
	{
		this->m_innAttributes.SetCreepy(-1);
		this->m_innUpdateAttributes.SetReviewCreepy(10);
	}
	if (this->m_innUpdateAttributes.GetDrinkQuality() >= 10)
	{
		this->m_innAttributes.SetDrinkQuality(1);
		this->m_innUpdateAttributes.SetReviewDrinkQuality(-10);
	}
	else if (this->m_innUpdateAttributes.GetDrinkQuality() <= -10)
	{
		this->m_innAttributes.SetDrinkQuality(-1);
		this->m_innUpdateAttributes.SetReviewDrinkQuality(10);
	}
	if (this->m_innUpdateAttributes.GetFoodQuality() >= 10)
	{
		this->m_innAttributes.SetFoodQuality(1);
		this->m_innUpdateAttributes.SetReviewFoodQuality(-10);
	}
	else if (this->m_innUpdateAttributes.GetFoodQuality() <= -10)
	{
		this->m_innAttributes.SetFoodQuality(-1);
		this->m_innUpdateAttributes.SetReviewFoodQuality(10);
	}
	if (this->m_innUpdateAttributes.GetPrices() >= 10)
	{
		this->m_innAttributes.SetPrices(1);
		this->m_innUpdateAttributes.SetReviewPrices(-10);
	}
	else if (this->m_innUpdateAttributes.GetPrices() <= -10)
	{
		this->m_innAttributes.SetPrices(-1);
		this->m_innUpdateAttributes.SetReviewPrices(10);
	}
	if (this->m_innUpdateAttributes.GetReputation() >= 10)
	{
		this->m_innAttributes.SetReputation(1);
		this->m_innUpdateAttributes.SetReviewReputation(-10);
	}
	else if (this->m_innUpdateAttributes.GetReputation() <= -10)
	{
		this->m_innAttributes.SetReputation(-1);
		this->m_innUpdateAttributes.SetReviewReputation(10);
	}
	if (this->m_innUpdateAttributes.GetShady() >= 10)
	{
		this->m_innAttributes.SetShady(1);
		this->m_innUpdateAttributes.SetReviewShady(-10);
	}
	else if (this->m_innUpdateAttributes.GetShady() <= -10)
	{
		this->m_innAttributes.SetShady(-1);
		this->m_innUpdateAttributes.SetReviewShady(10);
	}
	if (this->m_innUpdateAttributes.GetStandard() >= 10)
	{
		this->m_innAttributes.SetStandard(1);
		this->m_innUpdateAttributes.SetReviewStandard(-10);
	}
	else if (this->m_innUpdateAttributes.GetStandard() <= -10)
	{
		this->m_innAttributes.SetStandard(-1);
		this->m_innUpdateAttributes.SetReviewStandard(10);
	}
	std::cout << "Creepy: " << this->m_innAttributes.GetCreepy() << std::endl;
	std::cout << "Drink Quality: " << this->m_innAttributes.GetDrinkQuality() << std::endl;
	std::cout << "Food Quality: " << this->m_innAttributes.GetFoodQuality() << std::endl;
	std::cout << "Prices: " << this->m_innAttributes.GetPrices() << std::endl;
	std::cout << "Reputation: " << this->m_innAttributes.GetReputation() << std::endl;
	std::cout << "Shady: " << this->m_innAttributes.GetShady() << std::endl;
	std::cout << "Standard: " << this->m_innAttributes.GetStandard() << std::endl;
}

Inn::Inn()
{
	m_kitchenTile.MakeRectangle();
	m_kitchenTile.setDiffuseTexture("trolls_inn/Resources/Untitled.bmp");
	m_kitchenTile.setNormalTexture("trolls_inn/Resources/BatmanNormal.png");
	m_rect.MakeRectangle();
	m_rect.setDiffuseTexture("trolls_inn/Resources/Grass.jpg");
	m_rect.setNormalTexture("trolls_inn/Resources/GrassNormal.png");
	m_door.LoadModel("trolls_inn/Resources/door/Door.obj");
	m_door.setNormalTexture("trolls_inn/Resources/door/SickDoorNormal.png");
	this->m_m.LoadModel("trolls_inn/Resources/Wall2.obj");
	this->m_m.setNormalTexture("trolls_inn/Resources/woodNormalMap.jpg");


	int startSize = 32;
	int firstRoomSizeX = 5;
	int firstRoomSizeY = 8;

	int secondRoomSizeX = 4;
	int secondRoomSizeY = 2;

	m_grid = DBG_NEW Grid(0, 0, startSize, startSize, &m_rect);
	m_grid->CreateWalls(&m_m);
	m_grid->getRoomCtrl().setTileMesh(&m_kitchenTile, RoomType::kitchen);
	m_grid->getRoomCtrl().setDoorMesh(&m_door);
	m_grid->AddRoom(DirectX::XMINT2((startSize / 2) - firstRoomSizeX / 2, 4), DirectX::XMINT2(firstRoomSizeX, firstRoomSizeY), RoomType::kitchen, true);
	////grid->getRoomCtrl().CreateDoor(grid->getGrid()[(startSize / 2)][4], grid->getGrid()[(startSize / 2)][3]);
	m_grid->getRoomCtrl().CreateMainDoor(m_grid->getGrid()[(startSize / 2)][4], m_grid->getGrid()[(startSize / 2)][3]);	//This will create the main door and place the pos in in m_mainDoorPos 

	this->m_foodPrice = 10;
	this->m_drinkPrice = 15;
	this->m_sleepPrice = 20;
}

Inn::~Inn()
{
	delete m_grid;
}

int Inn::GetFoodPrice() const
{
	return this->m_foodPrice;
}

int Inn::GetSleepPrice() const
{
	return this->m_sleepPrice;
}

int Inn::GetDrinkPrice() const
{
	return this->m_drinkPrice;
}

Attributes Inn::GetInnAttributes() const
{
	return this->m_innAttributes;
}

Grid * Inn::GetGrid()
{
	return m_grid;
}

// Change to one standard function for all stat adds?
void Inn::CustomerReview(Attributes review)
{
	this->_addStatsToInn(review);
	this->_checkInnStatUpdate();
}

void Inn::FurnitureStatAdd(Attributes furnitureStats)
{
	this->_addStatsToInn(furnitureStats);
	this->_checkInnStatUpdate();
}

void Inn::Update(Camera * cam)
{
	m_grid->Update(cam);
}

void Inn::Draw()
{
	m_grid->Draw();
}
