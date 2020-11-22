#pragma once
#include "Window.h"
#include <vector>
#include <string>
#include <random>
#include "Time.h"
#include "BindableBase.h"
#include "TextNode.h"
#include "NDI_Send.h"
#include "BoxOutline.h"
#include "LineMaker.h"
#include "BoxFill.h"
#include "RenderMode.h"

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
	Window wnd;
	RenderMode rm;

	//Line
	std::vector<std::unique_ptr<class Drawable>> lines;
	static constexpr Color Cyan = { 0.0f, 1.0f, 1.0f };
	static constexpr Color Red = { 1.0f, 0.0f, 0.0f };
	static constexpr DirectX::XMFLOAT3 pos_1 = { 20.0f, 360.0f, 1.0f };
	static constexpr DirectX::XMFLOAT3 pos_2 = { 1260.0f, 360.0f, 1.0f };
	static constexpr DirectX::XMFLOAT3 pos_3 = { 640.0f, 20.0f, 1.0f };
	static constexpr DirectX::XMFLOAT3 pos_4 = { 640.0f, 700.0f, 1.0f };
	static constexpr DirectX::XMFLOAT3 pos_5 = { 20.0f, 20.0f, 1.0f };
	static constexpr DirectX::XMFLOAT3 pos_6 = { 1260.0f, 700.0f, 1.0f };
	
};

