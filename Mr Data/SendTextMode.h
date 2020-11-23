#pragma once
#include "Time.h"
#include "BindableBase.h"
#include "TextNode.h"
#include "NDI_Send.h"
#include "Window.h"

class SendTextMode
{
public:
	SendTextMode(Graphics& gfx);
	void Update(Graphics& gfx);
	void Render(Graphics& gfx);

private:

	TextNode smallText;
	TextNode largeText;
	TextNode systemText;
	std::wstring message;
	std::vector<std::unique_ptr<class Drawable>> borders;
};


