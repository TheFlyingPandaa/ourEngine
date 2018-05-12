#include "MeshLoaderPlus.h"
#include <thread>
#include <chrono>
void MLP::LoadMesh(std::string name, std::string path)
{
	if (m_meshmap[name].IsLoaded() == false)
	{
	//	mtx2.lock();
		m_meshmap[name].LoadModel("trolls_inn/Resources/" + path);
		//m_futureMeshes.push_back(std::async(std::launch::async, &Mesh::LoadModelStr, &m_meshmap[name], "trolls_inn/Resources/"+ path));
	//	mtx2.unlock();
	}
	else
	{
		__debugbreak();
	}
	OutputDebugString("NUMBER OF MESHES " + m_meshmap.size());
}

void MLP::LoadMeshInverted(std::string name, std::string path)
{
	if (m_meshmap[name].IsLoaded() == false)
	{
		m_meshmap[name].LoadModelInverted("trolls_inn/Resources/" + path);
	
	}
}

void MLP::LoadMeshRectangle(std::string name)
{
	if (m_meshmap.find(name) != m_meshmap.end())
		__debugbreak();
	if (m_meshmap[name].IsLoaded() == false)
	{
		m_meshmap[name].MakeRectangle();
		//m_futureMeshes.push_back(std::async(std::launch::async, &Mesh::MakeRectangle, &m_meshmap[name]));
	
	}
	else
		__debugbreak();


}

Mesh * MLP::getMesh(std::string name)
{
	if (m_meshmap.find(name) == m_meshmap.end())
	{
		/*__debugbreak();*/
		return &m_meshmap["NULL"];
	}
	else
	{
		while (!m_meshmap[name].IsLoaded());
		return &m_meshmap[name];
	}
}

void MLP::ClearMeshes()
{
	m_meshmap.clear();
}
