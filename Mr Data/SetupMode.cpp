#include "SetupMode.h"

void SetupMode::Update()
{
	if (ImGui::Begin("Setup System"))
	{
		ImGui::InputInt("Outgoing Port Number 0 - 65535", &serverPort);
		ImGui::InputInt("Incoming Port Number 0 - 65535", &clientPort);
		ImGui::InputText("Remote Machine IP", IP.data(), IP.size());
		if (ImGui::Button("Compete Setup"))
		{
			SetSetupMode(true);
		}
	}
	ImGui::End();
}

RMData SetupMode::GetRMData() const
{
	RMData rmd;
	rmd.clientIP = IP.c_str();
	rmd.clientPort = clientPort;
	rmd.serverPort = serverPort;
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
