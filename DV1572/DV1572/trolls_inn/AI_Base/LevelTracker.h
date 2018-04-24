#pragma once

class LevelTracker
{
private:
	int m_experience;
	int m_level;
	int m_experienceToNextLevel;

	int _calculateExperienceToNextLevel() const;
	void _setExperienceToNextLevel(int requiredExperience);

	void _levelUp();

public:
	LevelTracker();
	~LevelTracker();

	int GetExperience() const;
	int GetLevel() const;
	int GetExperienceToNextLevel() const;

	bool IncreaseExperienceBy(int experienceGain);
	void SetLevel(int newLevel);
};