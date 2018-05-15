#pragma once
#include <unordered_map>
#include <future>
#include <vector>
#include "../../ourEngine/interface/shape/Mesh.h"
#include <string>
#include <mutex>

#define THREADED 1

enum MESH : short
{
	// Furnitures
	CHAIR,
	TABLE,
	BED_HIGH,
	BED_LOW,
	BAR_HIGH,
	BAR_LOW,
	CHAIR_HIGH,
	CHAIR_LOW,
	STOVE,
	RECEPTION_HIGH,
	RECEPTION_LOW,

	// Terrain meshes
	TREE_BILL,
	WATER,
	TERRAIN,
	SKYBOX
};

class MLP
{
private:
	struct MESH_THREAD
	{
		std::future<void> futureObj;
		std::string name;
	};
	std::unordered_map<std::string, Mesh> m_meshmap;
	std::vector<MESH_THREAD*> m_futureMeshes;
public:
	std::mutex mtx;
	std::mutex mtx2;
	static MLP& GetInstance()
	{
		static MLP instance;
		return instance;
	}
	MLP(MLP const&) = delete;
	void operator=(MLP const&) = delete;
	// Gets loaded mesh pointer
	// Path is set to trolls_inn/Resources/
	void LoadMesh(short id, std::string path);
	void LoadMesh(std::string name, std::string path);

	void LoadMeshInverted(short id, std::string path);
	void LoadMeshInverted(std::string name, std::string path);

	void LoadMeshRectangle(short id);
	void LoadMeshRectangle(std::string name);

	bool IsReady(short id);
	bool IsReady(std::string name);

	Mesh* GetMesh(short id);
	Mesh* GetMesh(const std::string path);

	void ClearMeshes();
private:
	MLP() {}
	
};