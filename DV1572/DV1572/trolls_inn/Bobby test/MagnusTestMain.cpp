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
	

	
	Mesh mBox;
	mBox.LoadModel("trolls_inn/Resources/nanosuit/nanosuit.obj");
	
	const int size = 50;
	Object3D box[size];
	for (int i = 0; i < size; i++)
	{
		box[i].setMesh(&mBox);
		box[i].setPos(i *2, 0, 0);
		box[i].setScale(0.1f);
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
			for(auto& b : box)
				b.Rotate(0, 1, 0);
		}

		fpsCounter++;

		for (auto& b : box)
			b.Draw();
		
		wnd.Flush(cam);
		wnd.Present();
		wnd.FullReset();

		if (duration_cast<milliseconds>(steady_clock::now() - timer).count() > 1000)
		{
			std::string title;
			title = "Fps ";
			title += std::to_string(fpsCounter);
			title += " Tick ";
			title += std::to_string(updates);
			wnd.setTitle(title.c_str());
			updates = 0;
			fpsCounter = 0;
			timer += milliseconds(1000);
		}
	}

	
	delete cam;
	return 0;
}