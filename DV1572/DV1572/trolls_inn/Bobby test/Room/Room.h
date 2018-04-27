#pragma once
#include <string>
#include <vector>
#include "../../../ourEngine/interface/Interface.h"
#include "Structure.h"
#include "../../../ourEngine/interface/light/PointLight.h"
#include "../../Furniture/Furniture.h"
#include "Node.h"
class Room
{
private:
	static Mesh s_AABB;
	static bool s_isLoaded;
	static int s_index;

	int	m_index;

	Object3D m_AABB;

	void _loadStatic();
	void _initAABB(int x, int y, int sx, int sy, int level = 0);
	void _createLight(int x, int y, int sx, int sy, int level = 0);

	//bool _findInVec(std::vector<RoomNode*>& list, RoomNode* node) const;
	bool _findInVec(std::vector<std::shared_ptr<Node>>& list, std::shared_ptr<Node> node) const;

protected:
	std::vector<PointLight> m_lights;
	std::vector<Tile*> m_roomTiles;
	Object3D m_wholeFloor;
	std::vector<Furniture> m_roomObjects;

	int		m_posX, m_posY;
	int		m_sizeX, m_sizeY;

	std::vector<Wall*> m_allWalls;

	std::vector<Wall*> m_upWalls;
	std::vector<Wall*> m_downWalls;
	std::vector<Wall*> m_rightWalls;
	std::vector<Wall*> m_leftWalls;

	int _index(int x, int y);
	bool m_isBuildingDoor = false;

public:
	Room(int posX = 0, int posY = 0, int sizeX = 0, int sizeY = 0, Mesh * m = nullptr);	
	Room(int posX, int posY , int sizeX, int sizeY, std::vector<Tile*> tiles);

	virtual ~Room();	

	//virtual std::vector<std::vector<Tile*>>		getTiles() const; 
	//virtual Tile*								getTiles(int x, int y) const;

	virtual void		setFloorMesh(Mesh * mesh);
	virtual void		CreateWallSide(Mesh* mesh, std::vector<bool> allowed, Direction side);
	virtual void		AddRoomObject(Furniture fut);
	virtual void		PickTiles();
	virtual void		PickWalls();
	
	std::vector<Tile*>	ReturnTiles();
	std::vector<Tile*>	getTiles();

	virtual int			getX() const; 
	virtual int			getY() const;
	XMINT2				getSize() const;
	virtual DirectX::XMFLOAT3	getPosition() const;

	virtual std::vector<Wall*>*	getAllWalls();
	virtual std::vector<Wall*>	getWalls(Direction dir);

	int getAmountOfObjects();
	int getAmountOfSpecificObjects(Furniture compare);
		
	virtual void		move(int x, int y);

	virtual bool		Inside(int x, int y); 
	virtual bool		Inside(Tile * t); 

	virtual void Update(Camera * cam);

	virtual void Draw() = 0;

	virtual std::string	toString() const = 0;
	int getRoomIndex() const;

	bool operator==(const Room& other) const;

	Tile* getTile(int x, int y);

	void ApplyIndexOnMesh();

	void CastShadow();
	float getDistance(Tile* t1, Tile* t2);
	std::vector<std::shared_ptr<Node>> findPath(Tile* startTile, Tile* endTile);

	void setIsBuildingDoor(bool tje);

};

