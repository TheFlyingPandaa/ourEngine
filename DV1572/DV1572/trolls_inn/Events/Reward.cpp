#include "OurEngine.h"
#include "Reward.h"

Reward::Reward()
{
	m_goldReward = 0;
}

Reward::~Reward()
{
}

int Reward::getGoldReward() const
{
	return m_goldReward;
}

void Reward::setGoldReward(const int reward)
{
	m_goldReward = reward;
}
