#include "GUI.h"

GUI::GUI(std::vector<TextSettings>& settings)
	:
	tSet(settings)
{
}

void GUI::ControlWindow(const std::string info, bool& textSize, size_t& currSettings)
{
	int i = currSettings;
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
		if (ImGui::InputInt("Current Setting", &i, 1, 1))
		{
			if (i < 0)
			{
				i = 0;
			}
			else if (i >= 15)
			{
				i = 15;
			}
			currSettings = i;
		}
		
		//Add Buttons for each occupied setting slot;
		for (int i = 0; i < tSet.size(); i++)
		{
			std::stringstream ss;
			ss << "Preset : " << i;
			Color curCol = tSet[i].currFillCol;
			Color curBor = tSet[i].currOutlineCol;

			if ((i % 4) > 0)
				ImGui::SameLine();

			ImGui::PushID(i);
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::ImColor(curCol.r,curCol.g,curCol.b));
			ImGui::PushStyleColor(ImGuiCol_Border, (ImVec4)ImColor::ImColor(curBor.r, curBor.g, curBor.b));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::ImColor(0.2f, 0.2f, 0.2f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::ImColor(0.8f, 0.8f, 0.8f));

			if (curCol.r + curCol.g + curCol.b > 1.5f)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::ImColor(0.0f, 0.0f, 0.0f));
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::ImColor(1.0f, 1.0f, 1.0f));
			}
			if (ImGui::Button(ss.str().c_str(), {100.0f,40.0f}))
			{
				currSettings = i;
			}
			ImGui::PopStyleColor(5);
			ImGui::PopID();	
		}
		
	}
	ImGui::End();
}

void GUI::PrestEditorWindow(size_t& currSettings, Color& oldFillCol, Color& oldOutlineCol)
{
	std::stringstream ss;
	ss << "Preset Control : Preset " << currSettings;
	if (ImGui::Begin(ss.str().c_str()))
	{
		ImGui::InputFloat("Small text size", &tSet[currSettings].smallScale, 0.02f);
		ImGui::InputFloat("Large text size", &tSet[currSettings].largeScale, 0.02f);
		ImGui::InputFloat("Line spacing", &tSet[currSettings].lineSpacing, 0.02f);
		ImGui::InputFloat("Stroke Width", &tSet[currSettings].strokeWidth, 0.02f);
		ImGui::InputFloat("Offset X", &tSet[currSettings].offsetX, 2.0f);
		ImGui::InputFloat("Offset Y", &tSet[currSettings].offsetY, 2.0f);
		ImGui::InputFloat("Kerning", &tSet[currSettings].kerning, 0.2f);
		ImGui::SliderFloat("Delta Alpha (time)", &tSet[currSettings].deltaAlpha, 0.0f, 3.0f);
		ImGui::SliderFloat("Delta Zoom  (time)", &tSet[currSettings].deltaZoom, 0.01f, 1.0f);
		ImGui::ColorPicker4("Fill Color", &oldFillCol.r, ImGuiColorEditFlags_::ImGuiColorEditFlags_AlphaBar);
		ImGui::ColorPicker4("Outine Color", &oldOutlineCol.r, ImGuiColorEditFlags_::ImGuiColorEditFlags_AlphaBar);
	}
	ImGui::End();
}
