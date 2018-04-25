#include "Furniture.h"
#include <fstream>
#include <sstream>
#include <iostream>

Furniture::Furniture(DirectX::XMFLOAT3 pos, Mesh * mesh)
{
	p_rot = 0;
	this->pos.x = pos.x;
	this->pos.y = pos.y;
	this->pos.z = pos.z;
}

Furniture::~Furniture()
{
}

Object3D Furniture::getObject3D()
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

DirectX::XMINT3 Furniture::getPosition()
{
	return this->pos;
}

int Furniture::getRotation()
{
	return p_rot;
}

void Furniture::LoadFurnitureStats(std::string path)
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

}

void Furniture::Draw()
{
	p_object.Draw();
}


