#pragma once

class LevelTracker
{
private:
	int experience;
	int level;
	int experienceToNextLevel;

	int calculateExperienceToNextLevel() const;
	void setExperienceToNextLevel(int requiredExperience);

	void levelUp();

public:
	LevelTracker();
	~LevelTracker();

	int getExperience() const;
	int getLevel() const;
	int getExperienceToNextLevel() const;

	bool increaseExperienceBy(int experienceGain);
	void setLevel(int newLevel);
};