#include "../ourEngine/interface/Interface.h"
#include <iostream>
#include <chrono>

const float REFRESH_RATE = 60.0f;


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

	Camera cam;
	cam.setPosition(0, 100, 100);
	Mesh m;
	Mesh a;
	a.LoadModel("trolls_inn/Resources/Aaakali.txt");
	m.LoadModel("trolls_inn/Resources/cube.txt");
	Shape shapes[9];
	int counterx = 0;
	int counterz = 0;
	for (int i = 0; i < 9; i++)
	{
		shapes[i].setMesh(&a);
		shapes[i].setPos(counterx++, 0, counterz--);
	}
	Shape box;
	box.setMesh(&a);
	box.setPos(0, -2, 0);

	Shape box2;
	box2.setMesh(&a);
	box2.setPos(1, 0, 0);


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
		}

		fpsCounter++;

		//Draw geometry
		for (int i = 0; i < 9; i++)
		{
			shapes[i].Draw();
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

	

	return 0;
}