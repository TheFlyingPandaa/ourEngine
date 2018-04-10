
#include <iostream>
#include <string>
#include <stack>

#include "../Bobby test/Room/Grid.h"
#include "StateManager/State.h"
#include "StateManager/GameState.h"
#include "StateManager/MainMenu.h"

#include <iostream>
#include <chrono>
//extern "C" {
//	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
//}
//#pragma warning(disable : 4061 4265 4365 4571 4623 4625 4626 4628 4668 4710 4711 4746 4774 4820 4987 5026 5027 5031 5032 5039)

#include "../../ourEngine/core/Audio/Audio.h"
#include "../../ourEngine/core/Font/SpriteBatch.h"
#include "../../ourEngine/core/Font/SpriteFont.h"
#include "../../ourEngine/core/Dx.h"

#ifdef NDEBUG
	#pragma comment (lib, "ourEngine/core/Audio/AudioLibxRL.lib")
	#pragma comment (lib, "ourEngine/core/Font/FontLibxRL.lib")
#else
	#pragma comment (lib, "ourEngine/core/Audio/AudioLibxDB.lib")
	#pragma comment (lib, "ourEngine/core/Font/FontLibxDB.lib")
#endif 


const float REFRESH_RATE = 60.0f;

extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
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
	//wnd.setMousePositionCallback(cam, &Camera::setMousePos);
	
	std::stack<State *> gameStates;
	std::stack<Shape *> pickingEvents;
	std::stack<int>		keyEvent;

	Light light;
	light.Init(DirectX::XMFLOAT4A(0, 100, 0, 0), DirectX::XMFLOAT4A(-1, -1, -1, 0), DirectX::XMFLOAT4A(1, 1, 1, 1), 420, 420);
	//light.setDir(DirectX::XMFLOAT4A(0, -1, 0, 0));

	Mesh test;
	test.MakeRectangle();
	test.setDiffuseTexture("trolls_inn/Resources/wood.jpg");

	gameStates.push(new GameState(&pickingEvents, &keyEvent, cam));

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

	std::unique_ptr<DirectX::SpriteFont> m_font;
	m_font = std::make_unique<SpriteFont>(DX::g_device, L"trolls_inn/Resources/Fonts/myfile.spritefont");
	DirectX::XMVECTOR m_fontPos = DirectX::XMVECTOR{1280/2,720/2,1,1};
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	m_spriteBatch = std::make_unique<SpriteBatch>(DX::g_deviceContext);

	while (wnd.isOpen())
	{
		
		wnd.Clear();
		auto currentTime = steady_clock::now();
		wnd.PollEvents();
		auto dt = duration_cast<nanoseconds>(currentTime - time).count();
		time = steady_clock::now();

		unprocessed += dt / freq;

		if (Input::GetKeyIndex() != -1)
			keyEvent.push(Input::GetKeyIndex());
		

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


			if (Input::isKeyPressed('P') && !pressed)
			{
				if (!play)
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
				
				pressed = true;
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

		


		fpsCounter++;
		if (!gameStates.empty())
			gameStates.top()->Draw();

		Shape* picked = nullptr;
		picked = wnd.getPicked(cam);

		if (picked) {
			pickingEvents.push(picked);
			
		}
		
		wnd.Flush(cam, light);

		m_spriteBatch->Begin();

		const wchar_t* output = L"Magnus Ar gullig #noHomo";

		DirectX::XMVECTOR origin = m_font->MeasureString(output) / 2.f;

		m_font->DrawString(m_spriteBatch.get(), output,
			m_fontPos, Colors::White, 0.f, origin);

		m_spriteBatch->End();

  		wnd.Present();
		wnd.FullReset();

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