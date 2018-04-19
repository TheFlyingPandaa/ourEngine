#pragma once
#include <string>
#include <vector>
#include "../../../ourEngine/interface/Interface.h"
#include "Structure.h"
#include "../../../ourEngine/interface/light/PointLight.h"

class Room
{
public:
	struct RoomConnection
	{
		Room* otherRoom; // if nullptr, then the connection is to the outside
		XMINT2 connectingDoor;
		XMINT2 direction;

		bool operator==(const Room::RoomConnection& other)
		{
			return *otherRoom == *other.otherRoom;
		}
		bool operator==(const Room& other)
		{
			return *otherRoom == other;
		}
		bool operator==(const Room* other)
		{
			if (other == nullptr)
			{
				if(otherRoom == nullptr)
					return true;
				return false;
			}
			if (otherRoom == nullptr)
				return false;

			return *otherRoom == *other;
		}
	};

private:
	friend class RoomCtrl;
	
private:
	static Mesh s_AABB;
	static bool s_isLoaded;
	static int s_index;
	int	m_index;
	Object3D m_AABB;
	Object3D m_AABB2;
	void _loadStatic();
	void _initAABB(int x, int y, int sx, int sy, int level = 0);
	void _createLight(int x, int y, int sx, int sy, int level = 0);
protected:
	std::vector<PointLight> m_lights;


	int		m_posX, m_posY;
	int		m_sizeX, m_sizeY;

	std::vector<std::vector<Tile*>> m_tiles;

	bool	m_culledWalls[4] = { false };

	std::vector<Wall*> m_allWalls;
	//TODO
	std::vector<Wall*> up;
	std::vector<Wall*> down;
	std::vector<Wall*> left;
	std::vector<Wall*> right;

	std::vector<Room*> adjasentRooms;
	
	std::vector<RoomConnection> adjasentRoomDoors;
	
	bool				m_hasDoor[4]{ false };

public:
	Room(int posX = 0, int posY = 0, int sizeX = 0, int sizeY = 0, Mesh * m = nullptr);	
	Room(int posX, int posY , int sizeX, int sizeY, std::vector<std::vector<Tile*>> tiles);

	virtual ~Room();	

	virtual std::vector<std::vector<Tile*>>		getTiles() const; 
	virtual Tile*								getTiles(int x, int y) const;

	virtual void		setTile(Mesh * mesh);
	
	virtual int			getX() const; 
	virtual int			getY() const;
	virtual int			getSizeX() const; 
	virtual int			getSizeY() const; 
	virtual void		setWalls(std::vector<Wall*> walls, Direction dir);
	virtual void		addWall(Wall* wall, Direction dir);

	bool hasConnectedRooms() const;

	virtual DirectX::XMFLOAT3	getPosition() const;

	virtual void		addAdjasentRoomDoor(Room * room, XMINT2 doorPos, XMINT2 direction);
	virtual void		addAdjasentRoom(Room * room);

	virtual XMINT2 getConnectingRoomDoorPositionPartOne(Room* otherroom);
	virtual XMINT2 getConnectingRoomDoorPositionPartTwo(Room* otherroom);

	virtual std::vector<Room*>*	getAdjasent();
	virtual std::vector<Wall*>*	getAllWalls();
	virtual std::vector<Wall*>*	getWall(Direction dir);
	

	virtual void		setHasDoor(Direction, bool);
	virtual bool		getHasDoor(Direction) const;

	virtual void		move(int x, int y);

	virtual bool		Inside(int x, int y); 
	virtual bool		Inside(Tile * t); 

	virtual void Update(Camera * cam);
	virtual void Draw() = 0;
	virtual std::string	toString() const = 0;
	int getRoomIndex() const;

	bool operator==(const Room& other) const;

	void ApplyIndexOnMesh();

	void CastShadow();

};

