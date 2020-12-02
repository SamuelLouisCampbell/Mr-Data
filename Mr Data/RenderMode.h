#pragma once
#include "Time.h"
#include "BindableBase.h"
#include "TextNode.h"
#include "NDI_Send.h"
#include "Window.h"
#include "UDPClient.h"
#include "UDPServer.h"

class RenderMode
{
public:
	RenderMode(Graphics& gfx);
	void Update(Graphics& gfx);
	void Render(Graphics& gfx);
	void SendNDI(Graphics& gfx); //switched off in debug. Apply painters algorthm for rendering.
private:
	void StringControl(const std::string& ctrlStr, Color& colChange);
private:
	Time time;
	TextNode txt;
	DirectX::SimpleMath::Vector2 centre;

	//NDI stuff
	NDI_Send ndi;

	//UDP stuff
	UDPClient udp_c;
	UDPServer udp_s;

	//text control
	Color textCol = { 1.0f, 1.0f, 1.0f, 1.0f };
	Color oldTextCol = { 1.0f, 1.0f, 1.0f, 1.0f };
	float largeScale = 1.0f;
	float smallScale = 0.35f;
	float currScale = smallScale;
	float lineSpacingLarge = 1.65f;
	float lineSpacingSmall = 0.7f;
	float currLineSpacing = lineSpacingSmall;
	bool currSmall = true;
	float rotation = 0.0f;
	char buffer[512] = { '\0' };
	std::wstring oldMessage;
	bool holdingLastMsg = false;
	float alpha = 1.0f;
	float deltaAlpha = 1.0f;

};

