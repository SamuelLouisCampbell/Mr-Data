#pragma once
#include "Window.h" // for IMGUI
#include "TextSettings.h"


class GUI
{
public:
	GUI(std::vector<TextSettings>& settings);
	void ControlWindow(const std::string info, bool& textSize, size_t& currSettings);
	void PrestEditorWindow(size_t& currSettings, Color& oldFillCol, Color& oldOutlineCol);
private:
	std::vector<TextSettings>& tSet;
	std::string oldInfo = "No Messages";
};

