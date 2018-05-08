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

	enum Thoughts
	{
		ANGRY,
		THIRSTY,
		HUNGRY,
		TIRED
	};

private:
	struct Go
	{
		WalkDirection dir;
		float stepsLeft;
	};

	WalkDirection	m_currentDir;
	Billboard		m_model;
	int				m_modelSpriteIndex;

	Billboard		m_thoughtBubble;
	
	bool			m_displayThought;


	int				m_floor; //floor 0 is first floor 
	std::deque<Go>  m_goQueue;
	float			m_speed;

public:
	Character();
	Character(const Character& other);
	virtual ~Character();

	virtual void setModel(Mesh * model);
	
	virtual void Update();
	virtual void Move(WalkDirection dir);

	virtual void Turn(WalkDirection dir);

	virtual void setPosition(float x, float z);
	virtual void setFloor(int floor);
	virtual void setSpeed(float spd);

	void setThoughtBubble(Thoughts t);

	virtual void castShadow();

	virtual DirectX::XMFLOAT2 getPosition() const;

	int getModelSpriteIndex() const;
	void DisableThinkingEmjois();

	virtual WalkDirection getDirection() const;
	virtual XMFLOAT3 getDirection3f()const;
	virtual WalkDirection getDirectionFromPoint(XMFLOAT3 oldPos, XMFLOAT3 newPos) const;
	virtual int getFloor() const;
	virtual bool walkQueueDone() const;
	virtual void Draw();
	void setThoughtBubbleMesh(Mesh* bubbleMesh);
	virtual Shape* getShape();
};