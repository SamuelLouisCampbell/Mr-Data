#pragma once
#include "Color.h"

struct TextSettings 
{
	Color currFillCol = Colors::Black;
	Color oldFillCol = Colors::Black;
	Color currOutlineCol = Colors::White;
	Color oldOutlineColor = Colors::White;
	float largeScale = 1.0f;
	float smallScale = 0.35f;
	float currScale = smallScale;
	float lineSpacing = 1.0f;
	float strokeWidth = 3.0f;
	float kerning = 1.0f;
	bool currSmall = true;
	float rotation = 0.0f;
	char buffer[512] = { '\0' };
	std::wstring oldMessage;
	bool holdingLastMsg = false;
	float alpha = 1.0f;
	float deltaZoom = 0.20f;
	float deltaAlpha = 1.0f;
	float offsetX = 0;
	float offsetY = 0;
};
