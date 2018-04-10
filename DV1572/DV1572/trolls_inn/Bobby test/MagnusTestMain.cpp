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

	Mesh mBox3;
	mBox3.LoadModel("trolls_inn/Resources/Box.obj");
	mBox3.setNormalTexture("trolls_inn/Resources/NormalMap.jpg");
	mBox3.setDiffuseTexture("trolls_inn/Resources/wood.jpg");

	Mesh mBox4;
	mBox4.LoadModel("trolls_inn/Resources/Box.obj");
	mBox4.setNormalTexture("trolls_inn/Resources/NormalMap.jpg");
	mBox4.setDiffuseTexture("trolls_inn/Resources/wood.jpg");


	const long int NR = 600;

	Object3D box[NR];

	for (int i = 0; i < 150; i++)
	{
		box[i].setMesh(&mBox);
		box[i].setPos(i, 0, 0);
	}
	for (int i = 150; i < 300; i++)
	{
		box[i].setMesh(&mBox2);
		box[i].setPos(i - 150, 1, 0);
	}
	for (int i = 300; i < 450; i++)
	{
		box[i].setMesh(&mBox3);
		box[i].setPos(i - 300, 2, 0);
	}
	for (int i = 450; i < 600; i++)
	{
		box[i].setMesh(&mBox4);
		box[i].setPos(i - 450, 3, 0);
	}



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
		
		if (Input::isMouseLeftPressed())
		{
			for (int i = NR - 1; i > 250; i--)
			{
				box[i].CheckPick();
			}
			for (int i = 0; i <= 250; i++)
			{
				box[i].CheckPick();
			}


			Shape* p = wnd.getPicked(cam);
			if (p)
				p->Move(0, 0, 1);
		}
		
		for(int i = 0; i < NR; i++)
		{
			box[i].Draw();
		}

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