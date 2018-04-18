#include "Inn.h"

void Inn::addStatsToInn(Attributes type)
{
	this->iUA.creepy += type.getCreepy();
	this->iUA.drinkQuality += type.getDrinkQuality();
	this->iUA.foodQuality += type.getFoodQuality();
	this->iUA.prices += type.getPrices();
	this->iUA.reputation += type.getReputation();
	this->iUA.shady += type.getShady();
	this->iUA.standard += type.getStandard();
}

void Inn::checkInnStatUpdate()
{
	if (this->iUA.creepy >= 10.0f)
	{
		this->innAttributes.setCreepy((int)(this->iUA.creepy * 0.1));
		this->iUA.creepy -= 10.0f;
	}
	else if (this->iUA.drinkQuality >= 10.0f)
	{
		this->innAttributes.setDrinkQuality((int)(this->iUA.drinkQuality * 0.1));
		this->iUA.drinkQuality -= 10.0f;
	}
	else if (this->iUA.foodQuality >= 10.0f)
	{
		this->innAttributes.setFoodQuality((int)(this->iUA.foodQuality * 0.1));
		this->iUA.foodQuality -= 10.0f;
	}
	else if (this->iUA.prices >= 10.0f)
	{
		this->innAttributes.setPrices((int)(this->iUA.prices * 0.1));
		this->iUA.prices -= 10.0f;
	}
	else if (this->iUA.reputation >= 10.0f)
	{
		this->innAttributes.setReputation((int)(this->iUA.reputation * 0.1));
		this->iUA.reputation -= 10.0f;
	}
	else if (this->iUA.shady >= 10.0f)
	{
		this->innAttributes.setShady((int)(this->iUA.shady * 0.1));
		this->iUA.shady -= 10.0f;
	}
	else
	{
		this->innAttributes.setStandard((int)(this->iUA.standard * 0.1));
		this->iUA.standard -= 10.0f;
	}
}

Inn::Inn()
{
	kitchenTile.MakeRectangle();
	kitchenTile.setDiffuseTexture("trolls_inn/Resources/Untitled.bmp");
	kitchenTile.setNormalTexture("trolls_inn/Resources/BatmanNormal.png");
	rect.MakeRectangle();
	rect.setDiffuseTexture("trolls_inn/Resources/Grass.jpg");
	rect.setNormalTexture("trolls_inn/Resources/GrassNormal.png");
	door.LoadModel("trolls_inn/Resources/door/Door.obj");
	door.setNormalTexture("trolls_inn/Resources/door/SickDoorNormal.png");
	this->m.LoadModel("trolls_inn/Resources/Wall2.obj");
	this->m.setNormalTexture("trolls_inn/Resources/woodNormalMap.jpg");


	int startSize = 32;
	int firstRoomSizeX = 4;
	int firstRoomSizeY = 3;

	int secondRoomSizeX = 4;
	int secondRoomSizeY = 2;

	m_grid = new Grid(0, 0, startSize, startSize, &rect);
	m_grid->CreateWalls(&m);
	m_grid->getRoomCtrl().setTileMesh(&kitchenTile, RoomType::kitchen);
	m_grid->getRoomCtrl().setDoorMesh(&door);
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
	return this->foodPrice;
}

int Inn::getSleepPrice() const
{
	return this->sleepPrice;
}

int Inn::getDrinkPrice() const
{
	return this->drinkPrice;
}

Attributes Inn::getInnAttributes() const
{
	return this->innAttributes;
}

Grid * Inn::getGrid()
{
	return m_grid;
}

// Change to one standard function for all stat adds?
void Inn::customerReview(Attributes review)
{
	this->addStatsToInn(review);
	this->checkInnStatUpdate();
}

void Inn::furnitureStatAdd(Attributes furnitureStats)
{
	this->addStatsToInn(furnitureStats);
	this->checkInnStatUpdate();
}

void Inn::update(Camera * cam)
{
	m_grid->Update(cam);
}

void Inn::Draw()
{
	m_grid->Draw();
}
