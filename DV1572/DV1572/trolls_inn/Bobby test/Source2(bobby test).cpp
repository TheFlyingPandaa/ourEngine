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

#include "../../ourEngine/interface/OurMusic.h"


#include "../../ourEngine/core/FileReader.h"
#include "../Mesh Manager/MeshLoaderPlus.h"
#include "../../ourEngine/interface/shape/SkyBoxObject.h"

#include "../../InGameConsole.h"
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

void InitModels()
{
	MLP::GetInstance().LoadMesh(MESH::TERRAIN, "../TerrainLol.txt");
	MLP::GetInstance().LoadMesh(MESH::TREE_BILL, "tree/treeBillboard.obj");
	MLP::GetInstance().LoadMesh(MESH::RECEPTION_HIGH, "Reception/HighReception2.obj");
	MLP::GetInstance().LoadMesh(MESH::RECEPTION_LOW, "Reception/LowReception2.obj");
	MLP::GetInstance().LoadMesh(MESH::TABLE_HIGH, "Table/Table2.obj");
	MLP::GetInstance().LoadMesh(MESH::TABLE_LOW, "Stol.obj");
	MLP::GetInstance().LoadMesh(MESH::BED_HIGH, "Bed/HighBed.obj");
	MLP::GetInstance().LoadMesh(MESH::BED_LOW, "Bed/LowBed.obj");
	MLP::GetInstance().LoadMesh(MESH::BAR_HIGH, "Bar/HighBar2.obj");
	MLP::GetInstance().LoadMesh(MESH::BAR_LOW, "Bar/LowBar2.obj");
	MLP::GetInstance().LoadMesh(MESH::CHAIR_HIGH, "Chair/HighChair2.obj"); 
	MLP::GetInstance().LoadMesh(MESH::CHAIR_LOW, "Chair/LowChair2.obj");
	MLP::GetInstance().LoadMesh(MESH::STOVE, "Stove/Stove2.obj");
	MLP::GetInstance().LoadMesh(MESH::BOX_AABB, "Box.obj");

}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//srand(static_cast<unsigned>(time(0)));

	FileReader::GameSettings gameSettings = FileReader::SettingsFileRead();
	FileReader::GameSaveStates gameLoadState = FileReader::StatesFileRead();

	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
#if _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//	Activation of Console
	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
#endif
	

	Window wnd(hInstance);
	wnd.Init(static_cast<int>(gameSettings.width), static_cast<int>(gameSettings.height), "Trolls_inn", gameSettings.fullscreen);
	
	
	InitModels();
	
	using namespace std::chrono;
	auto time = steady_clock::now();
	auto timer = steady_clock::now();
	int updates = 0;
	int fpsCounter = 0;
	float freq = 1000000000.0f / REFRESH_RATE;
	float unprocessed = 0;
	bool loose = false;
	Camera* cam = new OrbitCamera(wnd.getSize(), XMFLOAT3(gameLoadState.camX,gameLoadState.camY, gameLoadState.camZ));
	//wnd.setMousePositionCallback(cam, &Camera::setMousePos);
	
	std::stack<State *> gameStates;
	std::stack<Shape *> pickingEvents;
	std::stack<int>		keyEvent;

	//Used to manage the time of day. 
	GameTime gameTime;

	MLP::GetInstance().LoadMeshInverted(MESH::SKYBOX, "skybox.obj");

	SkyBoxObject boxy;
	boxy.setMesh(MLP::GetInstance().GetMesh(MESH::SKYBOX));

	//gameStates.push(new GameState(&pickingEvents, &keyEvent, cam
	
	bool pressed = false;
	bool play = false;


	State* gameState = new GameState(&pickingEvents, &keyEvent, cam);

	InGameConsole igc;

	while (wnd.isOpen())
	{	
		wnd.Clear();
		auto currentTime = steady_clock::now();
		wnd.PollEvents();
		auto dt = duration_cast<nanoseconds>(currentTime - time).count();
		time = steady_clock::now();

		unprocessed += (dt / freq) + (Input::isKeyPressed('A')*3);

		while (unprocessed > 1)
		{
			updates++;
			unprocessed -= 1;
			InGameConsole::update(1.0/REFRESH_RATE);

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
			else
			{
				gameState->Update(1.0 / REFRESH_RATE);
				State* ref = gameState->NewState();
				
				if (ref)
					gameStates.push(ref);
			}
			
		}
	
		if(MLP::GetInstance().IsReady(MESH::SKYBOX))
			boxy.Draw();

		InGameConsole::draw();

		Shape* picked = nullptr;
		picked = wnd.getPicked(cam);
		if (picked)
			pickingEvents.push(picked);

		fpsCounter++;
		if (gameState->Exit())
		{
			delete gameState;
			gameState = new MainMenu(&pickingEvents, &keyEvent, cam);
		}
		gameState->Draw();

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

		OurMusic::Update();

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
	// MSDN
	MLP::GetInstance().ClearMeshes();
	MeshHandler::cleanAll();
	OurMusic::Clear();
	return 0;
}

