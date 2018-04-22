#pragma once
#include <string>
#include <vector>
#include "../../../ourEngine/interface/Interface.h"
#include "Structure.h"
#include "../../../ourEngine/interface/light/PointLight.h"

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

protected:
	std::vector<PointLight> m_lights;
	std::vector<Tile*> m_roomTiles;
	std::vector<Object3D> m_roomObjects;

	int		m_posX, m_posY;
	int		m_sizeX, m_sizeY;

	bool	m_culledWalls[4] = { false };

	std::vector<Wall*> m_allWalls;
	//TODO
	std::vector<Wall*> up;
	std::vector<Wall*> down;
	std::vector<Wall*> left;
	std::vector<Wall*> right;


public:
	Room(int posX = 0, int posY = 0, int sizeX = 0, int sizeY = 0, Mesh * m = nullptr);	
	Room(int posX, int posY , int sizeX, int sizeY, std::vector<Tile*> tiles);

	virtual ~Room();	

	//virtual std::vector<std::vector<Tile*>>		getTiles() const; 
	//virtual Tile*								getTiles(int x, int y) const;

	virtual void		setFloorMesh(Mesh * mesh);
	virtual void		CreateWalls(Mesh* mesh, std::vector<bool> sides);
	
	virtual int			getX() const; 
	virtual int			getY() const;
	virtual int			getSizeX() const; 
	virtual int			getSizeY() const; 

	virtual void		setWalls(std::vector<Wall*> walls, Direction dir);
	virtual void		addWall(Wall* wall, Direction dir);
	virtual DirectX::XMFLOAT3	getPosition() const;

	virtual std::vector<Wall*>*	getAllWalls();
	virtual std::vector<Wall*>*	getWall(Direction dir);
		
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

