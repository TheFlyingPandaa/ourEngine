#include "../AI_Base/FroggeDebug.h"
#include <iostream>
#include <string>
#include <stack>

#include "../Bobby test/Room/Grid.h"
#include "StateManager/State.h"
#include "StateManager/GameState.h"
#include "StateManager/MainMenu.h"

#include <iostream>
#include <chrono>

#include "../Time Management/GameTime.h"
#include "../../ourEngine/interface/light/PointLight.h"
#include "../Mesh Manager/MeshManager.h"
//extern "C" {
//	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
//}
//#pragma warning(disable : 4061 4265 4365 4571 4623 4625 4626 4628 4668 4710 4711 4746 4774 4820 4987 5026 5027 5031 5032 5039)

#include "../../ourEngine/core/Audio/Audio.h"


#include "../../ourEngine/core/FileReader.h"
#include "../../ourEngine/interface/shape/SkyBoxObject.h"

#ifdef NDEBUG
	#pragma comment (lib, "ourEngine/core/Audio/AudioLibxRL.lib")
	#pragma comment (lib, "ourEngine/core/Font/FontLibxRL.lib")
#else
	#pragma comment (lib, "ourEngine/core/Audio/AudioLibxDB.lib")
	#pragma comment (lib, "ourEngine/core/Font/FontLibxDB.lib")
#endif 

//#include <vld.h>

// MSDN Memory Leak Detection
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

/*#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif*/
// END MSDN



const float REFRESH_RATE = 60.0f;

extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	bool working;
	FileReader::GameSettings gameSettings = FileReader::SettingsFileRead(working);
	FileReader::GameSaveStates gameLoadState = FileReader::StatesFileRead();

	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//	Activation of Console
	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
	

	Window wnd(hInstance);
	//wnd.Init(static_cast<int>(gameSettings.width), static_cast<int>(gameSettings.height), "Trolls_inn", gameSettings.fullscreen, working);
	wnd.Init(static_cast<int>(gameSettings.width), static_cast<int>(gameSettings.height), "Trolls_inn", gameSettings.fullscreen, working);
	//wnd.Init(static_cast<int>(1920), static_cast<int>(1080), "Trolls_inn", gameSettings.fullscreen, working);
	using namespace std::chrono;
	auto time = steady_clock::now();
	auto timer = steady_clock::now();
	int updates = 0;
	int fpsCounter = 0;
	float freq = 1000000000.0f / REFRESH_RATE;
	float unprocessed = 0;

	Camera* cam = new OrbitCamera(wnd.getSize(), XMFLOAT3(gameLoadState.camX,gameLoadState.camY, gameLoadState.camZ));
	//wnd.setMousePositionCallback(cam, &Camera::setMousePos);
	
	std::stack<State *> gameStates;
	std::stack<Shape *> pickingEvents;
	std::stack<int>		keyEvent;

	//Used to manage the time of day. 
	GameTime gameTime;

	Mesh box;
	box.LoadModelInverted("trolls_inn/Resources/skybox.obj");
	box.setDiffuseTexture("trolls_inn/Resources/skybox2.jpg");

	SkyBoxObject boxy;
	boxy.setMesh(&box);

	Object3D table; 
	table.setMesh(MeshHandler::getTable()); 
	table.setPos(20, 0, 20);

	Object3D wallWithWindow; 
	wallWithWindow.setMesh(MeshHandler::getWallWithWindow()); 
	wallWithWindow.setPos(18, 0, 20); 

	//gameStates.push(new GameState(&pickingEvents, &keyEvent, cam));

	std::unique_ptr<AudioEngine> audEngine;
	AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
	#ifdef _DEBUG
	eflags = eflags | AudioEngine_Debug;
	#endif
	audEngine = std::make_unique<AudioEngine>(eflags);
	std::unique_ptr<SoundEffect> soundEffect;
	soundEffect = std::make_unique<SoundEffect>(audEngine.get(), L"trolls_inn/Resources/lolol.wav");
	auto effect = soundEffect->CreateInstance();
	//effect->Play(true);
	
	bool pressed = false;
	bool play = false;


	State* gameState = new GameState(&pickingEvents, &keyEvent, cam);

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


			if (!gameStates.empty())
			{
				gameStates.top()->Update(1.0f / REFRESH_RATE);

				if (gameStates.top()->Exit()) {
					delete gameStates.top();
					gameStates.pop();
				}
				else
				{
					State* ref = gameStates.top()->NewState();
					if (ref)
						gameStates.push(ref);
				}
			}
			else {
				gameState->Update(1.0 / REFRESH_RATE);
				State* ref = gameState->NewState();
				
				if (ref)
					gameStates.push(ref);
			}
			

			if (Input::isKeyPressed('P') && !pressed)
			{
				/*if (!play)
				{
					effect->Stop(false);
					effect->Play(true);
					play = true;
				}
				else
				{
					effect->Play(false);
					effect->Stop(true);
					play = false;
				}
				
				pressed = true;*/
			}
			else if (!Input::isKeyPressed('P') && pressed)
			{
				pressed = false;
			}
		}
		if (!audEngine->Update())
		{
			// No audio device is active
			if (audEngine->IsCriticalError())
			{
			}
		}
		//boxy.setPos(cam->getPosition());
		boxy.Draw();

		Shape* picked = nullptr;
		picked = wnd.getPicked(cam);
		if (picked)
			pickingEvents.push(picked);

		fpsCounter++;
		gameState->Draw();
		table.Draw();
		wallWithWindow.Draw(); 

		if (!gameStates.empty())
		{
			gameStates.top()->Draw();
		}
		else
		{
			gameState->DrawHUD();
		}

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
	
	XMFLOAT3 camPos = cam->getPosition();
	gameLoadState.camX = camPos.x;
	gameLoadState.camY = camPos.y;
	gameLoadState.camZ = camPos.z;

	//FileReader::StatesFileWrite(gameLoadState);

	while (!gameStates.empty())
	{
		delete gameStates.top();
		gameStates.pop();
	}
	delete cam;
	delete gameState;
	MeshHandler::cleanAll();
	return 0;
}

