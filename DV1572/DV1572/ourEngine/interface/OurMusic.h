#pragma once
#include "../core/Audio/Audio.h"
#ifdef NDEBUG
#pragma comment (lib, "ourEngine/core/Audio/AudioLibxRL.lib")
#pragma comment (lib, "ourEngine/core/Font/FontLibxRL.lib")
#else
#pragma comment (lib, "ourEngine/core/Audio/AudioLibxDB.lib")
#pragma comment (lib, "ourEngine/core/Font/FontLibxDB.lib")
#endif

#define SOUND std::unique_ptr<DirectX::SoundEffectInstance>

class OurMusic
{
private:
	static DirectX::AudioEngine*					s_audEngine;
	static std::vector<DirectX::SoundEffect*>		s_soundEffects;
	static bool										s_init;
public:
	
	static void Clear();
	static void Update();
	static DirectX::SoundEffectInstance* LoadSound(const std::string & path);
private:
	static void _init();
};