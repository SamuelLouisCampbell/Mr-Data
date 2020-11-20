#pragma once
#include "Window.h"
#include <vector>
#include <string>
#include <random>
#include "Time.h"
#include "BindableBase.h"
#include "TextNode.h"
#include "NDI_Send.h"
#include "Planar.h"
#include "LineMaker.h"

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

	//NDI stuff
	NDI_Send ndi;

	//text control
	Color textCol = { 1.0f, 1.0f, 1.0f, 1.0f };
	Color oldTextCol = { 1.0f, 1.0f, 1.0f, 1.0f };
	float scale = 1.0f;
	float rotation = 0.0f;
	float lineSpacing = 1.5f;
	char buffer[512] = {'\0'};
	std::wstring oldMessage;
	bool holdingLastMsg = false;
	float alpha = 1.0f;
	float deltaAlpha = 1.0f;

	//BG Quad
	std::unique_ptr<Drawable> quad;
	float posX = 1.0f;
	float posY = 1.0f;
	float posZ = 1.0f;

	//Line
	std::unique_ptr<Drawable> line;
	Color lineCol = { 1.0f, 1.0f, 0.0f };
	DirectX::XMFLOAT3 pos_1 = { -1.0f, -1.0f, 1.0f };
	DirectX::XMFLOAT3 pos_2 = { 0.0f, 0.0f, 1.0f };
	
};

