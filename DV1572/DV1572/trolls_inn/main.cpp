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


	Mesh wall;
	wall.LoadModel("trolls_inn/Resources/Box.obj");
	Object3D aWall;
	wall.setDiffuseTexture("trolls_inn/Resources/wood.jpg");
	aWall.setMesh(&wall);


	a.LoadModel("trolls_inn/Resources/Aaakali.txt");

	a.setDiffuseTexture("trolls_inn/Resources/Untitled.bmp");

	RectangleShape r;
	r.setDiffuseTexture("trolls_inn/Resources/Untitled.bmp");
	r.setPos(-1.5, 1.4, 0);

	RectangleShape r2;
	r2.setDiffuseTexture("trolls_inn/Resources/Untitled.bmp");
	r2.setPos(0, 0, 0);

	Light light; 
	light.Init(XMFLOAT4A{ 10,30,20,0 }, XMFLOAT4A{ 0,0,0,0 }, XMFLOAT4A{ 1,1,1,1 }, 50, 50); 


	modelThread.join();
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
			/*for (int i = 0; i < 9; i++)
			{
				shapes[i].Rotate(0, 1, 0);
			}*/
			box.Rotate(0, 1, 0);
			box2.Rotate(0, -1, 0);
			r2.Rotate(0, 1, 0); 
			r.Rotate(1, 1, 0); 
			cam->update();
			Akali.Rotate(0, 0.1, 0);
		}

		fpsCounter++;

		//HUD.Draw();
		HUD.DrawAsHud();
		Akali.Draw();
		//Draw geometry
		/*for (int i = 0; i < 9; i++)
		{
			shapes[i].Draw();
		}*/
		//r.Draw();
		r.TEMPTRANS();
		r.CheckPick();
		r2.Draw();
		r2.CheckPick();
		aWall.Draw();
		aWall.CheckPick();

		Shape* picked = nullptr;
		if (GetAsyncKeyState(VK_LBUTTON))
		{
			picked = wnd.getPicked(cam);
		}
		if (picked)
		{
			picked->Move(-0.01f, 0.0f, 0.0f);
		}


		wnd.Flush(cam,light); 
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