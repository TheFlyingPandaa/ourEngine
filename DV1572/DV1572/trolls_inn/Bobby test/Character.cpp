#include "Character.h" 
#include "../../ourEngine/core/Dx.h"

Character::Character()
{
	m_floor = 0;
	m_speed = 2.0f / 60;
	setPosition(16.0f, 8.2f);
	m_currentDir = DOWN;
	m_modelSpriteIndex = 0;
	m_displayThought = false;
}

Character::Character(const Character & other)
{
	this->m_currentDir = other.m_currentDir;
	this->m_model = other.m_model;
	this->m_floor = other.m_floor;
	this->m_goQueue = other.m_goQueue;
	this->m_speed = other.m_speed;
}

Character::~Character()
{
}

void Character::setModel(Mesh * model)
{
	m_model.setMesh(model);
}
const char* printDir(Character::WalkDirection dir)
{
	switch (dir)
	{
	case Character::UP:
		return "Up";
	case Character::DOWN:
		return "Down";
	case Character::RIGHT:
		return "Right";
	case Character::LEFT:
		return "Left";
	}
	return "No movement";

}
#include <iostream>
void Character::Update()
{
	if (!m_goQueue.empty())
	{
		DirectX::XMFLOAT3 pos = m_model.getPosition();
		bool moving = true;

		Go dir = m_goQueue.front();

		if (abs(dir.stepsLeft) <= 0.01f)
		{
			dir.stepsLeft = 0.0f;
			//m_model.setPos(pos.x + 0.5f, pos.y, pos.z); 
			//printf("%f", pos.x);

			moving = false;
		}
		
		if (!moving)
			m_goQueue.pop_front();
		else
		{
			
			dir.stepsLeft -= m_speed;
			m_goQueue[0] = dir;
			if (dir.dir != m_currentDir)
			{
				this->Turn(dir.dir);
			}
			switch (dir.dir)
			{
			case UP:
				m_model.Move(0.0f, 0.0f, m_speed);
				break;
			case UPRIGHT:
				m_model.Move(m_speed, 0.0f, m_speed);
				break;
			case UPLEFT:
				m_model.Move(-m_speed, 0.0f, m_speed);
				break;
			case DOWN:
				m_model.Move(0.0f, 0.0f, -m_speed);
				break;
			case DOWNRIGHT:
				m_model.Move(m_speed, 0.0f, -m_speed);
				break;
			case DOWNLEFT:
				m_model.Move(-m_speed, 0.0f, -m_speed);
				break;
			case RIGHT:
				m_model.Move(m_speed, 0.0f, 0.0f);
				break;
			case LEFT:
				m_model.Move(-m_speed, 0.0f, 0.0f);
				break;
			}
			//std::cout << printDir(getDirection()) << std::endl;
			XMFLOAT3 playerPos = m_model.getPosition();
			playerPos.y += 1.5f;
			m_thoughtBubble.setPos(playerPos);


			static float indexLol = 0.01f;
			m_modelSpriteIndex = (int)m_modelSpriteIndex % 4;
			indexLol += 0.04f;
			if (indexLol >= 1)
			{
				m_modelSpriteIndex += indexLol;
				indexLol = 0.0f;
			}
			
		}
	}
}

void Character::Move(WalkDirection dir)
{
	Go step;
	step.stepsLeft = 1.0f;
	step.dir = dir;
	m_goQueue.push_back(step);
}

void Character::Turn(WalkDirection dir)
{
	m_currentDir = dir;
	switch (dir)
	{
	case UP:
		m_model.setRotation(0.0f, 180.0f, 0.0f);
		break;
	case UPRIGHT:
		m_model.setRotation(0.0f, 60.0f, 0.0f);
		break;
	case UPLEFT:
		m_model.setRotation(0.0f, 240.0f, 0.0f);
		break;
	case DOWN:
		m_model.setRotation(0.0f, 0.0f, 0.0f);
		break;
	case DOWNRIGHT:
		m_model.setRotation(0.0f, -60.0f, 0.0f);
		break;
	case DOWNLEFT:
		m_model.setRotation(0.0f, -150.0f, 0.0f);
		break;
	case RIGHT:
		m_model.setRotation(0.0f, -90.0f, 0.0f);
		break;
	case LEFT:
		m_model.setRotation(0.0f, 90.0f, 0.0f);
		break;

	}
}

void Character::setPosition(float x, float z)
{
	m_model.setPos(x, 0.5f, z);
}

void Character::setFloor(int floor)
{
	m_floor = floor;
	DirectX::XMFLOAT3 p = m_model.getPosition();
	m_model.setPos(p.x, 2 * floor + 0.5f, p.z);
}

void Character::setSpeed(float spd)
{
	m_speed = spd;
}

void Character::setThoughtBubble(Thoughts t)
{
	m_displayThought = true;
	switch (t)
	{
	case ANGRY:
		m_thoughtBubble.setSpriteIndex(3);
		return;
	case TIRED:
		m_thoughtBubble.setSpriteIndex(2);
		return;
	case HUNGRY:
		m_thoughtBubble.setSpriteIndex(1);
		return;
	case THIRSTY:
		m_thoughtBubble.setSpriteIndex(0);
		return;
	}
}

void Character::castShadow()
{
	DX::submitToInstance(&m_model, DX::g_InstanceGroupsShadow);
}

 DirectX::XMFLOAT2 Character::getPosition() const
{
	DirectX::XMFLOAT3 p = m_model.getPosition();
	DirectX::XMFLOAT2 position;
	position.x = p.x;
	position.y = p.z;
	return position;
}

int Character::getModelSpriteIndex() const
{
	return m_modelSpriteIndex;
}

void Character::DisableThinkingEmjois()
{
	m_displayThought = false;
}


Character::WalkDirection Character::getDirection() const
{
	return m_currentDir;
}
XMFLOAT3 Character::getDirection3f() const
{
	switch (m_currentDir)
	{
	case UP:
	case UPRIGHT:
	case UPLEFT:
		return XMFLOAT3(0, 0, -1);
	case RIGHT:
		return XMFLOAT3(1, 0, 0);
	case LEFT:
		return XMFLOAT3(-1, 0, 0);
	case DOWN:
	case DOWNRIGHT:
	case DOWNLEFT:
		return XMFLOAT3(0, 0, 1);
	}
	return XMFLOAT3(0,0,0);
}

Character::WalkDirection Character::getDirectionFromPoint(XMFLOAT3 oldPos, XMFLOAT3 newPos) const
{

	XMVECTOR oldPosWithoutOffset = XMLoadFloat3(&oldPos);
	XMVECTOR xmNewPos = XMLoadFloat3(&newPos);
	XMVECTOR xmDeltaPos = xmNewPos - oldPosWithoutOffset;
	XMFLOAT3 result;
	XMStoreFloat3(&result, xmDeltaPos);
	WalkDirection dir;
	
	if (result.x > 0) 
	{
		dir = RIGHT;
		if (result.z > 0)
			dir = UPRIGHT;
		else if (result.z < 0)
			dir = DOWNRIGHT;
	}
	else if (result.x < 0)
	{
		dir = LEFT;
		if (result.z > 0)
			dir = UPLEFT;
		else if (result.z < 0)
			dir = DOWNLEFT;
	}
	else if (result.z > 0) dir = UP;
	else if (result.z < 0) dir = DOWN;


	return dir;
}

int Character::getFloor() const
{
	return m_floor;
}

bool Character::walkQueueDone() const
{
	return m_goQueue.size() == 0;
}

void Character::Draw()
{
	DX::submitToInstance(this);

	if(m_displayThought)
		DX::submitToInstance(&m_thoughtBubble);
}

void Character::setThoughtBubbleMesh(Mesh * bubbleMesh)
{
	m_thoughtBubble.setMesh(bubbleMesh);
}

Shape * Character::getShape()
{
	return &m_model;
}
