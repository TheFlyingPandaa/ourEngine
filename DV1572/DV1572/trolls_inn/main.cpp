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
	auto start = std::chrono::system_clock::now();

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

	Camera* cam = new FPSCamera();
	wnd.setMousePositionCallback(cam, &Camera::setMousePos);

	//cam.setPosition(0, 100, 100);
	Mesh m;
	Mesh a;
	//a.LoadModel("trolls_inn/Resources/Aaakali.txt");
	std::thread modelThread(_loadModelThread,&a, "trolls_inn/Resources/Aaakali.txt");
	m.LoadModel("trolls_inn/Resources/cube.txt");
	Object3D shapes[9];
	int counterx = 0;
	int counterz = 0;
	
	a.setDiffuseTexture("trolls_inn/Resources/Untitled.bmp");
	for (int i = 0; i < 9; i++)
	{
		shapes[i].setMesh(&a);
		shapes[i].setPos((float)counterx++, 0, (float)counterz--);
	}
	Object3D box;
	box.setMesh(&a);
	box.setPos(0, -2, 0);

	Object3D box2;
	box2.setMesh(&a);
	box2.setPos(1, 0, 0);

	RectangleShape r;
	r.setDiffuseTexture("trolls_inn/Resources/Untitled.bmp");
	r.setPos(-1.5, 1.4, 0);

	RectangleShape r2;
	r2.setDiffuseTexture("trolls_inn/Resources/Untitled.bmp");
	r2.setPos(0, 0, 0);

	modelThread.join();
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	std::cout << elapsed_seconds.count() << std::endl;
	
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
			for (int i = 0; i < 9; i++)
			{
				shapes[i].Rotate(0, 1, 0);
			}
			box.Rotate(0, 1, 0);
			box2.Rotate(0, -1, 0);
			cam->update();
		}

		fpsCounter++;

		//Draw geometry
		for (int i = 0; i < 9; i++)
		{
			shapes[i].Draw();
		}
		r.Draw();
		r.CheckPick();
		r2.Draw();
		r2.CheckPick();

		Shape* picked = nullptr;
		if (GetAsyncKeyState(VK_LBUTTON))
		{
			picked = wnd.getPicked(cam);
		}
		if (picked)
		{
			picked->Move(0.01f, 0.0f, 0.0f);
		}


		wnd.Flush(cam); 
		
		wnd.Present();

		if (duration_cast<milliseconds>(steady_clock::now() - timer).count() > 1000)
		{
			printf("\rFPS: %d TICK: %d", fpsCounter, updates);
			updates = 0;
			fpsCounter = 0;
			timer += milliseconds(1000);
		}
	}

	
	delete cam;
	return 0;
}