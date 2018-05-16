#include "OurEngine.h"
#include "LevelTracker.h"

int LevelTracker::_calculateExperienceToNextLevel() const
{
	return this->m_level * this->m_level * 10;
}

void LevelTracker::_setExperienceToNextLevel(int requiredExperience)
{
	this->m_experienceToNextLevel = requiredExperience;
}

void LevelTracker::_levelUp()
{
	this->m_level++;
	this->m_experience -= this->m_experienceToNextLevel;
	this->_setExperienceToNextLevel(this->_calculateExperienceToNextLevel());
}

LevelTracker::LevelTracker()
{
	this->m_experience = 0;
	this->m_level = 0;
	this->_levelUp();
}

LevelTracker::~LevelTracker()
{
}

int LevelTracker::GetExperience() const
{
	return this->m_experience;
}

int LevelTracker::GetLevel() const
{
	return this->m_level;
}

int LevelTracker::GetExperienceToNextLevel() const
{
	return this->m_experienceToNextLevel;
}

bool LevelTracker::IncreaseExperienceBy(int experienceGain)
{
	bool leveledUp = false;
	this->m_experience += experienceGain;
	
	while (this->m_experience > this->m_experienceToNextLevel)
	{
		leveledUp = true;
		this->_levelUp();
	}

	return leveledUp;
}

void LevelTracker::SetLevel(int newLevel)
{
	this->m_level = newLevel;
	this->_setExperienceToNextLevel(this->_calculateExperienceToNextLevel());
}