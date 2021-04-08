#pragma once
#include "Window.h" // for IMGUI
#include "TextSettings.h"
#include <fstream>


class GUI
{
public:
	void Load(const char* filename, TextSettings defaultSettings);
	void Save(const char* filename);

	GUI(std::vector<TextSettings>& settings, TextSettings defaultSettings);
	void ControlWindow(const std::string info, bool& textSize, size_t& currSettings);
	void PrestEditorWindow(size_t& currSettings, Color& oldFillCol, Color& oldOutlineCol);
private:
	std::vector<TextSettings>& tSet;
	std::string oldInfo = "No Messages";
	TextSettings defaultSettings;

};

