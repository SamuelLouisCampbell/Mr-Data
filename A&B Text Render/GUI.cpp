#include "GUI.h"

GUI::GUI(std::vector<TextSettings>& settings)
	:
	settings(settings)
{
}

void GUI::ControlWindow(const std::string info, bool& textSize, size_t& currSetting)
{
	int i = currSetting;
	if (ImGui::Begin("Text Controls"))
	{

		//display where message came from in imgui
		if (info.size() != 0)
		{
			oldInfo = info;
		}
		ImGui::TextColored({ 0.0f,1.0f, 1.0f, 1.0f }, oldInfo.c_str());
		if (ImGui::Button("Large Text."))
		{
			textSize = false;
		}
		if (ImGui::Button("Small Text."))
		{
			textSize = true;
		}
		if (ImGui::InputInt("Current Setting", &i))
		{
			currSetting = i;
		}

	}
	ImGui::End();
}

void GUI::PrestEditorWindow()
{
}
