#pragma once
#include "Time.h"
#include "BindableBase.h"
#include "TextNode.h"
#include "NDI_Send.h"
#include "Window.h"
#include "UDPClient.h"
#include "UDPServer.h"
#include <thread>

struct RMData
{
	uint16_t clientPort;
	uint16_t serverPort;
	const char* clientIP;
};

class RenderMode
{
public:
	RenderMode(Graphics& gfx, RMData& data);
	~RenderMode();
	void Update(Window& wnd);
	void Render(Graphics& gfx);
	bool returnToSetupMode() const;
private:
	void StringControl(const std::string& ctrlStr, Color& colChange);
private:
	Time time;
	TextNode txt;
	FPS fps;
	DirectX::SimpleMath::Vector2 centre;
	bool returnToSetup = false;


	//UDP stuff
	std::thread serverThread;
	UDPClient echoClient;
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
	float deltaZoom = 0.20f;
	float deltaAlpha = 1.0f;

};


