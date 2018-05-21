#pragma once
#include "../../ourEngine/interface/shape/3DObject.h"
#include "../AI_Base/Attributes.h"
#include "../AI_Base/Customer.h"
#include "../Mesh Manager/MeshLoaderPlus.h"
class Furniture
{
public:
	Furniture(DirectX::XMFLOAT3 pos, MESH mesh);
	virtual ~Furniture();

	Object3D&	getObject3D();

	Attributes&	getAttributes();

	int	getGridSize(); //gets the size this furniture will take up
	
	void			setRotation(int rot);
	int				getRotation();
	void			setLightIndex(int index);
	void			setPosition(DirectX::XMFLOAT3 pos);
	void			setPosition(float x, float y, float z);
	virtual	 DirectX::XMINT3 getPosition();
	
	Customer*		getOwner() const; 

	int				getPrice();

	bool			getIsBusy();
	void			setIsBusy(bool busy);

	int				getLevel();
	void			setLevel(int amount);
	int				AddLevel(int gold,int amount = 1);
	void			setIndex(int index); 
	void			setOwner(Customer* owner); 
	void			increaseDirtyLevel(); 
	void			cleanFurniture(); 

	int				getPriceToLevelUp() const;
	int				getDirtyStat() const; 

	int				getIndex() const; 

	void			releaseOwnerShip(); 
	//void LoadFurnitureStats(std::string path);

	virtual std::string WhatType();

	virtual void Draw();

	virtual Furniture * MakeCopy();

	static std::string getInfo(int index);
	int getType() const;


private:
	DirectX::XMINT3 pos;
	int m_index; 
	Customer* m_owner; 
	int m_dirtyStat; 

protected:
	Object3D	p_object;
	Attributes	p_attributes;
	int			p_gridSize;
	int			p_rot;
	int			p_price;

	bool		p_isBusy;

	int			p_level;
	MESH		m_meshId;
};
