#include "SetupMode.h"
#include <fstream>

bool SetupMode::Init(const std::string filename)
{
	isInit = true;
	std::string line;
	std::string::size_type n;
	std::ifstream defFile(filename);
	if (defFile.is_open())
	{
		//serverPort
		std::getline(defFile, line);
		n = line.find("#");
		std::string substr = line.substr(n+1,line.size());
		serverPort = std::stoi(substr);
		//Large Scale Text
		std::getline(defFile, line);
		n = line.find("#");
		substr = line.substr(n + 1, line.size());
		largeScale = std::stof(substr);
		//Small Scale Text
		std::getline(defFile, line);
		n = line.find("#");
		substr = line.substr(n + 1, line.size());
		smallScale = std::stof(substr);
		//Line Spacing Small
		std::getline(defFile, line);
		n = line.find("#");
		substr = line.substr(n + 1, line.size());
		lineSpacing = std::stof(substr);
		//close out
		defFile.close();
		return true;
	}
	return false;
}

void SetupMode::Update()
{
	if (!isInit)
	{
		Init("assets/renderer_defaults.txt");
	}
	if (ImGui::Begin("Setup System"))
	{
		ImGui::InputInt("Outgoing Port Number 0 - 65535", &serverPort);
		if (ImGui::Button("Compete Setup"))
		{
			SetSetupMode(true);
		}
	}
	ImGui::End();
}

RMData SetupMode::GetRMData() const
{
	RMData rmd{ size_t(serverPort), largeScale, smallScale, lineSpacing };
	return rmd;
}

bool SetupMode::SetupComplete() const
{
	return setupComplete;
}

void SetupMode::SetSetupMode(bool setMode)
{
	setupComplete = setMode;
}
