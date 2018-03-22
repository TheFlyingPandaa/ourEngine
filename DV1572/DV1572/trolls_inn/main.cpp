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
	wnd.Init(1280, 720, "sourEngine");
	
	using namespace std::chrono;
	auto time = steady_clock::now();
	auto timer = steady_clock::now();
	int updates = 0;
	int fpsCounter = 0;
	float freq = 1000000000.0f / REFRESH_RATE;
	float unprocessed = 0;

	Mesh m;
	Camera cam; 
	m.LoadModel("");

	Shape triangle;
	triangle.setMesh(&m);
	RectangleShape r;
	r.setPos(0, -0.5, 0);
	r.setRotation(90, 0, 0);

	RectangleShape r2;
	r2.setRotation(90, 0, 0);
	r2.setPos(0, 0.5, 0);

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
			r.Rotate(-1, 1, 0.3);
			r2.Rotate(1, -1, -0.3);
		}

		fpsCounter++;

		//Draw geometry
		r.Draw();
		r2.Draw();
		wnd.Flush(cam); 
		
		wnd.Present();
	}

	if (duration_cast<milliseconds>(steady_clock::now() - timer).count() > 1000)
	{
		printf("\rFPS: %d TICK: %d", fpsCounter, updates);
		updates = 0;
		fpsCounter = 0;
		timer += milliseconds(1000);
	}

	return 0;
}