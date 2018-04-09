#include <chrono>
#include "../../ourEngine/interface/Interface.h"
#include <iostream>

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


	Camera* cam = new OrbitCamera(wnd.getSize());
	cam->update();

	Light light;
	light.Init(DirectX::XMFLOAT4A(0, 100, 0, 0), DirectX::XMFLOAT4A(-1, -1, -1, 0), DirectX::XMFLOAT4A(1, 1, 1, 1), 420, 420);

	Mesh mBox;
	mBox.LoadModel("trolls_inn/Resources/Box.obj");
	mBox.setNormalTexture("trolls_inn/Resources/NormalMap.jpg");
	mBox.setDiffuseTexture("trolls_inn/Resources/wood.jpg");

	Mesh mBox2;
	mBox2.LoadModel("trolls_inn/Resources/Box.obj");
	mBox2.setNormalTexture("trolls_inn/Resources/NormalMap.jpg");
	mBox2.setDiffuseTexture("trolls_inn/Resources/wood.jpg");

	const long int NR = 2;

	Object3D box2;
	Object3D box1;
	box2.setMesh(&mBox);
	box1.setMesh(&mBox2);
	box1.setPos(1, 0, 0);


	while (wnd.isOpen())
	{
		wnd.Clear();
		wnd.PollEvents();
	
		auto currentTime = steady_clock::now();
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
		
		box1.CheckPick();
		box2.CheckPick();
		box1.Draw();
		box2.Draw();

		wnd.getPicked(cam);


		wnd.Flush(cam, light);
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