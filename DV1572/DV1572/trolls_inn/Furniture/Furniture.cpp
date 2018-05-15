#include "Furniture.h"
#include <fstream>
#include <sstream>
#include <iostream>

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
	//p_object.setRotation(0, rot * (-90), 0);
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

void Furniture::AddLevel(int amount)
{
	p_level += amount;
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


