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
		//clientPort
		std::getline(defFile, line);
		n = line.find("#");
		substr = line.substr(n + 1, line.size());
		clientPort = std::stoi(substr);
		//IP
		std::getline(defFile, line);
		n = line.find("#");
		IP = line.substr(n + 1, line.size());
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
		Init("assets/defaults.txt");
	}
	if (ImGui::Begin("Setup System"))
	{
		ImGui::InputInt("Outgoing Port Number 0 - 65535", &serverPort);
		ImGui::InputInt("Incoming Port Number 0 - 65535", &clientPort);
		ImGui::InputText("Remote Machine IP", IP.data(), 16U);
		if (ImGui::Button("Compete Setup"))
		{
			SetSetupMode(true);
		}
	}
	ImGui::End();
}

RMData SetupMode::GetRMData() const
{
	RMData rmd{ size_t(clientPort), size_t(serverPort), IP.c_str() };
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
