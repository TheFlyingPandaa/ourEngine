#pragma once
#include "Reward.h"

class Events
{
private:
	bool m_exitState = false;
protected:
	//<Need to have>
	//Reward reward
	//QuestGiver
	//Timer
	//</Need to have>
	Reward p_reward;

	
public:
	Events();
	virtual ~Events();

	virtual void Update();

	virtual bool exitState() { return m_exitState; }
	virtual void setExitState(bool setExit) { m_exitState = setExit; }

	Reward getReward();
};
