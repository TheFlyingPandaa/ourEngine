#include "Inn.h"
#include "FroggeDebug.h"

void Inn::_addStatsToInn(Attributes type)
{
	this->m_iUA.creepy += type.GetCreepy();
	this->m_iUA.drinkQuality += type.GetDrinkQuality();
	this->m_iUA.foodQuality += type.GetFoodQuality();
	this->m_iUA.prices += type.GetPrices();
	this->m_iUA.reputation += type.GetReputation();
	this->m_iUA.shady += type.GetShady();
	this->m_iUA.standard += type.GetStandard();
}

void Inn::_checkInnStatUpdate()
{
	if (this->m_iUA.creepy >= 10.0f)
	{
		this->m_innAttributes.SetCreepy((int)(this->m_iUA.creepy * 0.1));
		this->m_iUA.creepy -= 10.0f;
	}
	else if (this->m_iUA.drinkQuality >= 10.0f)
	{
		this->m_innAttributes.SetDrinkQuality((int)(this->m_iUA.drinkQuality * 0.1));
		this->m_iUA.drinkQuality -= 10.0f;
	}
	else if (this->m_iUA.foodQuality >= 10.0f)
	{
		this->m_innAttributes.SetFoodQuality((int)(this->m_iUA.foodQuality * 0.1));
		this->m_iUA.foodQuality -= 10.0f;
	}
	else if (this->m_iUA.prices >= 10.0f)
	{
		this->m_innAttributes.SetPrices((int)(this->m_iUA.prices * 0.1));
		this->m_iUA.prices -= 10.0f;
	}
	else if (this->m_iUA.reputation >= 10.0f)
	{
		this->m_innAttributes.SetReputation((int)(this->m_iUA.reputation * 0.1));
		this->m_iUA.reputation -= 10.0f;
	}
	else if (this->m_iUA.shady >= 10.0f)
	{
		this->m_innAttributes.SetShady((int)(this->m_iUA.shady * 0.1));
		this->m_iUA.shady -= 10.0f;
	}
	else
	{
		this->m_innAttributes.SetStandard((int)(this->m_iUA.standard * 0.1));
		this->m_iUA.standard -= 10.0f;
	}
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
	int firstRoomSizeX = 4;
	int firstRoomSizeY = 3;

	int secondRoomSizeX = 4;
	int secondRoomSizeY = 2;

	m_grid = DBG_NEW Grid(0, 0, startSize, startSize, &m_rect);
	m_grid->CreateWalls(&m_m);
	m_grid->getRoomCtrl().setTileMesh(&m_kitchenTile, RoomType::kitchen);
	m_grid->getRoomCtrl().setDoorMesh(&m_door);
	m_grid->AddRoom(DirectX::XMINT2((startSize / 2) - firstRoomSizeX / 2, 4), DirectX::XMINT2(firstRoomSizeX, firstRoomSizeY), RoomType::kitchen, true);
	////grid->getRoomCtrl().CreateDoor(grid->getGrid()[(startSize / 2)][4], grid->getGrid()[(startSize / 2)][3]);
	m_grid->getRoomCtrl().CreateMainDoor(m_grid->getGrid()[(startSize / 2)][4], m_grid->getGrid()[(startSize / 2)][3]);	//This will create the main door and place the pos in in m_mainDoorPos 
}

Inn::~Inn()
{
	delete m_grid;
}

int Inn::getFoodPrice() const
{
	return this->m_foodPrice;
}

int Inn::getSleepPrice() const
{
	return this->m_sleepPrice;
}

int Inn::getDrinkPrice() const
{
	return this->m_drinkPrice;
}

Attributes Inn::getInnAttributes() const
{
	return this->m_innAttributes;
}

Grid * Inn::getGrid()
{
	return m_grid;
}

// Change to one standard function for all stat adds?
void Inn::customerReview(Attributes review)
{
	this->_addStatsToInn(review);
	this->_checkInnStatUpdate();
}

void Inn::furnitureStatAdd(Attributes furnitureStats)
{
	this->_addStatsToInn(furnitureStats);
	this->_checkInnStatUpdate();
}

void Inn::update(Camera * cam)
{
	m_grid->Update(cam);
}

void Inn::Draw()
{
	m_grid->Draw();
}
