#include "GUI.h"

void GUI::Load(const char* filename, TextSettings defaultSettings)
{
	int n;
	std::ifstream in(filename, std::ios::binary);
	in.read(reinterpret_cast<char*>(&n), sizeof(n));

	//erase all but the first. 
	tSet.erase(tSet.begin() + 1, tSet.end());
	tSet.reserve(n);
	//load all but the first. 
	for (int i = 1; i < n; i++)
	{
		TextSettings t;
		t.DeSerialize(in);
		tSet.push_back(t);
	}
	tSet[0] = defaultSettings;
	in.close();
}

void GUI::Save(const char* filename)
{
	//save all but the first, first is always restored to default. 
	int n = tSet.size();
	std::ofstream out(filename, std::ios::binary);
	out.write(reinterpret_cast<const char*>(&n), sizeof(n));
	for (int i = 1; i < n; i++ )
	{
		tSet[i].Serialize(out);
	}
	out.close();
}

GUI::GUI(std::vector<TextSettings>& settings, TextSettings defaultSettings)
	:
	tSet(settings),
	defaultSettings(defaultSettings)
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
		ImGui::SameLine();
		if (ImGui::Button("Small Text."))
		{
			textSize = true;
		}
		//Add Buttons for each occupied setting slot;
		for (int i = 0; i < tSet.size(); i++)
		{
			std::stringstream ss;
			ss << "Preset : " << i;
			Color curCol = tSet[i].oldFillCol;
			Color curBor = tSet[i].oldOutlineCol;

			if ((i % 4) > 0)
				ImGui::SameLine();

			ImGui::PushID(i);
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::ImColor(curCol.r,curCol.g,curCol.b));
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, tSet[i].strokeWidth);
			ImGui::PushStyleColor(ImGuiCol_Border, (ImVec4)ImColor::ImColor(curBor.r, curBor.g, curBor.b));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::ImColor(0.2f, 0.2f, 0.2f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::ImColor(0.8f, 0.8f, 0.8f));

			//alternate font color;
			if (curCol.r + curCol.g + curCol.b > 1.5f)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::ImColor(0.0f, 0.0f, 0.0f));
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::ImColor(1.0f, 1.0f, 1.0f));
			}

			//do the buttons;
			if (ImGui::Button(ss.str().c_str(), {100.0f,40.0f}))
			{
				currSettings = i;
			}
			ImGui::PopStyleColor(5);
			ImGui::PopStyleVar(1);
			ImGui::PopID();	
		}

		
		if (ImGui::Button("Save Presets"))
		{
			Save("presets.txt");
		}
		ImGui::SameLine();
		if (ImGui::Button("Load Presets"))
		{
			Load("presets.txt", defaultSettings);
			currSettings = 0;
		}		
	}
	
	ImGui::End();
}

void GUI::PrestEditorWindow(size_t& currSettings, Color& oldFillCol, Color& oldOutlineCol)
{
	//TODO sizing and grouping with conditions.
	//ImGui::SetNextWindowSize({ 300, 600 });
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
