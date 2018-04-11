#pragma once
#include "../../../ourEngine/interface/Interface.h"
#include "../Room/Structure.h"

const float OFFSET = 0.5f;
class ObjectsCtrl
{
private:
	std::vector<Object3D*>	m_objects;
	std::vector<Mesh*>		m_mesh;
public:
	ObjectsCtrl();
	~ObjectsCtrl();
	
	int		addObject(Mesh * mesh);
	void	PlaceObject(Tile * tile, int index);
	void	draw();	
};

