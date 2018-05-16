#include "Furniture.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "Table.h"
#include "Bar.h"
#include "Bed.h"
#include "Chair.h"

Furniture::Furniture(DirectX::XMFLOAT3 pos, MESH mesh)
	: m_meshId(mesh)
{
	p_rot = 0;
	this->pos.x = static_cast<int>(pos.x);
	this->pos.y = static_cast<int>(pos.y);
	this->pos.z = static_cast<int>(pos.z);
	p_object.setMesh(MLP::GetInstance().GetMesh(m_meshId));
	p_isBusy = false;

	p_level = 0;
}

Furniture::~Furniture()
{
}

Object3D& Furniture::getObject3D()
{
	return p_object;
}

Attributes& Furniture::getAttributes()
{
	return p_attributes;
}

int Furniture::getGridSize()
{
	return p_gridSize;
}

void Furniture::setPosition(DirectX::XMFLOAT3 pos)
{
	p_object.setPos(pos);
	this->pos.x = static_cast<int>(pos.x);
	this->pos.y = static_cast<int>(pos.y);
	this->pos.z = static_cast<int>(pos.z);
}

void Furniture::setPosition(float x, float y, float z)
{
	p_object.setPos(x, y, z);
	this->pos.x = static_cast<int>(x);
	this->pos.y = static_cast<int>(y);
	this->pos.z = static_cast<int>(z);
}

void Furniture::setRotation(int rot)
{
	p_rot = rot;
	if (p_rot < 0)
		p_rot += 360;
	else if (p_rot > 360)
		p_rot -= 360;

	p_object.setRotation(0.0f, static_cast<float>(rot) - 90.0f, 0.0f);
}

DirectX::XMINT3 Furniture::getPosition()
{
	return this->pos;
}

Customer * Furniture::getOwner() const
{
	return m_owner; 
}

int Furniture::getPrice()
{
	return p_price;
}

bool Furniture::getIsBusy()
{
	return p_isBusy;
}

void Furniture::setIsBusy(bool busy)
{
	p_isBusy = busy;
}

int Furniture::getLevel()
{
	return 0;
}

void Furniture::setLevel(int amount)
{
	p_level = amount;
}

int Furniture::AddLevel(int gold,int amount)
{
	if ((p_level + 1) * 100 > gold)
	{
		return 0;
	}
	p_level += amount;
	return p_level * 100;
}

void Furniture::setIndex(int index)
{
	m_index = index; 
}

void Furniture::setOwner(Customer * owner)
{
	m_owner = owner;
}

int Furniture::getIndex() const
{
	return m_index; 
}

void Furniture::releaseOwnerShip()
{
	m_owner = nullptr; 
}

int Furniture::getRotation()
{
	return p_rot;
}

void Furniture::setLightIndex(int index)
{
	p_object.setLightIndex(index);
}

/*void Furniture::LoadFurnitureStats(std::string path)
{
	std::string line;
	std::ifstream myfile(path);

	if (myfile.is_open())
	{
		
		while (std::getline(myfile, line))
		{

			if (line[0] != '#' || line != "")
			{
				std::istringstream stream(line);
				std::string type;

				stream >> type;
				if (type == "Reputation")
				{
					float t = 0;
					sscanf_s(line.c_str(), " %*s %f", &t);
					p_attributes.SetReputation(t);
				}
				else if (type == "Standard")
				{
					float t = 0;
					sscanf_s(line.c_str(), " %*s %f", &t);
					p_attributes.SetStandard(t);
					
				}
				else if (type == "Prices")
				{
					float t = 0;
					sscanf_s(line.c_str(), " %*s %f", &t);
					p_attributes.SetPrices(t);
					
				}
				else if (type == "Creepy")
				{
					float t = 0;
					sscanf_s(line.c_str(), " %*s %f", &t);
					p_attributes.SetCreepy(t);
				}
				else if (type == "Shady")
				{
					float t = 0;
					sscanf_s(line.c_str(), " %*s %f", &t);
					p_attributes.SetShady(t);
				}
				else if (type == "DrinkQuality")
				{
					float t = 0;
					sscanf_s(line.c_str(), " %*s %f", &t);
					p_attributes.SetDrinkQuality(t);
				}
				else if (type == "FoodQuality")
				{
					float t = 0;
					sscanf_s(line.c_str(), " %*s %f", &t);
					p_attributes.SetFoodQuality(t);
				}
			}

		}

		myfile.close();
	}
	else
	{
		std::cout << "Can't load Furniture Stats " << path << std::endl;
	}

}*/

std::string Furniture::WhatType()
{
	return "This should not be possible, earlier there was nothing here.";
}

void Furniture::Draw()
{
	if(MLP::GetInstance().IsReady(m_meshId))
		p_object.Draw();
}

Furniture * Furniture::MakeCopy()
{
	return nullptr;
}

std::string Furniture::getInfo(int index)
{
	MESH type = static_cast<MESH>(index);
	std::string str = "";

	switch (type)
	{
	case TABLE_LOW:
		str += "Simple Table\n";
		str += "+1 Simple\n";
		str += "$" + std::to_string(Table::getPrice(true));
		break;
	case TABLE_HIGH:
		str += "Fancy Table\n";
		str += "+1 Fancy\n";
		str += "$" + std::to_string(Table::getPrice(false));
		break;
	case BED_HIGH:
		str += "Fancy Bed\n";
		str += "+8 Fancy\n";
		str += "$" + std::to_string(Bed::getPrice(false));;
		break;
	case BED_LOW:
		str += "Simple Bed\n";
		str += "+8 Simple\n";
		str += "$" + std::to_string(Bed::getPrice(true));;
		break;
	case BAR_HIGH:
		str += "Fancy Bar\n";
		str += "+4 Fancy\n";
		str += "$" + std::to_string(Bar::getPrice(false));
		break;
	case BAR_LOW:
		str += "Simple Bar\n";
		str += "+1 Simple\n";
		str += "$" + std::to_string(Bar::getPrice(true));;
		break;
	case CHAIR_HIGH:
		str += "Fancy Chair\n";
		str += "+4 Fancy\n";
		str += "$" + std::to_string(Chair::getPrice(false));;
		break;
	case CHAIR_LOW:
		str += "Simple Chair\n";
		str += "+1 Simple\n";
		str += "$" + std::to_string(Bar::getPrice(true));;
		break;
	case STOVE:
		str += "Stove\n";
		str += "+1 Simple\n";
		str += "$100";
		break;
	case RECEPTION_HIGH:
		str += "Fancy Reception\n";
		str += "+4 Fancy\n";
		str += "$" + std::to_string(Bar::getPrice(false));;
		break;
	case RECEPTION_LOW:
		str += "Simple Reception\n";
		str += "+1 Simple\n";
		str += "$" + std::to_string(Bar::getPrice(true));;
		break;
	default:
		str = "";
		break;
	}


	return str;
}

int Furniture::getType() const
{
	return static_cast<int>(m_meshId);
}


