#pragma once
#include "Window.h" // for IMGUI
#include "TextSettings.h"


class GUI
{
public:
	GUI(std::vector<TextSettings>& settings);
	void ControlWindow(const std::string info, bool& textSize, size_t& currSetting);
	void PrestEditorWindow();
private:
	std::vector<TextSettings>& settings;
	std::string oldInfo = "No Messages";
};

