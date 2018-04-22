#pragma once
#include "../../../ourEngine/interface/Interface.h"

enum Direction {
	up,
	down,
	left,
	right,
	upright, // SPECIAL CASES
	upleft,
	downright,
	downleft,
	noneSpecial
};

class Wall
{
private:
	Object3D			m_wall;
	XMFLOAT2			m_direction;
	bool				m_sharedWall;
public:
	Wall(Mesh * mesh, XMFLOAT2 dir);
	~Wall();

	void	Draw();

	void	setPosition(float x, float y);
	void	setPosition(DirectX::XMFLOAT2 position);
	void	setRotation(float x, float y, float z);
	void	setRotation(DirectX::XMFLOAT3 rotation);
	void	setScale(float x, float y, float z);

	void	setMesh(Mesh * mesh);
	void	setIsShared(bool shared);
	bool	isShared() const;

	XMFLOAT2 getDirection() const;
	bool	operator==(const Wall& other);

	Object3D& getObject3D()
	{
		return m_wall;
	}
};

class Tile {

private:
	Object3D quad;
public:
	Tile(int sizeX = 0, int sizeY = 0, Mesh * mesh = nullptr)
	{
		quad.setMesh(mesh);
		quad.setPos(static_cast<float>(sizeX), 0.0f, static_cast<float>(sizeY));
		quad.setRotation(90.0f, 0.0f, 0.0f);		
	}

	int	getPosX() const
	{
		return quad.getPosition().x;
	}
	int		getPosY() const
	{
		return quad.getPosition().z;
	}

	void	setPosX(const int x);
	void	setPosY(const int x);

	Object3D&	getQuad()
	{
		return quad;
	}
	void	setMesh(Mesh * mesh);

	XMFLOAT2 getPosition() const;
	bool operator==(const Tile& other) const
	{
		XMFLOAT3 pos = quad.getPosition();
		XMFLOAT3 otherPos = other.quad.getPosition();

		return pos.x == otherPos.x && pos.y == otherPos.y && pos.z == otherPos.z;

	}
	bool operator==(const XMINT2& searchingPos) const
	{
		XMFLOAT3 pos = quad.getPosition();
		XMFLOAT3 lookingPosition = { float(searchingPos.x), 0.0f, float(searchingPos.y) };

		return pos.x == lookingPosition.x && pos.z == lookingPosition.z;
	}

};
