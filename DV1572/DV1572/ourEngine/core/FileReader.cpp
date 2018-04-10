#include "FileReader.h"
#include <fstream>
#include <sstream>

FileReader::GameSettings FileReader::SettingsFileRead(bool &working ,const std::string path)
{
	std::string line;
	std::ifstream myfile(path);

	FileReader::GameSettings gameSettings;
	gameSettings.width = 1280;
	gameSettings.height = 720;
	gameSettings.fullscreen = false;
	if (myfile.is_open())
	{
		working = true;
		while (std::getline(myfile, line))
		{

			if (line[0] != '#' || line != "")
			{
				std::istringstream stream(line);
				std::string type;

				stream >> type;
				if (type == "w_width")
				{
					float t = 0;
					sscanf_s(line.c_str()," %*s %f",&t);
					gameSettings.width = t;
				}
				else if(type == "w_height")
				{
					float t = 0;
					sscanf_s(line.c_str(), " %*s %f", &t);
					gameSettings.height = t;
				}
				else if(type == "w_fullscreen")
				{
					float t = 0;
					sscanf_s(line.c_str(), " %*s %f", &t);
					if (t == 1)
					{
						gameSettings.fullscreen = true;
					}
					else
					{
						gameSettings.fullscreen = false;
					}
				}
			
			}
			
		}

		myfile.close();
	}
	else
	{
		working = false;
	}
	return gameSettings;
}

FileReader::GameSettings FileReader::SettingsFileRead(const std::string path)
{
	std::string line;
	std::ifstream myfile(path);

	FileReader::GameSettings gameSettings;
	gameSettings.width = 1280;
	gameSettings.height = 720;
	gameSettings.fullscreen = false;
	if (myfile.is_open())
	{
		while (std::getline(myfile, line))
		{

			if (line[0] != '#' || line != "")
			{
				std::istringstream stream(line);
				std::string type;

				stream >> type;
				if (type == "w_width")
				{
					float t = 0;
					sscanf_s(line.c_str(), " %*s %f", &t);
					gameSettings.width = t;
				}
				else if (type == "w_height")
				{
					float t = 0;
					sscanf_s(line.c_str(), " %*s %f", &t);
					gameSettings.height = t;
				}
				else if (type == "w_fullscreen")
				{
					float t = 0;
					sscanf_s(line.c_str(), " %*s %f", &t);
					if (t == 1)
					{
						gameSettings.fullscreen = true;
					}
					else
					{
						gameSettings.fullscreen = false;
					}
				}

			}

		}

		myfile.close();
	}
	return gameSettings;
}