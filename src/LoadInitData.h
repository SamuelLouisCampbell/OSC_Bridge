#pragma once
#include <fstream>
#include <string>
#include "InitData.h"
#include <assert.h>

class LoadInitData
{
public:
	bool LoadData(const char* fileName)
	{
		isLoaded = true;
		std::string line;
		std::string::size_type n;
		std::ifstream defFile(fileName);
		std::string substr;
		if (defFile.is_open())
		{
			//IP Resolume
			std::getline(defFile, line);
			n = line.find("#");
			data.IPRes = line.substr(n + 1, line.size());
			//IP Pad
			std::getline(defFile, line);
			n = line.find("#");
			data.IPtouch = line.substr(n + 1, line.size());
			//Port to Res
			std::getline(defFile, line);
			n = line.find("#");
			substr = line.substr(n + 1, line.size());
			data.oscSendResPort = std::stoi(substr);
			//Port from Res
			std::getline(defFile, line);
			n = line.find("#");
			substr = line.substr(n + 1, line.size());
			data.oscRecResPort = std::stoi(substr);
			//port to Pad
			std::getline(defFile, line);
			n = line.find("#");
			substr = line.substr(n + 1, line.size());
			data.oscSendTouchPort = std::stoi(substr);
			//port from Pad
			std::getline(defFile, line);
			n = line.find("#");
			substr = line.substr(n + 1, line.size());
			data.oscRecTouchPort = std::stoi(substr);
			//content Target BPM
			std::getline(defFile, line);
			n = line.find("#");
			substr = line.substr(n + 1, line.size());
			data.contentTargetBPM = std::stof(substr);
			//Which Layers to Kill?
			std::getline(defFile, line);
			n = line.find("#");
			substr = line.substr(n + 1, line.size());
			std::stringstream ss;
			ss << substr;
			std::string killNum;
			while (std::getline(ss, killNum, ','))
			{
				data.killCommands.push_back(std::stoi(killNum));
			}
			
			//close out
			defFile.close();
			fileReadOk = true;
			return fileReadOk;
		}
		else
		{
			fileReadOk = false;
			data.IPRes = "127.0.0.1";
			data.IPtouch = "192.168.0.3";
			data.oscSendResPort = 8000;
			data.oscRecResPort = 8001;
			data.oscSendTouchPort = 7000;
			data.oscRecTouchPort = 7001;
			data.contentTargetBPM = 128.0f;
			return fileReadOk;
		}
	}
	const InitData getData()
	{
		assert(isLoaded);
		return data;
	}
private:
	InitData data;
	bool isLoaded = false;
	bool fileReadOk = false;
};

