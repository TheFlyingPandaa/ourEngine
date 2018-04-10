#pragma once
#pragma once 
#include "../../ourEngine/interface/Interface.h" 
#include <deque> 
#include <math.h> 

class Character
{
public:
	enum WalkDirection
	{
		UP,
		UPRIGHT,
		UPLEFT,
		DOWN,
		DOWNRIGHT,
		DOWNLEFT,
		LEFT,
		RIGHT
	};

private:
	struct Go
	{
		WalkDirection dir;
		float stepsLeft;
	};

	Object3D		m_model;
	int				m_floor; //floor 0 is first floor 
	std::deque<Go>  m_goQueue;
	float			m_speed;
	WalkDirection	m_currentDir;

public:
	Character();
	virtual ~Character();
	virtual void setModel(Mesh * model);
	virtual void Update();
	virtual void Move(WalkDirection dir);
	virtual void Turn(WalkDirection dir);
	virtual void setPosition(float x, float z);
	virtual void setFloor(int floor);
	virtual void setSpeed(float spd);

	virtual const DirectX::XMFLOAT2 getPosition() const;
	virtual WalkDirection getDirection() const;
	virtual WalkDirection getDirectionFromPoint(XMFLOAT3 oldPos, XMFLOAT3 newPos) const;
	virtual int getFloor() const;
	virtual bool walkQueueDone() const;
	virtual void Draw();
};