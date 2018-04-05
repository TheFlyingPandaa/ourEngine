#include "../ourEngine/interface/Interface.h"
#include <iostream>
#include <chrono>
#include <thread>

const float REFRESH_RATE = 60.0f;

inline void _loadModelThread(Mesh * a, const std::string path)
{
	a->LoadModel(path);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{



	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//	Activation of Console
	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);

	Window wnd(hInstance);
	wnd.Init(1280, 720, "Banan");

	using namespace std::chrono;
	auto time = steady_clock::now();
	auto timer = steady_clock::now();
	int updates = 0;
	int fpsCounter = 0;
	float freq = 1000000000.0f / REFRESH_RATE;
	float unprocessed = 0;

	Camera* cam = new OrbitCamera(wnd.getSize());



	Mesh wall;
	wall.LoadModel("trolls_inn/Resources/Box.obj");
	wall.setNormalTexture("trolls_inn/Resources/NormalMap.jpg");
	wall.setDiffuseTexture("trolls_inn/Resources/wood.jpg");
	const int numberOfWalls = 10;
	Object3D aWall[numberOfWalls];

	Object3D box[100];
	for (int i = 0; i < 100; i++)
	{
		box[i].setMesh(&wall);
		box[i].setPos(i * 0.1, 0.1 * i, -0.2 * i);
	}
	

	int counter = 0;
	for(auto& cWall : aWall)
	{
		cWall.setMesh(&wall);
		cWall.setPos(0.0f, counter++, 0.0f);

	}
	cam->setPosition(0, 0, 2);
	cam->update();
	
	Light light;
	light.Init(XMFLOAT4A{ 0,5,0,1 }, XMFLOAT4A{ 0.0f, -1.0f,0.0f,0.0f }, XMFLOAT4A{ 1.0f,1.0f,1.0f,0.0f }, 1280, 720, cam);
	while (wnd.isOpen())
	{
		wnd.Clear();
		auto currentTime = steady_clock::now();
		wnd.PollEvents();
		auto dt = duration_cast<nanoseconds>(currentTime - time).count();
		time = steady_clock::now();

		unprocessed += dt / freq;

		while (unprocessed > 1)
		{
			updates++;
			unprocessed -= 1;
			
			cam->update();
			
		}

		fpsCounter++;

		/*for(auto& wall:aWall)
			wall.Draw();
	*/
		//box.Draw();
		for (int i = 0; i < 100; i++)
		{
			box[i].CastShadow();
			box[i].Draw();
		}
		
		//light.updateMatrix();
		wnd.Flush(cam, light);

		wnd.Present();

		if (duration_cast<milliseconds>(steady_clock::now() - timer).count() > 1000)
		{
			//printf("\rFPS: %d TICK: %d Time: %d", fpsCounter, updates);
			std::cout << "FPS: " << fpsCounter << "TICK: " << updates << " Time :"  << std::endl;
			updates = 0;
			fpsCounter = 0;
			timer += milliseconds(1000);
		}
	}


	delete cam;
	return 0;
}