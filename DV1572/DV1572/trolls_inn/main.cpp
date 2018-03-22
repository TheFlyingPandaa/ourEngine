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

	Mesh m;
	Camera cam; 
	m.LoadModel("");

	Shape triangle;
	triangle.setMesh(&m);
	RectangleShape r;
	r.setRotation(0, 45, 0);
	RectangleShape r2;
	r2.setRotation(0, -45, 0);

	RectangleShape r3;
	r3.setRotation(0, -90, 0);

	RectangleShape r4;
	r4.setRotation(0, 0, 0);

	RectangleShape r5;
	r5.setRotation(180, 45, 0);
	RectangleShape r6;
	r6.setRotation(180, -45, 0);

	RectangleShape r7;
	r7.setRotation(180, -90, 0);

	RectangleShape r8;
	r8.setRotation(180, 0, 0);

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
			r.Rotate(0, -1, 0);
			r2.Rotate(0, -1, 0);
			r3.Rotate(0, -1, 0);
			r4.Rotate(0, -1, 0);
			r5.Rotate(0, -1, 0);
			r6.Rotate(0, -1, 0);
			r7.Rotate(0, -1, 0);
			r8.Rotate(0, -1, 0);

		}

		fpsCounter++;

		//Draw geometry
		r.Draw();
		r2.Draw();
		r3.Draw();
		r4.Draw();
		r5.Draw();
		r6.Draw();
		r7.Draw();
		r8.Draw();

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