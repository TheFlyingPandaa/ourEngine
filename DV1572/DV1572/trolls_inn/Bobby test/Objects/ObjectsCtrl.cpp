#include "ObjectsCtrl.h"



ObjectsCtrl::ObjectsCtrl()
{
	m_mesh = std::vector<Mesh*>();
	m_objects = std::vector<Object3D*>();
}


ObjectsCtrl::~ObjectsCtrl()
{
	for (int i = 0; i < m_objects.size(); i++)
	{
		delete m_objects[i];
	}
	m_objects.clear();
}

int ObjectsCtrl::addObject(Mesh * mesh)
{
	if (std::find(m_mesh.begin(), m_mesh.end(), mesh) == m_mesh.end())
	{
		this->m_mesh.push_back(mesh);
		return static_cast<int>(m_mesh.size() - 1);
	}
	else
	{
		return static_cast<int>(std::find(m_mesh.begin(), m_mesh.end(), mesh) - m_mesh.begin());			
	}
}

void ObjectsCtrl::PlaceObject(Tile * tile, int index)
{
	Object3D * obj = new Object3D();
	obj->setMesh(m_mesh[index]);
	DirectX::XMFLOAT3 pos = DirectX::XMFLOAT3(tile->getQuad().getPosition().x + OFFSET, 0.0f, tile->getQuad().getPosition().z + OFFSET);
	obj->setPos(pos);
	tile->setIsWalkeble(false);
	m_objects.push_back(obj);
}

void ObjectsCtrl::draw()
{
	for (int i = 0; i < m_objects.size(); i++)
	{
		m_objects[i]->Draw();
	}
}


