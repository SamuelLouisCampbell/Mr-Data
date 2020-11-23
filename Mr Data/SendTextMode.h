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
	TextNode sysText;
	std::wstring message;
	std::vector<std::unique_ptr<class Drawable>> borders;
	DirectX::XMFLOAT3 pos_1;
	DirectX::XMFLOAT3 pos_2;
	DirectX::XMFLOAT3 pos_3;
	DirectX::XMFLOAT3 pos_4;
};


