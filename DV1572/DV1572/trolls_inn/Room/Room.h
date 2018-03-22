#pragma once
#include <string>

enum TileType
{
	EMPTY,
	MODULE,
	DOOR
};

struct Point
{
	int x;
	int y;
	Point(int x = 0, int y = 0)
	{
		this->x = x;
		this->y = y;
	}
	void abs()
	{
		x = (x > 0) ? x : -x;
		y = (y > 0) ? y : -y;
	}
	Point operator-(const Point& other) const
	{
		return Point(x - other.x, y - other.y);
	}
	Point operator/(int div) const
	{
		return Point(x / div, y / div);
	}
	Point operator+(const Point& other) const
	{
		return Point(x + other.x, y + other.y);
	}

};

class Room;

struct Tile
{
	Tile()
	{
		type = EMPTY;
		x = -1;
		y = -1;
		myRoom = nullptr;
	}
	TileType type;
	int x, y;
	Room* myRoom;
	std::string toString()
	{
		switch (type)
		{
		case EMPTY:
			return "0";
			break;
		case MODULE:
			return "X";
			break;
		case DOOR:
			return "D";
			break;
		}

	}
};

class Room
{
private:
	Tile* m_tiles;
	Point* m_corners;
	int m_width;
	int m_height;
public:
	Room(int xPos, int yPos, int width, int height);
	
	Point getPosition() const;
	int getWidth() const;
	int getHeight() const;

	virtual ~Room();
	
	Tile* getTiles() const;
	Point* getCorners() const;
};
