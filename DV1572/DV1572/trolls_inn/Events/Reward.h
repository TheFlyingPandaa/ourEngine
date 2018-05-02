#pragma once
class Reward
{
private:
	int m_goldReward;

	//Stats reward
	//upgradeReward?
	//Whatever reward
public:
	Reward();
	~Reward();

	//<Setters and getters>
	int getGoldReward() const;
	void setGoldReward(const int reward);
};
