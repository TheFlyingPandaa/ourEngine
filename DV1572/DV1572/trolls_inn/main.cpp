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
	Mesh a;
	//a.LoadModel("trolls_inn/Resources/Aaakali.txt");
	std::thread modelThread(_loadModelThread,&a, "trolls_inn/Resources/Aaakali.txt");
	
	RectangleShape HUD;
	HUD.setDiffuseTexture("trolls_inn/Resources/Untitled.bmp");
	HUD.setPos(-0.073500, -0.041000, 0);
	HUD.setScale(0.04f);

	modelThread.join();
	Object3D Akali;
	a.setDiffuseTexture("trolls_inn/Resources/wood.jpg");
	Akali.setMesh(&a);
	Akali.setScale(0.2, 0.2, 0.2);


	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	std::cout << elapsed_seconds.count() << std::endl;
	
	bool keyPressed = false;
	bool pressedInFrame = false;
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
			Akali.Rotate(0, 0.1, 0);
		}

		fpsCounter++;

		//HUD.Draw();
		HUD.DrawAsHud();
		Akali.Draw();
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