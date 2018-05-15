#include "MeshLoaderPlus.h"
#include <thread>
#include <chrono>

void MLP::LoadMesh(short id, std::string path)
{
	LoadMesh(std::to_string(id), path);
}

void MLP::LoadMesh(std::string name, std::string path)
{
	if (m_meshmap.find(name) == m_meshmap.end())
	{
#if THREADED
		MESH_THREAD* mt = new MESH_THREAD;
		mt->futureObj = std::async(std::launch::async, &Mesh::LoadModelStr, &m_meshmap[name], "trolls_inn/Resources/" + path);
		mt->name = name;
		m_futureMeshes.push_back(mt);
#else
		m_meshmap[name].LoadModelStr("trolls_inn/Resources/" + path);
#endif

	}
	else
		throw "Mesh with the same name exists!";
}

void MLP::LoadMeshInverted(short id, std::string path)
{
	LoadMeshInverted(std::to_string(id), path);
}

void MLP::LoadMeshInverted(std::string name, std::string path)
{
	if (m_meshmap.find(name) == m_meshmap.end())
	{
#if THREADED
		MESH_THREAD* mt = new MESH_THREAD;
		mt->futureObj = std::async(std::launch::async, &Mesh::LoadModelInverted, &m_meshmap[name], "trolls_inn/Resources/" + path);
		mt->name = name;
		m_futureMeshes.push_back(mt);
#else
		m_meshmap[name].LoadModelInverted("trolls_inn/Resources/" + path);
#endif
	}
	else
		throw "Mesh with the same name exists";
}

void MLP::LoadMeshRectangle(short id)
{
	LoadMeshRectangle(std::to_string(id));
}

void MLP::LoadMeshRectangle(std::string name)
{
	if (m_meshmap.find(name) == m_meshmap.end())
	{

		m_meshmap[name].MakeRectangle();
	
	}
	else
		throw "Mesh exist with the same name!";
}

bool MLP::IsReady(short id)
{
	return IsReady(std::to_string(id));
}

bool MLP::IsReady(std::string name)
{
#if THREADED
	using namespace std::chrono_literals;
	//If the mesh is not located in the vector, the mesh had already been loaded. not safe
	for (int i = 0; i < m_futureMeshes.size(); i++)
	{
		auto status = m_futureMeshes[i]->futureObj.wait_for(0ms);
		if (status == std::future_status::ready)
		{
			delete m_futureMeshes[i];
			m_futureMeshes.erase(m_futureMeshes.begin() + i);
		}
		else if(name == m_futureMeshes[i]->name)
		{
			return false; // This mesh wasnt ready
		}
		
	}
#endif
	return true;
}

Mesh * MLP::GetMesh(short id)
{
	return GetMesh(std::to_string(id));
}

Mesh * MLP::GetMesh(std::string name)
{
	if (m_meshmap.find(name) != m_meshmap.end())
		return &m_meshmap[name];
	else
		throw "Requested Mesh that doesnt exist!";
}

void MLP::ClearMeshes()
{
	m_meshmap.clear();
	//if (m_futureMeshes.size())
	//	// This is not gut! We are loading models that we are not using!
	for (auto& fut : m_futureMeshes)
	{
		delete fut;
	}

}
