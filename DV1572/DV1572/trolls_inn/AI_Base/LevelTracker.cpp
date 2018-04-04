#include "LevelTracker.h"

int LevelTracker::calculateExperienceToNextLevel() const
{
	return this->level * this->level * 10;
}

void LevelTracker::setExperienceToNextLevel(int requiredExperience)
{
	this->experienceToNextLevel = requiredExperience;
}

void LevelTracker::levelUp()
{
	this->level++;
	this->experience -= this->experienceToNextLevel;
	this->setExperienceToNextLevel(this->calculateExperienceToNextLevel());
}

LevelTracker::LevelTracker()
{
	this->experience = 0;
	this->level = 0;
	this->levelUp();
}

LevelTracker::~LevelTracker()
{
}

int LevelTracker::getExperience() const
{
	return this->experience;
}

int LevelTracker::getLevel() const
{
	return this->level;
}

int LevelTracker::getExperienceToNextLevel() const
{
	return this->experienceToNextLevel;
}

bool LevelTracker::increaseExperienceBy(int experienceGain)
{
	bool leveledUp = false;
	this->experience += experienceGain;
	
	while (this->experience > this->experienceToNextLevel)
	{
		leveledUp = true;
		this->levelUp();
	}

	return leveledUp;
}

void LevelTracker::setLevel(int newLevel)
{
	this->level = newLevel;
	this->setExperienceToNextLevel(this->calculateExperienceToNextLevel());
}