#pragma once
#include "Time.h"
#include "BindableBase.h"
#include "TextNode.h"
#include "NDI_Send.h"
#include "Window.h"

class RenderMode
{
public:
	RenderMode(Graphics& gfx);
	void Update(Graphics& gfx);
	void Render(Graphics& gfx);
	void SendNDI(Graphics& gfx);

private:
	Time time;
	TextNode txt;
	DirectX::SimpleMath::Vector2 centre;

	//NDI stuff
	NDI_Send ndi;

	//text control
	Color textCol = { 1.0f, 1.0f, 1.0f, 1.0f };
	Color oldTextCol = { 1.0f, 1.0f, 1.0f, 1.0f };
	float scale = 1.0f;
	float rotation = 0.0f;
	float lineSpacing = 1.5f;
	char buffer[512] = { '\0' };
	std::wstring oldMessage;
	bool holdingLastMsg = false;
	float alpha = 1.0f;
	float deltaAlpha = 1.0f;

};
