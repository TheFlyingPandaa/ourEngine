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
	DirectX::XMINT3 getPosition();

	int				getPrice();

	bool			getIsBusy();
	void			setIsBusy(bool busy);

	int				getLevel();
	void			setLevel(int amount);
	void			AddLevel(int amount = 1);
	//void LoadFurnitureStats(std::string path);

	virtual std::string WhatType();

	virtual void Draw();

	virtual Furniture * MakeCopy();

private:
	DirectX::XMINT3 pos;
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
