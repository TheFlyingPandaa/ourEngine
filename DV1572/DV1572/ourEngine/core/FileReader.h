#pragma once
#include <string>

namespace FileReader
{
	struct GameSettings
	{
		float width;
		float height;
		bool fullscreen;
	};
	struct GameSaveStates
	{
		float camX, camY, camZ;
	};

	GameSettings SettingsFileRead(const std::string path = "trolls_inn/settings.txt");

	GameSaveStates StatesFileRead(const std::string path = "trolls_inn/stateSave.txt");
	void StatesFileWrite(const GameSaveStates gameState,const std::string path = "trolls_inn/stateSave.txt");
	
}
