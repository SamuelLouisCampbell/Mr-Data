#include "SetupMode.h"

void SetupMode::Update()
{
	if (ImGui::Begin("Setup System"))
	{
		ImGui::InputInt("Outgoing Port Number 0 - 65535", &serverPort);
		ImGui::InputInt("Incoming Port Number 0 - 65535", &clientPort);
		ImGui::InputText("Remote Machine IP", IP.data(), IP.size());
	}
	ImGui::End();
}

void SetupMode::Render()
{
}

RMData SetupMode::GetRMData() const
{
    return RMData();
}

bool SetupMode::InSetupMode() const
{
	return finished;
}

void SetupMode::SetSetupMode(bool setMode)
{
	finished = setMode;
}
