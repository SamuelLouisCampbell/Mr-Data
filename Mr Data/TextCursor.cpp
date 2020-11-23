#include "TextCursor.h"
#include "LineMaker.h"

TextCursor::TextCursor(Graphics& gfx, const DirectX::XMFLOAT3& pos_in, const float cursorHeight, const Color col_in = Colors::White)
	:
	pos(pos_in),
	color(col_in),
	cursorHeight(cursorHeight)
{
	pos_2 = { pos_in.x, pos_in.y + cursorHeight, pos_in.z };
	theCursor = std::make_unique<LineMaker>(gfx, pos_in, pos_2, col_in);
}

void TextCursor::UpdateBlink(float dt)
{
	if (int(dt*2.0f) % 2 == 0)
	{
		blink = false;
	}
	else
	{
		blink = true;
	}
}

void TextCursor::UpdatePos(Graphics& gfx,const DirectX::XMFLOAT3& pos_in)
{
	theCursor.reset();
	pos_2 = { pos_in.x, pos_in.y + cursorHeight, pos_in.z };
	theCursor = std::make_unique<LineMaker>(gfx, pos_in, pos_2, color);
}

void TextCursor::UpdateCursorHeight(const float ch)
{
	cursorHeight = ch;

}

void TextCursor::Draw(Graphics& gfx)
{
	if (!blink)
	{
		theCursor->Draw(gfx);
	}
}
