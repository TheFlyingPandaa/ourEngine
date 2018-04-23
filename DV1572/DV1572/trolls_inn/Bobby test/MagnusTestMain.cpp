#include <chrono>
#include"Room\Grid.h"
#include "Room\RoomCtrl.h"

#include <iostream>

const float REFRESH_RATE = 60.0f;

extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
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
	Grid test(0,0,32,32);
	RoomCtrl ctrl;
	XMINT2 pos = {6, 0 };
	XMINT2 pos2 = { 1, 1 };
	XMINT2 size = { 5,5 };

	ctrl.AddRoom(pos, size, RoomType::reception, test.extractTiles(pos, size));
	ctrl.AddRoom(pos2, size, RoomType::reception, test.extractTiles(pos2, size));

	using namespace std::chrono;
	auto time = steady_clock::now();
	auto timer = steady_clock::now();
	int updates = 0;
	int fpsCounter = 0;
	float freq = 1000000000.0f / REFRESH_RATE;
	float unprocessed = 0;

	Camera* cam = new OrbitCamera(wnd.getSize());

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
			ctrl.Update(cam);
		}

		fpsCounter++;
		
		ctrl.Draw();
		test.Draw();

		/*Shape* picked = nullptr;
		picked = wnd.getPicked(cam);*/

		
			



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