#include <iostream>
#include <string>
#include <stack>

#include "Grid.h"
#include "StateManager\State.h"
#include "StateManager\GameState.h"
#include "StateManager\MainMenu.h"

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

	Camera* cam = new FPSCamera();
	wnd.setMousePositionCallback(cam, &Camera::setMousePos);
	
	std::stack<State *> gameStates;
	std::stack<Shape *> pickingEvents;
	std::stack<int>		keyEvent;

	Light light;
	light.Init(DirectX::XMFLOAT4A(0, 100, 0, 0), DirectX::XMFLOAT4A(-1, -1, -1, 0), DirectX::XMFLOAT4A(1, 1, 1, 1), 420, 420);
	//light.setDir(DirectX::XMFLOAT4A(0, -1, 0, 0));

	

	gameStates.push(new GameState(&pickingEvents, &keyEvent, cam));
	
	while (wnd.isOpen())
	{
		
		wnd.Clear();
		auto currentTime = steady_clock::now();
		wnd.PollEvents();
		auto dt = duration_cast<nanoseconds>(currentTime - time).count();
		time = steady_clock::now();

		unprocessed += dt / freq;

		
		//keyEvent.push();
		

		while (unprocessed > 1)
		{
			updates++;
			unprocessed -= 1;

			if (!gameStates.empty())
			{
				gameStates.top()->Update(1.0f / REFRESH_RATE);

				if (gameStates.top()->Exit()) {
					delete gameStates.top();
					gameStates.pop();
				}
				else
				{
					State * ref = gameStates.top()->NewState();
					if (ref)
						gameStates.push(ref);

				}
			}

		}

		fpsCounter++;
		if (!gameStates.empty())
			gameStates.top()->Draw();

		Shape* picked = nullptr;
		if (GetAsyncKeyState(VK_LBUTTON))		
			picked = wnd.getPicked(cam);

		if (picked)
			pickingEvents.push(picked);
	
		
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

	while (!gameStates.empty())
	{
		delete gameStates.top();
		gameStates.pop();
	}
	delete cam;
	return 0;
}