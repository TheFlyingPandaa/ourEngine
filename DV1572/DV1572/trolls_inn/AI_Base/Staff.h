#pragma once
#include "LevelTracker.h"
#include "../Bobby test/Character.h"
#include <DirectXMath.h>
#include "../Furniture/Furniture.h"

enum Professions
{
	Maid,
	Assassin,
	Thief,
	Cook,
	Bartender,
	Guard
};

class Staff : public Character
{
private:
	LevelTracker m_level;
	Professions m_profession;
	DirectX::XMFLOAT2 m_position; 
	Mesh m_trollMesh; 
	bool m_cancelFlag;
	
	// This code sucks(Henrik)
	Furniture* m_fur;
	bool m_isCleaning;
	
public:
	Staff();
	Staff(Professions profession);
	Staff(Professions profession, int level);
	~Staff();

	Professions GetProfession() const;
	LevelTracker& GetLevelTracker();

	//For chase
	void setCancelFlag(bool flag);
	bool getCancelFlag() const;

	// For cleaning
	void setCleaning(bool flag, Furniture* furToBeClean);
	bool isCleaning() const;
	Furniture* getCleaningFurniture() const;
	void cleanFurniture();

};