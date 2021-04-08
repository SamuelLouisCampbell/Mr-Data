#pragma once
#include "Color.h"

struct TextSettings 
{
	Color currFillCol;
	Color oldFillCol;
	Color currOutlineCol;
	Color oldOutlineCol;
	float largeScale;
	float smallScale;
	float lineSpacing;
	float strokeWidth;
	float kerning;
	float rotaion;
	float deltaZoom;
	float deltaAlpha;
	float offsetX;
	float offsetY;
};
