#include "OurMusic.h"
#include <fstream>
DirectX::AudioEngine*					OurMusic::s_audEngine = nullptr;
std::vector<DirectX::SoundEffect*>		OurMusic::s_soundEffects;
bool									OurMusic::s_init = false;
using namespace DirectX;

void OurMusic::Clear()
{
	for (auto &s : s_soundEffects)
	{
		delete s;
		s = nullptr;
	}
	delete s_audEngine;
	s_audEngine = nullptr;
	s_soundEffects.clear();
}

void OurMusic::Update()
{
	if (!s_init)
	{
		s_init = true;
		_init();
	}
	if (!s_audEngine->Update())
	{
		// No audio device is active
		if (s_audEngine->IsCriticalError())
		{
			printf("Something is wrong with audio\n");
		}
	}
}

DirectX::SoundEffectInstance* OurMusic::LoadSound(const std::string & path)
{
	if (!s_init)
	{
		s_init = true;
		_init();
	}
	std::string search = path.substr(path.size() - 4);
	if (_strcmpi(search.c_str(), ".wav"))
	{
		printf("\"%s\" is not of the type .wav format", path.c_str());
		return nullptr;
	}
	else
	{
		std::ifstream f(path);
		if (!f.good())
		{
			printf("Could not find the \"%s\"", path.c_str());
			return nullptr;
		}
	}
	

	std::wstring widestr = std::wstring(path.begin(), path.end());
	const wchar_t* soundPath = widestr.c_str();
	SoundEffect * se = new SoundEffect(s_audEngine, soundPath);
	DirectX::SoundEffectInstance* effect = se->CreateInstance().release();
	
	s_soundEffects.push_back(se);
	return effect;
}

void OurMusic::_init()
{
	AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
	eflags = eflags | AudioEngine_Debug;
#endif
	s_audEngine = new AudioEngine(eflags);
}
