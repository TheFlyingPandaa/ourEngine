#include "Staff.h"

Staff::Staff() : Character()
{
	m_trollMesh.LoadModel("trolls_inn/Resources/SpritesheetSample/troll.obj"); 
	m_level.SetLevel(1);
	this->setModel(&m_trollMesh);
}

Staff::Staff(Professions profession)
{
	this->m_profession = profession;
}

Staff::Staff(Professions profession, int level)
{
	this->m_profession = profession;
	this->m_level.SetLevel(level);
}

Staff::~Staff()
{
}

Professions Staff::GetProfession() const
{
	return this->m_profession;
}

LevelTracker& Staff::GetLevelTracker()
{
	return this->m_level;
}