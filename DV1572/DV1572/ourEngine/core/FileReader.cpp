#include "OurEngine.h"
#include "FileReader.h"

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

	return gameSettings;
}

FileReader::GameSaveStates FileReader::StatesFileRead(const std::string path)
{
	std::string line;
	std::ifstream myfile(path);

	FileReader::GameSaveStates gameStates;
	gameStates.camX = 0;
	gameStates.camY = 0;
	gameStates.camZ = 5;
	if (myfile.is_open())
	{
		while (std::getline(myfile, line))
		{

			if (line[0] != '#' || line != "")
			{
				std::istringstream stream(line);
				std::string type;

				stream >> type;
				if (type == "cam")
				{
					float x = 0;
					float y = 0;
					float z = 0;
					sscanf_s(line.c_str(), " %*s %f %f %f", &x, &y,&z);
					gameStates.camX = x;
					gameStates.camY = y;
					gameStates.camZ = z;
				}
				

			}

		}

		myfile.close();
	}
	return gameStates;
}

void FileReader::StatesFileWrite(const GameSaveStates gameState,const std::string path)
{

	std::ofstream myfile;
	myfile.open(path);
	if (myfile.is_open())
	{
		myfile << "cam" << " " << gameState.camX << " " << gameState.camY << " " << gameState.camZ;
	}
	
	
	myfile.close();

}

