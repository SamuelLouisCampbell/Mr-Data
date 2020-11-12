#pragma once
#include "Window.h"
#include <vector>
#include <string>
#include <random>
#include "Time.h"
#include "BindableBase.h"
#include "TextNode.h"

class App
{
public:
	App();
	int Setup();
private:
	void RenderFrame();
	void ComposeFrame();
	
private:
	IMGuiManager img;
	Time time;
	Window wnd;
	TextNode txt;
	DirectX::SimpleMath::Vector2 centre = { 0.0f, 0.0f };

	//text control
	Color textCol = { 1.0f, 1.0f, 1.0f, 1.0f };
	float scale = 1.0f;
	float rotation = 0.0f;
	float lineSpacing = 1.5f;
	char buffer[512] = {'\0'};
	std::wstring oldMessage;
};

