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

	GameSettings SettingsFileRead(bool &working ,const std::string path = "trolls_inn/settings.txt");
	GameSettings SettingsFileRead(const std::string path = "trolls_inn/settings.txt");


}
