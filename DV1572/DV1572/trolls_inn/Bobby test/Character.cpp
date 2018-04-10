#include "Character.h" 

Character::Character()
{
	m_floor = 0;
	m_speed = 2.0f / 60;
	m_currentDir = DOWN;
	m_model.setPos(0.5, 0.5, 0.5);
}

Character::~Character()
{
}

void Character::setModel(Mesh * model)
{
	m_model.setMesh(model);
}

void Character::Update()
{
	if (!m_goQueue.empty())
	{
		DirectX::XMFLOAT3 pos = m_model.getPosition();
		bool moving = true;

		Go dir = m_goQueue.front();

		if ((dir.dir == LEFT || dir.dir == RIGHT) && abs(dir.stepsLeft) <= 0.01f)
		{
			dir.stepsLeft = 0.0f;
			//m_model.setPos(pos.x + 0.5f, pos.y, pos.z); 
			printf("%f", pos.x);

			moving = false;
		}
		else if ((dir.dir == DOWN || dir.dir == UP) && abs(dir.stepsLeft) <= 0.01f)
		{
			dir.stepsLeft = 0.0f;
			//m_model.setPos(pos.x, pos.y, round(pos.z) + 0.5f); 
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
			case DOWN:
				m_model.Move(0.0f, 0.0f, -m_speed);
				break;
			case RIGHT:
				m_model.Move(m_speed, 0.0f, 0.0f);
				break;
			case LEFT:
				m_model.Move(-m_speed, 0.0f, 0.0f);
				break;
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
	case DOWN:
		m_model.setRotation(0.0f, 0.0f, 0.0f);
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
	m_model.setPos(x, 2.0f * m_floor, z);
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

const DirectX::XMFLOAT2 Character::getPosition() const
{
	DirectX::XMFLOAT3 p = m_model.getPosition();
	DirectX::XMFLOAT2 position;
	position.x = p.x;
	position.y = p.z;
	return position;
}

Character::WalkDirection Character::getDirection() const
{
	return m_currentDir;
}

int Character::getFloor() const
{
	return m_floor;
}

void Character::Draw()
{
	m_model.Draw();
}