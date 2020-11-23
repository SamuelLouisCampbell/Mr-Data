#pragma once
#include "Time.h"
#include "BindableBase.h"
#include "TextNode.h"
#include "Window.h"

class TextCursor
{
public:
	TextCursor(Graphics& gfx, const DirectX::XMFLOAT3& pos_in, const float cursorHeight, const Color col_in);
	void UpdateBlink(float dt);
	void UpdatePos(Graphics& gfx, const DirectX::XMFLOAT3& pos_in);
	void UpdateCursorHeight(const float ch);
	void Draw(Graphics& gfx);
private:
	DirectX::XMFLOAT3 pos_2;
	DirectX::XMFLOAT3 pos;
	float cursorHeight;
	std::unique_ptr<Drawable> theCursor;
	Color color;
	bool blink = true;
};

