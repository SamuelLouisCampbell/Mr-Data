#pragma once
#include "Time.h"
#include "BindableBase.h"
#include "TextNode.h"
#include "Window.h"
#include "TextCursor.h"

class SendTextMode
{
public:
	SendTextMode(Graphics& gfx);
	void Update(Window& wnd);
	void Render(Graphics& gfx);

private:
	void SetCursorCollision(const std::pair<int, int>& input, const DirectX::XMFLOAT3& pos_1,
		const DirectX::XMFLOAT3& pos_2, ImGuiMouseCursor IMGui_Cursor);
	TextNode smallText;
	TextNode largeText;
	TextNode sysText;
	std::wstring message;
	std::vector<std::unique_ptr<class Drawable>> borders;
	DirectX::XMFLOAT3 pos_1;
	DirectX::XMFLOAT3 pos_2;
	DirectX::XMFLOAT3 pos_3;
	DirectX::XMFLOAT3 pos_4;
	Time time;
	TextCursor tc;
};


